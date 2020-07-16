#include "global.h"
#include "RageFileManager.h"
#include <filesystem>
#include <vector>
#include <set>

bool IsAFile( const RString &sPath )
{
    // I think this is the right function?
    return std::filesystem::is_regular_file((const char*)(sPath));
}

bool IsADirectory( const RString &sPath )
{
    return std::filesystem::is_directory((const char*)(sPath));
}

bool DoesFileExist( const RString &sPath )
{
    return std::filesystem::exists((const char*)(sPath));
}

// --------------------------------
//  File
// --------------------------------
class FileSet;
struct File
{
	RString name;
	RString lname;

	void SetName( const RString &fn )
	{
		name = fn;
		lname = name;
		lname.MakeLower();
	}
	
	bool dir;
	int size;
	/* Modification time of the file.  The contents of this is undefined, except that
	 * when the file has been modified, this value will change. */
	int hash;

	/* Private data, for RageFileDrivers. */
	void *priv;

	/* If this is non-nullptr, and dir is true, this is a pointer to the FileSet containing
	 * the directory contents.  (This is a cache; it isn't always set.) */
	const FileSet *dirp;

	File() { dir=false; dirp=nullptr; size=-1; hash=-1; priv=nullptr;}
	File( const RString &fn )
	{
		SetName( fn );
		dir=false; size=-1; hash=-1; priv=nullptr; dirp=nullptr;
	}
	
	bool operator< (const File &rhs) const { return lname<rhs.lname; }

	bool equal(const File &rhs) const { return lname == rhs.lname; }
	bool equal(const RString &rhs) const
	{
		RString l = rhs;
		l.MakeLower();
		return lname == l;
	}
};

// --------------------------------
//  FileSet
// --------------------------------
/** @brief This represents a directory. */
struct FileSet
{
	set<File> files;
	// RageTimer age;

	/*
	 * If m_bFilled is false, this FileSet hasn't completed being filled in yet; it's
	 * owned by the thread filling it in.  Wait on FilenameDB::m_Mutex and retry until
	 * it becomes true.
	 */
	bool m_bFilled;

	FileSet() { m_bFilled = true; }

	void GetFilesMatching(
		const RString &sBeginning, const RString &sContaining, const RString &sEnding,
		vector<RString> &asOut, bool bOnlyDirs ) const;
	void GetFilesEqualTo( const RString &pat, vector<RString> &out, bool bOnlyDirs ) const;

	RageFileManager::FileType GetFileType( const RString &sPath ) const;
	int GetFileSize( const RString &sPath ) const;
	int GetFileHash( const RString &sPath ) const;
};

/* Search for "beginning*containing*ending". */
void FileSet::GetFilesMatching( const RString &sBeginning_, const RString &sContaining_, const RString &sEnding_, vector<RString> &asOut, bool bOnlyDirs ) const
{
	/* "files" is a case-insensitive mapping, by filename.  Use lower_bound to figure
	 * out where to start. */
	RString sBeginning = sBeginning_;
	sBeginning.MakeLower();
	RString sContaining = sContaining_;
	sContaining.MakeLower();
	RString sEnding = sEnding_;
	sEnding.MakeLower();

	set<File>::const_iterator i = files.lower_bound( File(sBeginning) );
	for( ; i != files.end(); ++i )
	{
		const File &f = *i;

		if( bOnlyDirs && !f.dir )
			continue;

		const RString &sPath = f.lname;

		/* Check sBeginning. Once we hit a filename that no longer matches sBeginning,
		 * we're past all possible matches in the sort, so stop. */
		if( sBeginning.size() > sPath.size() )
			break; /* can't start with it */
		if( sPath.compare(0, sBeginning.size(), sBeginning) )
			break; /* doesn't start with it */

		/* Position the end starts on: */
		int end_pos = int(sPath.size())-int(sEnding.size());

		/* Check end. */
		if( end_pos < 0 )
			continue; /* can't end with it */
		if( sPath.compare(end_pos, string::npos, sEnding) )
			continue; /* doesn't end with it */

		/* Check sContaining.  Do this last, since it's the slowest (substring
		 * search instead of string match). */
		if( !sContaining.empty() )
		{
			size_t pos = sPath.find( sContaining, sBeginning.size() );
			if( pos == sPath.npos )
				continue; /* doesn't contain it */
			if( pos + sContaining.size() > unsigned(end_pos) )
				continue; /* found it but it overlaps with the end */
		}

		asOut.push_back( f.name );
	}
}

void FileSet::GetFilesEqualTo( const RString &sStr, vector<RString> &asOut, bool bOnlyDirs ) const
{
	set<File>::const_iterator i = files.find( File(sStr) );
	if( i == files.end() )
		return;

	if( bOnlyDirs && !i->dir )
		return;

	asOut.push_back( i->name );
}

RageFileManager::FileType FileSet::GetFileType( const RString &sPath ) const
{
	set<File>::const_iterator i = files.find( File(sPath) );
	if( i == files.end() )
		return RageFileManager::TYPE_NONE;

	return i->dir? RageFileManager::TYPE_DIR:RageFileManager::TYPE_FILE;
}

int FileSet::GetFileSize( const RString &sPath ) const
{
	set<File>::const_iterator i = files.find( File(sPath) );
	if( i == files.end() )
		return -1;
	return i->size;
}

int FileSet::GetFileHash( const RString &sPath ) const
{
	set<File>::const_iterator i = files.find( File(sPath) );
	if( i == files.end() )
		return -1;
	return i->hash + i->size;
}

// --------------------------------
//  GetDirListing
// --------------------------------
void GetFilesMatching( const RString &sDir, const RString &sBeginning, const RString &sContaining, const RString &sEnding, vector<RString> &asOut, bool bOnlyDirs )
{
    // fill a file set with paths in sDir
	FileSet fs;
    for(auto& p: std::filesystem::directory_iterator((const char*)(sDir))) {
        std::string name = p.path().string();
        File f(name);
        f.dir = std::filesystem::is_directory(p);
        fs.files.insert(f);
    }

	fs.GetFilesMatching( sBeginning, sContaining, sEnding, asOut, bOnlyDirs );
}

void GetFilesEqualTo( const RString &sDir, const RString &sFile, vector<RString> &asOut, bool bOnlyDirs )
{
    // fill a file set with paths in sDir
	FileSet fs;
    for(auto& p: std::filesystem::directory_iterator((const char*)(sDir))) {
        std::string name = p.path().string();
        File f(name);
        f.dir = std::filesystem::is_directory(p);
        fs.files.insert(f);
    }

	fs.GetFilesEqualTo( sFile, asOut, bOnlyDirs );
}

void GetFilesSimpleMatch( const RString &sDir, const RString &sMask, vector<RString> &asOut, bool bOnlyDirs )
{
	/* Does this contain a wildcard? */
	size_t first_pos = sMask.find_first_of( '*' );
	if( first_pos == sMask.npos )
	{
		/* No; just do a regular search. */
		GetFilesEqualTo( sDir, sMask, asOut, bOnlyDirs );
		return;
	}
	size_t second_pos = sMask.find_first_of( '*', first_pos+1 );
	if( second_pos == sMask.npos )
	{
		/* Only one *: "A*B". */
		/* XXX: "_blank.png*.png" shouldn't match the file "_blank.png". */
		GetFilesMatching( sDir, sMask.substr(0, first_pos), RString(), sMask.substr(first_pos+1), asOut, bOnlyDirs );
		return;
	}

	/* Two *s: "A*B*C". */
	GetFilesMatching( sDir, 
		sMask.substr(0, first_pos),
		sMask.substr(first_pos+1, second_pos-first_pos-1),
		sMask.substr(second_pos+1), asOut, bOnlyDirs );
}

void GetDirListing( const RString &sPath_, vector<RString> &asAddTo, bool bOnlyDirs, bool bReturnPathToo )
{
	RString sPath = sPath_;
//	LOG->Trace( "GetDirListing( %s )", sPath.c_str() );

	ASSERT( !sPath.empty() );

	/* Strip off the last path element and use it as a mask. */
	size_t  pos = sPath.find_last_of( '/' );
	RString fn;
	if( pos == sPath.npos )
	{
		fn = sPath;
		sPath = "";
	}
	else
	{
		fn = sPath.substr(pos+1);
		sPath = sPath.substr(0, pos+1);
	}

	/* If the last element was empty, use "*". */
	if( fn.size() == 0 )
		fn = "*";

	unsigned iStart = asAddTo.size();
	GetFilesSimpleMatch( sPath, fn, asAddTo, bOnlyDirs );

	if( bReturnPathToo && iStart < asAddTo.size() )
	{
		while( iStart < asAddTo.size() )
		{
			asAddTo[iStart].insert( 0, sPath );
			iStart++;
		}
	}
}