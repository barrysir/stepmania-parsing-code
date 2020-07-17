#include "MsdFile.h"
#include "GameManager.h"
#include "GameConstantsAndTypes.h"
#include "GameState.h"
#include "RageLog.h"
#include "NoteData.h"
#include "Song.h"
#include "NotesLoaderSM.h"
#include "NotesWriterSM.h"
#include "ActorUtil.h"
#include <iostream>
#include <filesystem>

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
    ActorUtil::InitFileTypeLists();
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

void print_song(const Song &s) {
    std::cout << "=====" << s.m_sArtist << " - " << s.m_sMainTitle << "=====" << std::endl;
    std::cout << "Directory: " << s.GetSongDir() << std::endl;
    std::cout << "File path: " << s.GetSongFilePath() << std::endl;
    std::cout << "Audio File: " << s.m_sMusicFile << std::endl;
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

void test_smfile(const std::string &filepath, const std::string &outpath) {
    std::cout << "Testing SMLoader functions" << std::endl;
    Song s;
    SMLoader loader;
    if (std::filesystem::is_directory(filepath)) {
        bool success = loader.LoadFromDir(filepath, s);
        if (!success) {
            std::cout << "Could not load from directory " << filepath << "!" << std::endl;
            return;
        }
    } else {
        bool success = loader.LoadFromSimfile(filepath, s);
        if (!success) {
            std::cout << "Could not load simfile at " << filepath << "!" << std::endl;
            return;
        }
    }

    print_song(s);
    NotesWriterSM::Write(outpath, s, s.GetAllSteps());

    {
        Song s2;
        if (std::filesystem::is_directory(filepath)) {
            std::cout << "Testing Song::LoadFromSongDir" << std::endl;
            bool success = s2.LoadFromSongDir(filepath);
            if (!success) {
                std::cout << "Could not load from directory " << filepath << "!" << std::endl;
                return;
            }
            print_song(s2);
        }
    }
}

int main(int argc, char *argv[]) {
    initialize();
    test_gameman();

    if (argc <= 1) {
        std::cout << "Usage: " << argv[0] << " [file path]" << std::endl;
        return 0;
    }
    if (std::string(argv[1]).find('\\') != std::string::npos) {
        std::cerr << "Warning - pack has backslashes! The SM parsing code only supports forward slashes. Converting them to forward slashes for you." << std::endl;
    }
    std::filesystem::path filepath(argv[1]);
    test_smfile(filepath.generic_string(), "output.sm");
    return 0;
}