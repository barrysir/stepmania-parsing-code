#include "MsdFile.h"
#include "GameManager.h"
#include "GameConstantsAndTypes.h"
#include "GameState.h"
#include "RageLog.h"
#include "NoteData.h"
#include "Song.h"
#include "NotesLoaderSM.h"
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

void test_msdfile(const std::string &filepath) {
    MsdFile m;
    m.ReadFile(filepath, true);
    // m.ReadFromString(filepath, true);
    print_msd(m);
}

void test_smfile(const std::string &filepath) {
    Song s;
    SMLoader loader;
    loader.LoadFromSimfile(filepath, s);
    std::cout << s.m_sArtist << " - " << s.m_sMainTitle << std::endl;
    std::cout << "BPMs: ";
    for (auto &p : s.m_SongTiming.ToVectorString(TimingSegmentType::SEGMENT_BPM)) {
        std::cout << p << ',';
    }    
    std::cout << std::endl;

    std::cout << "--------- Steps ---------" << std::endl;
    // dance-single Beginner 23 (xx steps)
    for (Steps *steps : s.GetAllSteps()) {
        NoteData test;
        steps->GetNoteData(test);
        std::cout
            << steps->m_StepsTypeStr << " "
            << DifficultyToString(steps->GetDifficulty()) << " "
            << steps->GetMeter() << " "
            << "(" << test.GetNumTapNotesNoTiming() << " steps"
            << ", " << steps->GetCredit() << ")"
            << std::endl;
    }
}

int main(int argc, char *argv[]) {
    initialize();
    test_gameman();

    if (argc <= 1) {
        std::cout << "Usage: " << argv[0] << " [file path]" << std::endl;
        return 0;
    }
    test_smfile(argv[1]);
    return 0;
}