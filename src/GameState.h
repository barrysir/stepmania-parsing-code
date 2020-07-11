#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "global.h"
#include "PlayerNumber.h"

class TimingData;
class Style;

class GameState
{
    /** @brief The TimingData that is used for processing certain functions. */
	TimingData * processedTiming;
public:
	/** @brief Set up the GameState with initial values. */
	GameState();
	~GameState();

    /**
	 * @brief Retrieve the present timing data being processed.
	 * @return the timing data pointer. */
	TimingData * GetProcessedTimingData() const;

	const Style* GetCurrentStyle(PlayerNumber pn) const;

	/**
	 * @brief Set the timing data to be used with processing.
	 * @param t the timing data. */
	void SetProcessedTimingData(TimingData * t);

	// Autogen stuff.  This should probably be moved to its own singleton or
	// something when autogen is generalized and more customizable. -Kyz
	float GetAutoGenFarg(size_t i)
	{
		if(i >= m_autogen_fargs.size()) { return 0.0f; }
		return m_autogen_fargs[i];
	}
	vector<float> m_autogen_fargs;
	
	// This is set to a random number per-game/round; it can be used for a random seed.
	int			m_iGameSeed = 0, m_iStageSeed = 0;
};

extern GameState*	GAMESTATE;	// global and accessible from anywhere in our program

#endif