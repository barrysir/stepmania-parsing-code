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

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cout << "Usage: " << argv[0] << " [file path]" << std::endl;
        return 0;
    }
    MsdFile m;
    m.ReadFile(argv[1], true);
    // m.ReadFromString(argv[1], true);
    print_msd(m);
    return 0;
}