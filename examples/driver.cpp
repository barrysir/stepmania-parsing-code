#include "MsdFile.h"
#include "GameManager.h"
#include "GameConstantsAndTypes.h"
#include "GameState.h"
#include "RageLog.h"
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

void initialize() {
    GAMEMAN = new GameManager();
    LOG = new RageLog();
    GAMESTATE = new GameState();
}

void test_gameman() {
    auto x = GAMEMAN->GetStepsTypeInfo(StepsType_popn_five);
    std::cout << "---- Testing GameManager ----" << std::endl;
    std::cout << "StepsType_popn_five = " << x.szName << " with " << x.iNumTracks << " columns" << std::endl;
    std::cout << "pnm-five = " << (int)(GAMEMAN->StringToStepsType("pnm-five")) << std::endl;
    std::cout << "-----------------------------" << std::endl;
}

int main(int argc, char *argv[]) {
    initialize();
    test_gameman();

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