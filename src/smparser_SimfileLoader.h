/**
 * A helper class to provide a flat interface for parsing operations
 */
#pragma once 

#include <string>
#include "Song.h"
#include "MsdFile.h"

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
    
    // Guess the simfile type of a file from its name.
    FileType GetTypeFromFilename(const std::string &filepath);
    
    // Get the typical extension of a simfile type.
    std::string GetExtensionFromType(FileType type);

    // Prepare a filepath before passing it into the Stepmania code.
    // (Converts it to slashes, and adds a trailing slash if there isn't already one if as_directory is specified)
    std::string CleanPath(const std::string &path, bool as_directory);

    // Get the path of the file that would be loaded when calling LoadFromDir()
    std::string GetFileLoadedFromDir(const std::string &dirpath);

    // simfile loading/saving functions
    bool LoadFromDir(const std::string &dirpath, Song &out);
    bool Load(const std::string &filepath, Song &out, FileType format = DEFAULT);
    bool Save(const std::string &filepath, Song &out, FileType format = DEFAULT);
    bool LoadFromFile(const std::string &filepath, Song &out, FileType format = DEFAULT) {  // alias for Load
        return Load(filepath, out, format);
    }

    // msd parsing
    bool LoadMsd(const std::string &filepath, MsdFile &out, bool unescape=true);

    // LoadFromFile(GetFileLoadedFromDir(path), s) <=> LoadFromDir(path, s)

    // possibly load from string, save to string?
};