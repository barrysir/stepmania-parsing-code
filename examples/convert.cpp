#include "smparser.h"
#include "smparser_SimfileLoader.h"
#include "NoteData.h"
#include <filesystem>
#include <iostream>

void convertsim(const std::string &filepath, SimfileLoader::FileType outputtype, const std::string &outputpath) {
    SimfileLoader loader;
    Song s;
    if (std::filesystem::is_directory(filepath)) {
        bool success = loader.LoadFromDir(filepath, s);
        if (!success) {
            std::cout << "Could not load from directory " << filepath << "!" << std::endl;
            return;
        }
    } else {
        bool success = loader.LoadFromFile(filepath, s);
        if (!success) {
            std::cout << "Could not load simfile at " << filepath << "!" << std::endl;
            return;
        }
    }

    bool success = loader.Save(outputpath, s, outputtype);
    if (!success) {
        std::cout << "Could not save simfile to " << outputpath << "!" << std::endl;
        return;
    }
}

void MakeLower(std::string &data) {
    std::transform(data.begin(), data.end(), data.begin(),
    [](unsigned char c){ return std::tolower(c); });
}

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        std::cout << "Testing script. Convert a Stepmania file to a different format." << std::endl;
        std::cout << "Usage: " << argv[0] << " [file path]" << " [output type]" << " (output path)" << std::endl;
        return 0;
    }
    
    SMParserLibrary smparserlibrary;

    // Parse argument 1 for filepath
    // if (std::string(argv[1]).find('\\') != std::string::npos) {
    //     std::cerr << "Warning - path has backslashes! The SM parsing code only supports forward slashes. Converting to a valid format for you." << std::endl;
    // }
    std::filesystem::path filepath(argv[1]);

    // Parse argument 2 for output type
    SimfileLoader::FileType outputtype;
    {
        std::unordered_map<std::string, SimfileLoader::FileType> LOOKUP_OUTPUT_TYPES{
            {"ssc", SimfileLoader::SSC},
            {"sm", SimfileLoader::SM},
            {"dwi", SimfileLoader::DWI},
            {"json", SimfileLoader::JSON},
        };

        std::string output_type_name(argv[2]);
        MakeLower(output_type_name);

        auto it = LOOKUP_OUTPUT_TYPES.find(output_type_name);
        if (it != LOOKUP_OUTPUT_TYPES.end()) {
            outputtype = it->second;
        } else {
            std::cerr << "output type " << "\"" << output_type_name << "\"" << " is not supported" << std::endl;
            return 0;
        }
    }

    // Parse argument 3 for output path
    std::filesystem::path outputpath;
    if (argc > 3) {
        // if (std::string(argv[3]).find('\\') != std::string::npos) {
        //     std::cerr << "Warning - output path has backslashes! The SM parsing code only supports forward slashes. Converting to a valid format for you." << std::endl;
        // }
        outputpath = argv[3];
    } else {
        SimfileLoader loader;
        outputpath = filepath;
        outputpath.replace_extension(loader.GetExtensionFromType(outputtype));
    }

    convertsim(filepath.generic_string(), outputtype, outputpath.generic_string());
    return 0;
}