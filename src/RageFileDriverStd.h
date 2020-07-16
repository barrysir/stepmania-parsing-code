#pragma once

#include "RageFileBasic.h"
#include "RageFileManager.h"
#include "RageFileDriver.h"
#include <fstream>

class RageFileObjStd: public RageFileObj
{
public:
	RageFileObjStd(const RString &path, std::fstream &filestream);
	~RageFileObjStd();

	int ReadInternal( void *buffer, size_t bytes );
	int WriteInternal( const void *buffer, size_t bytes );
	int SeekInternal( int offset );
	int GetFileSize() const;
    RString GetDisplayPath() const { return m_sPath; }
private:
    RString m_sPath;
    std::fstream fs;
	std::streampos filesize;
};

class RageFileDriverStd : public RageFileDriver
{
public:
	RageFileBasic *Open( const RString &sPath, int mode, int &err );
};