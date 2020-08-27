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

/************************************
 *         Static functions 
 ***********************************/

    // Guess a simfile's type from its name.
    static FileType GetTypeFromFilename(const std::string &filepath);
    
    // Get the typical extension of a simfile type.
    static std::string GetExtensionFromType(FileType type);

    // Prepare a filepath before passing it into the Stepmania code.
    //  - Converts relative paths to absolute paths
    //  - Converts paths to forwards slashes
    //  - Adds a trailing slash for directories (if as_directory is true)
    static std::string CleanPath(const std::string &path, bool as_directory);

    // Get the path of the file that would be loaded when calling LoadFromDir()
    static std::string GetFileLoadedFromDir(const std::string &dirpath);

/*************************************
 *    Loading/saving functions
 ************************************/
    // rule: LoadFromFile(GetFileLoadedFromDir(path), s) <=> LoadFromDir(path, s)

    bool LoadFromDir(const std::string &dirpath, Song &out);
    bool Load(const std::string &filepath, Song &out, FileType format = DEFAULT);
    bool Save(const std::string &filepath, Song &out, FileType format = DEFAULT);
    bool LoadFromFile(const std::string &filepath, Song &out, FileType format = DEFAULT) {  // alias for Load
        return Load(filepath, out, format);
    }

    // msd parsing
    bool LoadMsd(const std::string &filepath, MsdFile &out, bool unescape=true);

    // todo: possibly load from string, save to string?
};