/**
 * A helper class to provide a flat interface for parsing operations
 */
#pragma once 

#include <string>
#include "Song.h"

class SimfileLoader {
private:
    bool SaveToSSCFile(const RString &sPath, Song &out);
    bool SaveToSMFile(const RString &sPath, Song &out);
public:
    enum FileType {
        DEFAULT,
        SSC,
        SM,
        DWI,
        SMA,
        BMS,
        KSF,
        JSON,
        NONE
    };

    // going to keep these functions non-static,
    // might add flags to set parser options later
    
    FileType GetTypeFromFilename(const std::string &filepath);
    std::string GetExtensionFromType(FileType type);

    bool LoadFromDir(const std::string &filepath, Song &out);
    bool Load(const std::string &filepath, Song &out, FileType format = DEFAULT);
    bool Save(const std::string &filepath, Song &out, FileType format = DEFAULT);
    bool LoadFromFile(const std::string &filepath, Song &out, FileType format = DEFAULT) {  // alias for Load
        return Load(filepath, out, format);
    }

    std::string GetFileLoadedFromDir(const std::string &dirpath);
    // LoadFromFile(GetFileLoadedFromDir(path), s) <=> LoadFromDir(path, s)

    // possibly load from string, save to string?
};