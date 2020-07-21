#include "smparser.h"
#include "MsdFile.h"
#include <filesystem>
#include <iostream>

void print_msd(const MsdFile &m) {
    for (unsigned i=0; i<m.GetNumValues(); i++) {
        std::cout << '[';
        for (unsigned j=0; j<m.GetNumParams(i); j++) {
            if (j != 0) {
                std::cout << ", ";
            }
            std::cout << '"' << m.GetParam(i, j) << '"';
        }
        std::cout << ']' << std::endl;
    }
}

void test_msdfile(const std::string &filepath) {
    MsdFile m;
    m.ReadFile(filepath, true);
    print_msd(m);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cout << "Data dumper for files in the MSD file format." << std::endl;
        std::cout << "Usage: " << argv[0] << " [file path]" << std::endl;
        return 0;
    }
    
    SMParserLibrary smparserlibrary;

    if (std::string(argv[1]).find('\\') != std::string::npos) {
        std::cerr << "Warning - pack has backslashes! The SM parsing code only supports forward slashes. Converting to a valid format for you." << std::endl;
    }
    std::filesystem::path filepath(argv[1]);
    test_msdfile(filepath.generic_string());
    return 0;
}