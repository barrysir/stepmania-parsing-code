#ifndef RAGE_FILE_MANAGER_H
#define RAGE_FILE_MANAGER_H

class RageFileManager
{
public:	
	enum FileType { TYPE_FILE, TYPE_DIR, TYPE_NONE };
};

void GetDirListingWithMultipleExtensions(const RString &sPath,
		vector<RString> const& ExtensionList, vector<RString> &AddTo,
		bool bOnlyDirs= false, bool bReturnPathToo= false);
#endif