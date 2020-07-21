#include "smparser.h"
#include "smparser_SimfileLoader.h"
#include "NoteData.h"
#include <filesystem>
#include <iostream>

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

void dump_simfile(const std::string &filepath) {
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

    print_song(s);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cout << "Data dumper for Stepmania-supported files (SSC, SM, SMA, DWI, BMS, KSF)" << std::endl;
        std::cout << "Usage: " << argv[0] << " [file path]" << std::endl;
        return 0;
    }
    
    SMParserLibrary smparserlibrary;

    if (std::string(argv[1]).find('\\') != std::string::npos) {
        std::cerr << "Warning - pack has backslashes! The SM parsing code only supports forward slashes. Converting to a valid format for you." << std::endl;
    }
    std::filesystem::path filepath(argv[1]);
    dump_simfile(filepath.generic_string());
    return 0;
}