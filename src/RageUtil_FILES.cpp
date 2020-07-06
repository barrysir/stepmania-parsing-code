/*
Implementations of the filesystem utility functions in RageUtil.h
    DOES NOT WORK  (yet)
*/

/* Return true if "dir" is empty or does not exist. */
bool DirectoryIsEmpty( const RString &sDir )
{
	if( sDir.empty() )
		return true;
	if( !DoesFileExist(sDir) )
		return true;

	vector<RString> asFileNames;
	GetDirListing( sDir, asFileNames );
	return asFileNames.empty();
}

// path is a .redir pathname. Read it and return the real one.
RString DerefRedir( const RString &_path )
{
	RString sPath = _path;

	for( int i=0; i<100; i++ )
	{
		if( GetExtension(sPath) != "redir" )
			return sPath;

		RString sNewFileName;
		GetFileContents( sPath, sNewFileName, true );

		// Empty is invalid.
		if( sNewFileName == "" )
			return RString();

		RString sPath2 = Dirname(sPath) + sNewFileName;

		CollapsePath( sPath2 );

		sPath2 += "*";

		vector<RString> matches;
		GetDirListing( sPath2, matches, false, true );

		if( matches.empty() )
			RageException::Throw( "The redirect \"%s\" references a file \"%s\" which doesn't exist.", sPath.c_str(), sPath2.c_str() );
		else if( matches.size() > 1 )
			RageException::Throw( "The redirect \"%s\" references a file \"%s\" with multiple matches.", sPath.c_str(), sPath2.c_str() );

		sPath = matches[0];
	}

	RageException::Throw( "Circular redirect \"%s\".", sPath.c_str() );
}

bool GetFileContents( const RString &sPath, RString &sOut, bool bOneLine )
{
	// Don't warn if the file doesn't exist, but do warn if it exists and fails to open.
	if( !IsAFile(sPath) )
		return false;

	RageFile file;
	if( !file.Open(sPath) )
	{
		LOG->Warn( "GetFileContents(%s): %s", sPath.c_str(), file.GetError().c_str() );
		return false;
	}

	// todo: figure out how to make this UTF-8 safe. -aj
	RString sData;
	int iGot;
	if( bOneLine )
		iGot = file.GetLine( sData );
	else
		iGot = file.Read( sData, file.GetFileSize() );

	if( iGot == -1 )
	{
		LOG->Warn( "GetFileContents(%s): %s", sPath.c_str(), file.GetError().c_str() );
		return false;
	}

	if( bOneLine )
		StripCrnl( sData );

	sOut = sData;
	return true;
}

bool GetFileContents( const RString &sFile, vector<RString> &asOut )
{
	RageFile file;
	if( !file.Open(sFile) )
	{
		LOG->Warn( "GetFileContents(%s): %s", sFile.c_str(), file.GetError().c_str() );
		return false;
	}

	RString sLine;
	while( file.GetLine(sLine) )
		asOut.push_back( sLine );
	return true;
}

bool FileCopy( const RString &sSrcFile, const RString &sDstFile )
{
	if( !sSrcFile.CompareNoCase(sDstFile) )
	{
		LOG->Warn( "Tried to copy \"%s\" over itself", sSrcFile.c_str() );
		return false;
	}

	RageFile in;
	if( !in.Open(sSrcFile, RageFile::READ) )
		return false;

	RageFile out;
	if( !out.Open(sDstFile, RageFile::WRITE) )
		return false;

	RString sError;
	if( !FileCopy(in, out, sError) )
	{
		LOG->Warn( "FileCopy(%s,%s): %s",
				sSrcFile.c_str(), sDstFile.c_str(), sError.c_str() );
		return false;
	}

	return true;
}

bool FileCopy( RageFileBasic &in, RageFileBasic &out, RString &sError, bool *bReadError )
{
	for(;;)
	{
		RString data;
		if( in.Read(data, 1024*32) == -1 )
		{
			sError = ssprintf( "read error: %s", in.GetError().c_str() );
			if( bReadError != nullptr )
			{
				*bReadError = true;
			}
			return false;
		}
		if( data.empty() )
		{
			break;
		}
		int i = out.Write(data);
		if( i == -1 )
		{
			sError = ssprintf( "write error: %s", out.GetError().c_str() );
			if( bReadError != nullptr )
			{
				*bReadError = false;
			}
			return false;
		}
	}

	if( out.Flush() == -1 )
	{
		sError = ssprintf( "write error: %s", out.GetError().c_str() );
		if( bReadError != nullptr )
		{
			*bReadError = false;
		}
		return false;
	}

	return true;
}