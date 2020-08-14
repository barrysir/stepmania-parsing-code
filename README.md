# smparser
A Stepmania file parsing/writing library built from Stepmania's source code. Emphasis on parsing accuracy and support for any parsing performed by Stepmania. Currently just the Stepmania source code made to compile standalone and some helper classes, maybe eventually it'll be a custom parser using the Stepmania code.

Check out the wiki! [link](https://github.com/barrysir/smparser/wiki)

Still WIP, not all functions work, sorry about that.

I'm no professional at C++, I've probably made some weird choices while wrangling the code in this library. If you find anything that could be improved let me know.

 * headers have `using namespace std;` pollution, might fix later
 * sort the CMake source lists
 * finish the dumper script
 * write documentation
 * (SimfileLoader) set Song::m_sSongDir when loading from file paths
 * add parsing radar values
 * add calculating radar values (need to get the length of the audio file... maybe could deduce length from radar values?)
 * fix NoteData::GetNumTapNotes and related functions (uses GAMESTATE->GetProcessedTimingData)
 * add the rest of the SM file formats
 * better error messages
 * add parsing options to SimfileLoader
 * add std::path support to SimfileLoader
 * ~~lyric file parsing~~
 * optimize the executable size?
 * don't output RageLog messages to std::cout, or have it toggleable

## Requirements
 * C++17 (`std::filesystem` mostly)
 * CMake

## Usage
CMake will generate a static library `libsmparser.a` for linking. Includes are found in the `src/` directory (this mimics Stepmania's codebase style).

Stepmania has a few global variables that have to be initialized before using the library. I've written a RAII class to manage these, `SMParserLibrary`, found under `smparser.h`. Once the variables are initialized you can call the Stepmania code yourself, or use the few helper functions I've made under the `smparser_` prefixed files (`smparser_SimfileLoader.h`).

If you're calling the Stepmania code directly, there are a couple of gotchas to the filepath format:
 * All paths to the parsing library must be given in **forward slashes**. (`path/to/file.txt`, not `path\to\file.txt`)
 * Directory paths must have a **trailing slash**. (`path/to/folder/` not `path/to/folder`)

You don't have to worry about this with `SimfileLoader`, it will clean paths for you.

Example of SMParserLibrary use:
```c++
#include "smparser.h"

int main() {
  SMParserLibrary smparserlibrary;

  // ... do stuff ...
  function();
  // ... do stuff ...

  return 0;
  // global pointers automatically cleaned up
}

void function() {
  // can initialize it multiple times, will not re-initialize the global variables
  SMParserLibrary smparserlibrary;
  // ... do stuff ...
}
```


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
   * TranslateTitles
   * commented stuff relating to ImageDir -- some cache-y performance stuff, not relevant to us
   * calls to FILEMAN->GetDirListingWithMultipleExtensions have been replaced with GetDirListingWithMultipleExtensions
   * added getters for the Steps* vectors
 * `SongUtil.h`, `SongUtil.cpp`:
   * Removed implementations for everything except the `SongUtil::GetStepsXXX` methods
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