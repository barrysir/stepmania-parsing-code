#include "RageFileManager.h"
#include <filesystem>

bool IsAFile( const RString &sPath )
{
    // I think this is the right function?
    return std::filesystem::is_regular_file((const char*)(sPath));
}

bool DoesFileExist( const RString &sPath )
{
    return std::filesystem::exists((const char*)(sPath));
}