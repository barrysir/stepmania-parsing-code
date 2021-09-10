# stepmania-parsing-code

The bits of Stepmania's source code related to simfile parsing. Enough wiring is hooked up to use it as a fully-functional Stepmania file parser (though I'm not updating it anymore.)

This was an attempt to use Stepmania's real source code to make a 100% accurate simfile parsing library, but it's probably better to build one from scratch and copy paste relevant bits of code over, so this project is defunct. Still maybe interesting to look at.

I'm not a C++ programmer, I've probably made some weird choices while making this, just giving a heads-up.

Some way too extensive documentation I've put together on the SM file format is found in the [wiki](../../wiki).

## Feature list
 * SM, SSC, DWI file parsing/saving
 * JSON parsing/saving (note: buggy)
 * Song directory parsing (chooses which simfile to load from the directory)
 * .lrc parsing
 * Radar value calculation
 * Functions for editing simfiles

## Requirements
 * C++17 (mostly because of `std::filesystem`)
 * CMake (3.15 or higher)

## Compiling

If you know CMake you probably know what to do better than me. Intended to be built out-of-source.
```
cmake .. -G (generator type) -DCMAKE_BUILD_TYPE=(Release, Debug, whatever)
```
Stepmania CMake scripts support MSVC, APPLE, and UNIX.

More detailed info of how I compile it below.

## Usage

CMake will generate a static library `libsmparser.a` for linking. Header files are found in the `src/` directory (this mimics Stepmania's codebase style). You can call the Stepmania code directly, or included in the library are some helper functions I've written, found under the `smparser` prefixed files.
  * `smparser.h`: contains a RAII style class to initialize all the Stepmania global variables
  * `smparser_SimfileLoader.h`: helper class to provide a clean interface for loading/saving simfiles
  
Check the scripts under `examples/` for sample usage.

```c++
#include <iostream>
#include "smparser.h"
#include "smparser_SimfileLoader.h"

int main() {
  SMParserLibrary smparserlibrary;  // initialize global variables
  SimfileLoader loader;
  Song s;
  bool success = loader.LoadFromFile("simfile.sm", s);
  if (success) {
    std::cout << s.GetMainTitle() << std::endl;
  }
  return 0;
}
```

## Quirks

### Global variables
Stepmania has a few global variables that have to be initialized before using the library. You can use the `SMParserLibrary` class found under `smparser.h` to manage these.

### File path format
If you're calling the Stepmania code directly, there are a couple of gotchas to the filepath format:
 * All paths to the parsing library must be given in **forward slashes**. (`path/to/file.txt`, not `path\to\file.txt`)
 * Paths pointing to a directory must have a **trailing slash**. (`path/to/folder/` not `path/to/folder`)
 * Additionally, **song paths** must not be relative, but audio files/images/etc. are ok to be relative.

You don't have to worry about this with `SimfileLoader`, it will clean paths for you, or you can use `SimfileLoader::CleanPath` yourself.

### TidyUpData
Songs have a TidyUpData() function which
 * Cleans filepaths, title/artist fields, ...
 * Looks through the files in the song folder and attempts to fill in image/audio tags, if the fields are blank
 * Calculates the music length by reading the audio file (this is used in radar calculations)
 * Calculates radar values

This is a potentially costly operation you may not want to do. `SimfileLoader::Load` or `SimfileLoader::LoadFromDir` will call TidyUpData by default, but you can turn this off using the `tidyupdata` boolean (at the cost of some parsing accuracy).

### Radar value calculation
**Stepmania does not parse radar values**, instead it recalculates them (when TidyUpData is called). At the moment there is no way to get the parser to read the radar values, but in the future I might hack in some code to parse them and a flag to enable/disable it.

### `NoteData` and `GAMESTATE->GetProcessedTimingData`
**`GAMESTATE->SetProcessedTimingData` should be called before using a `NoteData` object, or you may get pointer shenanigans**. This is because some `Notedata` functions access a `TimingData` instance through the `GAMESTATE->GetProcessedTimingData` pointer. I can't see an easy workaround for this, so unfortunately that's how it is for now.

(It's a common pattern throughout the Stepmania codebase to set `GAMESTATE->SetProcessedTimingData` before working with `NoteData`.)
```c++
void fiddleWithNoteData(Steps *s) {
  NoteData nd = s->GetNoteData();
  // nd.GetNumTapNotes()                        // this will segfault because the timing data was not set
  GAMESTATE->SetProcessedTimingData(s->getTimingData());
  int numNotes = nd.GetNumTapNotes();           // now it's safe
  /* ... do stuff with nd ... */
  GAMESTATE->SetProcessedTimingData(nullptr);   // set back to null for courtesy
}
```

## Todo

 * headers have `using namespace std;` pollution, might fix later
 * sort the CMake source lists
 * finish the dumper script
 * write documentation
 * ~~(SimfileLoader) set Song::m_sSongDir when loading from file paths~~
 * add parsing radar values
 * ~~add calculating radar values (need to get the length of the audio file... maybe could deduce length from radar values?)~~
 * ~~fix NoteData::GetNumTapNotes and related functions (uses GAMESTATE->GetProcessedTimingData)~~ 
 * add the rest of the SM file formats
 * better error messages
 * add parsing options to SimfileLoader
 * add std::path support to SimfileLoader
 * ~~lyric file parsing~~
 * optimize the executable size?
 * don't output RageLog messages to std::cout, or have it toggleable
 * add XML
 * add CachedObject
 * add image loading (for TidyUpData)

### Adding a new file type

 * directory parsing: change NotesLoader.cpp
 * simfile parsing: change SimfileLoader::Load
 * simfile saving: change SimfileLoader::Save
 * change SimfileLoader::GetFileLoadedFromDir

## Changelist
I'm starting to notate where I've modified the original code, Ctrl-F `barry edit` to find stuff I've changed. It can get annoyingly spammy in places but I can't think of a better way and I'd rather have the comments than not.

(incomplete) List of things that have been changed:
 * Most Lua binding code has been commented out (for now)
   * Described in the details for other files
 * `RageLog.h`, `RageLog.cpp`:
   * code has been gutted
   * any log messages are written to stderr (for debug purposes)
 * `global.h`:
   * Most of the contents have been commented out, and I've been uncommenting stuff which is needed in the parsing code
   * the `FAIL_M` macro has been changed to throw an exception rather than calling `sm_crash`
   * `ShowWarningOrTrace()` only prints to `stderr`, does not log to a file
   * some defines which live in `config.hpp` (which I guess is a CMake generated file?) such as `PRINTF(a,b), CONST_FUNCTION` are placed in `global.h` and have been redefined (mostly to be blank)
 * `NoteData.h`, `NoteData.cpp`:
   * code relating to `XmlFile`, `XNode` has been commented out
   * ~~a `TimingData*` member variable has been added to remove the dependency on `GAMESTATE->GetProcessedTimingData()`~~ GAMESTATE has been added, implementation has been reverted to the original (though I should change this at some point)
     * (todo: test `NoteData::GetNumRowsWithTap`, `NoteData::GetNumRowsWithTapOrHoldHead` because it has some funny macro stuff)
 * `TimingData.cpp`
   * `TimingData::GetBeatAndBPSFromElapsedTime` and `TimingData::GetElapsedTimeFromBeat` have code commented out relating to `GlobalOffsetSeconds` which the parser doesn't need to worry about
 * `RadarValues.h`, `RadarValues.cpp`:
   * Lua code has been commented out
   * code relating to `XmlFile`, `XNode` has been commented out
   * Stuff relating to `ThemeMetric` has been commented out
 * `Steps.h`, `Steps.cpp`:
   * Lua code has been commented out
   * `CachedObject<Steps>` has been commented out
   * unused include `#include "Grade.h"` has been commented out
   * implementation of `Steps::GetNoteDataFromSimfile` has been removed until all the parsers have been implemented
 * `GameState.h`, `GameState.cpp`:
   * very minimal implementation of the class
   * (will probably remove all dependencies on this class in the finished version of this parser)
 * `NoteDataUtil.h`, `NoteDataUtil.cpp`:
   * `NoteDataUtil::TransformNoteData()` functions have been commented out due to dependency on `PlayerOptions`
   * `NoteDataUtil::AddTapAttacks()` has been commented out until `Song` is implemented
 * `Style.h`, `Style.cpp`:
   * Stuff relating to `GameInput.h` has been commented out
   * Lua code has been commented out
 * `CryptManager.h`, `CryptManager.cpp`
   * commented out everything except `CryptManager::GetMD5ForString`, `CryptManager::GetSHA1ForString`
 * `Song.h`, `Song.cpp`:
   * commented functions: ReloadFromSongDir, HasAutosaveFile, LoadAutosaveFile
   * commented stuff relating to ImageDir -- some cache-y performance stuff, not relevant to us
   * calls to FILEMAN->GetDirListingWithMultipleExtensions have been replaced with GetDirListingWithMultipleExtensions
   * added getters for the Steps* vectors
   * `ReCalculateRadarValuesAndLastSecond(from_cache, true);` -> `ReCalculateRadarValuesAndLastSecond(from_cache, false);` because false wipes the notedata
 * `SongUtil.h`, `SongUtil.cpp`:
   * Only a few functions available
 * the RageFile library has been heavily stripped down
   * RageFileDriverStd is custom code which uses the standard library for file I/O
   * RageFileBasic is unmodified
   * RageFileDriver has a couple of functions and includes commented out
   * RageFile has been changed to call RageFileDriverStd directly, rather than using the FILEMAN pointer
   * RageFile has LUA code commented out
   * RageFileManager only contains some enums and the implementations for the filesystem functions in RageUtil
   * added global function GetDirListingWithMultipleExtensions to RageFileManager (usually accessed by FILEMAN->GetDirListingWithMultipleExtensions)
 * NotesWriterSM:
   * `ThemeMetric<bool> USE_CREDIT` has been replaced with a plain bool and set to `false`. Maybe let this value be customizable in the future?
   * `NotesWriterSM::WriteEditFileToMachine()` removed
 * ActorUtil:
   * only left code related to GetTypeExtensionList()
 * NotesLoader:
   * limited number of filetypes currently supported
 * LyricsLoader:
   * removed dependency on theme to get the default colour, default colour can be passed in as an argument to the lyric parser
 * RageSoundReader_MP3:
   * include path changed from "mad.h" to "../extern/mad-0.15.1b/mad.h"
 * RageSoundReader_Vorbis:
   * add include to RageException
 * GameInput:
   * commented out everything except the GameButton, GameController enums and a couple of functions
 * RageUtil_CharConversions:
   * removed an include to `#include "archutils/Win32/ErrorStrings.h"`, taking the needed function and putting it inside CharConversions
   * I don't even know if this works TBH. It compiles and executes at least.
 * NotesLoaderDWI:
   * coded `DWILoader::LoadFromSimfile` myself, refactored `DWILoader::LoadFromDir` to use `LoadFromSimfile`
 * JsonUtil:
   * comment out Dialog code
 * NotesLoaderJson:
   * number of tracks not correctly calculated
   * bpms/stops loaded incorrectly
 * NotesWriterJSON:
   * StepsType saved in a way the loader doesn't understand
 * PrefsManager:
   * heavily stripped down, only a few preferences available
 * StepsUtil:
   * bunch of functions commented because of reliance on SONGMAN, PROFILEMAN, UNLOCKMAN, CachedObject, ...

## Compiling - more detailed
```
# (starting at the top-level folder)
mkdir build
cd build
cmake .. -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release
make
```

 * Say your current directory is at the top-level folder of the repository.
 * Create a `build/` folder -- this isn't a necessary step, but otherwise the build files will be mixed with the source files and it's not easy to clean up
 * Change directory into the `build/` folder
 * Run `cmake .. -G (generator) -DCMAKE_BUILD_TYPE=Release`. cmake will read from `..` (which is the top-level directory since we're in `build/`) and put all the makefiles within the current directory, `build/`.
   * `(generator)` is the kind of project file/makefile you use to compile. My generator is "MSYS Makefiles" since I'm using MinGW/MSYS.
   * `CMAKE_BUILD_TYPE` is "Debug", "Release", etc. It doesn't have to specifically be Release, set it to whatever. If you're changing this you know the effects better than I do.
 * Use the generated files to compile it (I just type `make`).
 * I hope it works for you.
