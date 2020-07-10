# smparser
 A Stepmania file parser based off the Stepmania source code

 * headers have `using namespace std;` pollution, might fix later

Things that have been changed:
 * Most Lua binding code has been commented out (for now)
   * `TimingData.cpp`
   * [todo: make a list of all changed files]
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
   * a `TimingData*` member variable has been added to remove the dependency on `GAMESTATE->GetProcessedTimingData()`
     * (todo: test `NoteData::GetNumRowsWithTap`, `NoteData::GetNumRowsWithTapOrHoldHead` because it has some funny macro stuff)
 * `TimingData.cpp`
   * `TimingData::GetBeatAndBPSFromElapsedTime` and `TimingData::GetElapsedTimeFromBeat` have code commented out relating to `GlobalOffsetSeconds` which the parser doesn't need to worry about