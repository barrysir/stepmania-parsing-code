# smparser
 A Stepmania file parser based off the Stepmania source code

 * headers have `using namespace std;` pollution might fix later

Things that have been changed
 * Most Lua binding code has been commented out (for now)
   * `TimingData.cpp`
   * [todo: make a list of all changed files]
 * `TimingData.cpp`: `TimingData::GetBeatAndBPSFromElapsedTime` and `TimingData::GetElapsedTimeFromBeat` have code commented out relating to `GlobalOffsetSeconds` which the parser doesn't need to worry about