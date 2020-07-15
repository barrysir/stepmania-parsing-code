#include "global.h"
#include "GameState.h"
#include "TimingData.h"
#include "RageUtil.h"
#include "Style.h"

GameState*	GAMESTATE = nullptr;	// global and accessible from anywhere in our program

GameState::GameState() :
	processedTiming(nullptr),
	// m_pCurGame(				Message_CurrentGameChanged ),
	// m_pCurStyle(			Message_CurrentStyleChanged ),
	// m_PlayMode(				Message_PlayModeChanged ),
	// m_iCoins(				Message_CoinsChanged ),
	// m_sPreferredSongGroup(	Message_PreferredSongGroupChanged ),
	// m_sPreferredCourseGroup(	Message_PreferredCourseGroupChanged ),
	// m_PreferredStepsType(	Message_PreferredStepsTypeChanged ),
	// m_PreferredDifficulty(	Message_PreferredDifficultyP1Changed ),
	// m_PreferredCourseDifficulty(	Message_PreferredCourseDifficultyP1Changed ),
	// m_SortOrder(			Message_SortOrderChanged ),
	// m_pCurSong(				Message_CurrentSongChanged ),
	// m_pCurSteps(			Message_CurrentStepsP1Changed ),
	// m_pCurCourse(			Message_CurrentCourseChanged ),
	// m_pCurTrail(			Message_CurrentTrailP1Changed ),
	// m_bGameplayLeadIn(		Message_GameplayLeadInChanged ),
	// m_bDidModeChangeNoteSkin(	false ),
	// m_bIsUsingStepTiming(		true ),
	m_bInStepEditor(		false )
	// m_stEdit(				Message_EditStepsTypeChanged ),
	// m_cdEdit(				Message_EditCourseDifficultyChanged ),
	// m_pEditSourceSteps(		Message_EditSourceStepsChanged ),
	// m_stEditSource(			Message_EditSourceStepsTypeChanged ),
	// m_iEditCourseEntryIndex(	Message_EditCourseEntryIndexChanged ),
	// m_sEditLocalProfileID(		Message_EditLocalProfileIDChanged )
{
	// g_pImpl = new GameStateImpl;

	// m_pCurStyle.Set(nullptr);
	// FOREACH_PlayerNumber(rpn)
	// {
	// 	m_SeparatedStyles[rpn]= nullptr;
	// }

	// m_pCurGame.Set(nullptr);
	// m_iCoins.Set( 0 );
	// m_timeGameStarted.SetZero();
	// m_bDemonstrationOrJukebox = false;

	// m_iNumTimesThroughAttract = -1;	// initial screen will bump this up to 0
	// m_iStageSeed = m_iGameSeed = 0;

	// m_PlayMode.Set( PlayMode_Invalid ); // used by IsPlayerEnabled before the first screen
	// FOREACH_PlayerNumber( p )
	// 	m_bSideIsJoined[p] = false; // used by GetNumSidesJoined before the first screen

	// FOREACH_PlayerNumber( p )
	// {
	// 	m_pPlayerState[p] = new PlayerState;
	// 	m_pPlayerState[p]->SetPlayerNumber(p);
	// }
	// FOREACH_MultiPlayer( p )
	// {
	// 	m_pMultiPlayerState[p] = new PlayerState;
	// 	m_pMultiPlayerState[p]->SetPlayerNumber(PLAYER_1);
	// 	m_pMultiPlayerState[p]->m_mp = p;
	// }

	// m_Environment = new LuaTable;

	// m_bDopefish = false;

	// sExpandedSectionName = "";

	// // Don't reset yet; let the first screen do it, so we can use PREFSMAN and THEME.
	// //Reset();

	// // Register with Lua.
	// {
	// 	Lua *L = LUA->Get();
	// 	lua_pushstring( L, "GAMESTATE" );
	// 	this->PushSelf( L );
	// 	lua_settable( L, LUA_GLOBALSINDEX );
	// 	LUA->Release( L );
	// }
}

GameState::~GameState()
{
	// // Unregister with Lua.
	// LUA->UnsetGlobal( "GAMESTATE" );

	// FOREACH_PlayerNumber( p )
	// 	SAFE_DELETE( m_pPlayerState[p] );
	// FOREACH_MultiPlayer( p )
	// 	SAFE_DELETE( m_pMultiPlayerState[p] );

	// SAFE_DELETE( m_Environment );
	// SAFE_DELETE( g_pImpl );
	SAFE_DELETE( processedTiming );
}

TimingData * GameState::GetProcessedTimingData() const
{
	return this->processedTiming;
}

void GameState::SetProcessedTimingData(TimingData * t)
{
	this->processedTiming = t;
}


const Style* GameState::GetCurrentStyle(PlayerNumber pn) const
{
	return nullptr;
	// if(GetCurrentGame() == nullptr) { return nullptr; }
	// if(!GetCurrentGame()->m_PlayersHaveSeparateStyles)
	// {
	// 	return m_pCurStyle;
	// }
	// else
	// {
	// 	if(pn >= NUM_PLAYERS)
	// 	{
	// 		return m_SeparatedStyles[PLAYER_1] == nullptr ? m_SeparatedStyles[PLAYER_2]
	// 			: m_SeparatedStyles[PLAYER_1];
	// 	}
	// 	return m_SeparatedStyles[pn];
	// }
}