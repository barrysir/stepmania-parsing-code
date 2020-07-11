#ifndef GAMESTATE_H
#define GAMESTATE_H

class TimingData;

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

	/**
	 * @brief Set the timing data to be used with processing.
	 * @param t the timing data. */
	void SetProcessedTimingData(TimingData * t);
};

extern GameState*	GAMESTATE;	// global and accessible from anywhere in our program

#endif