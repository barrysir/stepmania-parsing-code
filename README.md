# smparser
The bits of the Stepmania source code related to parsing stepfiles. That makes this a 100% accurate parsing library. As a bonus it has a lot of features which Stepmania itself uses to modify stepfiles.

 * headers have `using namespace std;` pollution, might fix later

I'm starting to notate where I've modified the original code, Ctrl-F `barry edit` to find stuff I've changed. It can get annoyingly spammy in places but I can't think of a better way and I'd rather have the comments than not.

Things that have been changed:
 * Most Lua binding code has been commented out (for now)
   * Described in the details for other files
 * `RageLog.h`, `RageLog.cpp`:
   * code has been gutted to swallow all log messages (todo: change this to print to stderr?)
   * (todo: remove all the logging instances in the code, or find some way to initialize the LOG global variable automatically)
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
 * `GameState.h`, `GameState.cpp`:
   * very minimal implementation of the class
   * (will probably remove all dependencies on this class in the finished version of this parser)