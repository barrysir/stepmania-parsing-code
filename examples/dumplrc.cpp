#include "LyricsLoader.h"
#include "smparser_SimfileLoader.h"     // for path cleaning
#include "Song.h"

#include <iostream>

void print_lrc(const Song &s) {
    for (const LyricSegment &lyric : s.m_LyricSegments) {
        std::cout << '['
            << lyric.m_fStartTime << ' '
            << '(' << lyric.m_Color.ToString() << ')'
        << ']' << ' '
        << lyric.m_sLyric << std::endl;
    }
}

void test_lrcfile(const std::string &_filepath) {
    SimfileLoader sltemp;
    std::string filepath = sltemp.CleanPath(_filepath, false);
    if (filepath != _filepath) {
        std::cerr << "Note: filepath was cleaned! If you're calling this from code make sure there aren't any backslashes or anything. " << _filepath << " -> " << filepath << std::endl;
    }

    Song out;
    LyricsLoader temp;
    bool success = temp.LoadFromLRCFile(filepath, out);
    if (!success) {
        std::cerr << "Could not load lyrics from " << filepath << "!" << std::endl;
        return;
    }
    print_lrc(out);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cout << "Data dumper for the LRC file format." << std::endl;
        std::cout << "Usage: " << argv[0] << " [file path]" << std::endl;
        return 0;
    }
    
    test_lrcfile(argv[1]);
    return 0;
}