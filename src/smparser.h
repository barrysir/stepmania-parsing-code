// this is a custom file for managing the global variables/pointers used by the Stepmania code
// Initialize the class RAII style

#pragma once

#include <memory>

class GameManager;
class RageLog;
class GameState;

class SMParserLibrary {
private:
    static bool onetime;
    static std::shared_ptr<GameManager> m_GAMEMAN;
    static std::shared_ptr<RageLog> m_LOG;
    static std::shared_ptr<GameState> m_GAMESTATE;

    void InitializeOneTime();
    void Initialize();

public:
    SMParserLibrary();
};