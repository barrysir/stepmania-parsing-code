#include "RageFileDriverStd.h"
#include "RageFileManager.h"
#include "RageFile.h"


RageFileObjStd::RageFileObjStd(const RString &path, std::fstream &filestream) : m_sPath(path) {
    std::swap(fs, filestream);

    auto curpos = fs.tellg();
    fs.seekg(0, fs.end);
    auto endpos = fs.tellg();
    fs.seekg(curpos);
    filesize = endpos;
}

RageFileObjStd::~RageFileObjStd() {
    fs.close();
}

int RageFileObjStd::ReadInternal( void *buffer, size_t bytes ) {
    fs.read((char*)buffer, bytes);
    return fs.gcount();
}

int RageFileObjStd::WriteInternal( const void *buffer, size_t bytes ) {
    fs.write((const char*)buffer, bytes);
    return bytes;
}

int RageFileObjStd::SeekInternal( int offset ) {
    fs.seekg(offset);
    return offset;
}

int RageFileObjStd::GetFileSize() const {
    return filesize;
}

RageFileBasic *RageFileDriverStd::Open( const RString &sPath, int mode, int &err ) {
    // specify binary mode (rather than text mode) so \n isn't turned into \r\n
    // (was having a problem with double line breaks [CR][CR][LF])
    std::ios::openmode flags = std::ios::binary;
    if (mode & RageFile::READ) {
        flags |= std::fstream::in;
    }
    if (mode & RageFile::WRITE) {
        flags |= std::fstream::out;
    }
    std::fstream fs(sPath, flags);
    if (!fs) {
        return nullptr;
    }
    RageFileObjStd *file = new RageFileObjStd(sPath, fs);
    return file;
}