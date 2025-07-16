#pragma once
#include <mutex>
#include <chrono>

#include "SDL/SDL.h"
#include "Loger/Loging.h"

enum class Dimension{MILISECONDS, SECONDS, MINUTES};

class Timer
{
public:
	Timer() = default;
	
	void startTimer();
	void startTimerFromEnd(uint32_t pEnd);
	void stopTimer();
	void resetTimer();
	
	void setProperFPS(const Uint32 pDelay);
	void setLimit(const float pLimit);
	// MILISECONDS, SECONDS, MINUTES
	void setDimensionOfTime(const Dimension pType);

	bool isRunning() const;
	bool isLimit();

	float getLimit();
	float getDeltaTime(bool pFromEnd);
	std::chrono::time_point<std::chrono::high_resolution_clock> getCurrentTimeOfTimer() const;

private:
	bool mRunning { false };
	bool mTimerFromEnd{ false };
	float mLimit{};

	Dimension mDimension{ Dimension::MILISECONDS };
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime, mEndTime;
	
	std::mutex mMtx;
	
};

