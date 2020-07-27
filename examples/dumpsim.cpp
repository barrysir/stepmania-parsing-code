#include "smparser.h"
#include "smparser_SimfileLoader.h"
#include "NoteData.h"
#include <filesystem>
#include <iostream>
#include "BackgroundUtil.h"

template <typename T, typename Lambda>
void print_vector(const std::vector<T> &vec, Lambda transform) {
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if (it != vec.begin()) 
            std::cout << ',';
        std::cout << transform(*it);
    }
}

template <typename T>
void print_vector(const std::vector<T> &vec) {
    print_vector(vec, [](const T& obj) { return obj; });
}

void print_displaybpms(DisplayBPM type, const DisplayBpms &bpms) {
    std::cout << DisplayBPMToString(type);
    switch (type) {
        case DISPLAY_BPM_ACTUAL:
        case DISPLAY_BPM_SPECIFIED:
            std::cout << " (" << bpms.GetMin() << "-" << bpms.GetMax() << " BPM)";  
        case DISPLAY_BPM_RANDOM:
            break;
    }
}

void print_steps(const Song &song, Steps *steps) {
    NoteData data;
    steps->GetNoteData(data);
    std::cout
        << steps->m_StepsTypeStr << " "
        << DifficultyToString(steps->GetDifficulty()) << " "
        << steps->GetMeter()
        << " (" << steps->GetChartKey() << ")"
        << std::endl;
        
    std::cout << '\t' << "Credit: " << steps->GetCredit() << std::endl;
    std::cout << '\t' << "Description: " << steps->GetDescription() << std::endl;
    std::cout << '\t' << "Chart Name: " << steps->GetChartName() << std::endl;
    std::cout << '\t' << "Chart Style: " << steps->GetChartStyle() << std::endl;

    // composite note data?
    // couples charts?
    std::cout << '\t' << "Radar Values (Player 1): " << steps->GetRadarValues(PlayerNumber::PLAYER_1).ToString() << std::endl;

    std::cout << '\t' << "Filename: " << steps->GetFilename() << std::endl;
    std::cout << '\t' << "Music Path: " << steps->GetMusicPath() << std::endl;
    std::cout << '\t' << "Music File: " << steps->GetMusicFile() << std::endl;

    std::cout << '\t' << "Display BPM: ";
    DisplayBpms bpms;
    steps->GetDisplayBpms(bpms);
    print_displaybpms(steps->GetDisplayBPM(), bpms);
    std::cout << std::endl;

    std::cout << '\t' << "Attacks: " << steps->GetAttackString() << std::endl;
    std::cout << '\t' << "Timing Data: " << steps->GetTimingData() << std::endl;
}

void print_song(const Song &s, bool long_paths = false) {
    const std::string FIFTY_DASHES(50, '-');

    auto if_long_paths = [&](const std::string &path) {
        if (long_paths) {
            return " (full path: " + path + ")";
        } else {
            return std::string();
        }
    };

    std::cout << "Directory: " << s.GetSongDir() << std::endl;
    std::cout << "File path: " << s.GetSongFilePath() << std::endl;
    std::cout << FIFTY_DASHES << std::endl;
    std::cout << "Simfile Version: " << s.m_fVersion << std::endl;
    std::cout << "Title: " << s.GetMainTitle() << " (translit: " << s.GetTranslitMainTitle() << ")" << std::endl;
    std::cout << "Subtitle: " << s.m_sSubTitle << " (translit: " << s.GetTranslitSubTitle() << ")" << std::endl;
    std::cout << "Artist: " << s.m_sArtist << " (translit: " << s.GetTranslitArtist() << ")" << std::endl;
    std::cout << "Genre: " << s.m_sGenre << std::endl;
    std::cout << "Origin: " << s.m_sOrigin << std::endl;
    std::cout << "Credit: " << s.m_sCredit << std::endl;
    std::cout << FIFTY_DASHES << std::endl;
    std::cout << "Banner: " << s.m_sBannerFile << if_long_paths(s.GetBannerPath()) << std::endl;
    std::cout << "Background: " << s.m_sBackgroundFile << if_long_paths(s.GetBackgroundPath()) << std::endl;
    std::cout << "Jacket: " << s.m_sJacketFile << if_long_paths(s.GetJacketPath()) << std::endl;
    std::cout << "Disc Image: " << s.m_sDiscFile << if_long_paths(s.GetDiscPath()) << std::endl;
    std::cout << "Cd Image: " << s.m_sCDFile << if_long_paths(s.GetCDImagePath()) << std::endl;
    std::cout << "Cd Title: " << s.m_sCDTitleFile << if_long_paths(s.GetCDTitlePath()) << std::endl;
    std::cout << "Preview Vid: " << s.m_sPreviewVidFile << if_long_paths(s.GetPreviewVidPath()) << std::endl;
    std::cout << "Preview Music: " << s.m_PreviewFile << if_long_paths(s.GetPreviewMusicPath()) << std::endl;
    std::cout << "Music: " << s.m_sMusicFile << if_long_paths(s.GetMusicPath()) << std::endl;
    std::cout << "Lyrics: " << s.m_sLyricsFile << std::endl;
    std::cout << FIFTY_DASHES << std::endl;
    std::cout << "Offset: " << s.m_SongTiming.m_fBeat0OffsetInSeconds << std::endl;
    std::cout << "Sample start: " << s.m_fMusicSampleStartSeconds << std::endl;
    std::cout << "Sample length: " << s.m_fMusicSampleLengthSeconds << std::endl;
    std::cout << FIFTY_DASHES << std::endl;

    std::cout << "Selectable: " << 
        ((s.m_SelectionDisplay == Song::SelectionDisplay::SHOW_ALWAYS) ? "Always" : "Never") << std::endl;
    
    DisplayBpms bpms;
    s.GetDisplayBpms(bpms);

    std::cout << "Display BPM: " << DisplayBPMToString(s.m_DisplayBPMType);
    switch (s.m_DisplayBPMType) {
        case DISPLAY_BPM_ACTUAL:
        case DISPLAY_BPM_SPECIFIED:
            std::cout << " (" << bpms.GetMin() << "-" << bpms.GetMax() << " BPM)";  
        case DISPLAY_BPM_RANDOM:
            break;
    }
    std::cout << std::endl;
    std::cout << FIFTY_DASHES << std::endl;

    const TimingData &timing = s.m_SongTiming;
    FOREACH_ENUM( TimingSegmentType, tst ) {
        auto segments = timing.GetTimingSegments(tst);
        std::cout << TimingSegmentTypeToString(tst) << ": ";
        print_vector(segments, [](TimingSegment *ts) { return ts->ToString(3); });
        std::cout << std::endl;
    }
    std::cout << FIFTY_DASHES << std::endl;

    // bgchanges, fgchanges
    std::cout << "BGCHANGES: ";
    print_vector(s.GetBackgroundChanges(BACKGROUND_LAYER_1), [](const BackgroundChange &ch) {
        return ch.ToString();
    });
    std::cout << std::endl;
    
    std::cout << "BGCHANGES2: ";
    print_vector(s.GetBackgroundChanges(BACKGROUND_LAYER_2), [](const BackgroundChange &ch) {
        return ch.ToString();
    });
    std::cout << std::endl;

    std::cout << "FGCHANGES: ";
    print_vector(s.GetForegroundChanges(), [](const BackgroundChange &ch) {
        return ch.ToString();
    });
    std::cout << std::endl;

    // keysounds
    std::cout << "KEYSOUNDS: ";
    print_vector(s.m_vsKeysoundFile);
    std::cout << std::endl;

    // attacks
    std::cout << "ATTACKS: ";
    print_vector(s.m_Attacks.ToVectorString());
    std::cout << std::endl;
    std::cout << FIFTY_DASHES << std::endl;

    // dance-single Beginner 23 (xx steps)
    for (Steps *steps : s.GetAllSteps()) {
        print_steps(s, steps);
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
        std::cout << "Mostly an example script of where to find everything in the library." << std::endl;
        std::cout << "Usage: " << argv[0] << " [file path]" << std::endl;
        return 0;
    }
    
    SMParserLibrary smparserlibrary;
    dump_simfile(argv[1]);
    return 0;
}