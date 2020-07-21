#include "smparser.h"
#include "GameManager.h"
#include "RageLog.h"
#include "GameState.h"
#include "ActorUtil.h"

bool SMParserLibrary::onetime = false;
std::shared_ptr<GameManager> SMParserLibrary::m_GAMEMAN(nullptr, [](GameManager *p) { delete p; GAMEMAN = nullptr; });
std::shared_ptr<RageLog> SMParserLibrary::m_LOG(nullptr, [](RageLog *p) { delete p; LOG = nullptr; });
std::shared_ptr<GameState> SMParserLibrary::m_GAMESTATE(nullptr, [](GameState *p) { delete p; GAMESTATE = nullptr; });

void SMParserLibrary::InitializeOneTime() {
    ActorUtil::InitFileTypeLists(); 
}

SMParserLibrary::SMParserLibrary() {
    if (onetime == false) {
        onetime = true;
        InitializeOneTime();
    }
    Initialize();
}

void SMParserLibrary::Initialize() {
    // is this the correct way of checking if the shared pointer points to something valid?
    if (!m_GAMEMAN) {
        m_GAMEMAN = std::make_shared<GameManager>();
        m_LOG = std::make_shared<RageLog>();
        m_GAMESTATE = std::make_shared<GameState>();

        GAMEMAN = m_GAMEMAN.get();
        LOG = m_LOG.get();
        GAMESTATE = m_GAMESTATE.get();
    }
    // the pointers are deleted in the shared_ptr deleters
}