#include "smparser.h"
#include "smparser_SimfileLoader.h"
#include "MsdFile.h"
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
    // std::string filepath = SimfileLoader().CleanPath(_filepath, false);
    // m.ReadFile(filepath, true);
    
    SimfileLoader loader;
    loader.LoadMsd(filepath, m);
    print_msd(m);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cout << "Data dumper for files in the MSD file format." << std::endl;
        std::cout << "Usage: " << argv[0] << " [file path]" << std::endl;
        return 0;
    }
    
    SMParserLibrary smparserlibrary;
    test_msdfile(argv[1]);
    return 0;
}