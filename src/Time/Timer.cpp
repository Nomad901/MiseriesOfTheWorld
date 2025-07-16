#include "Timer.h"

void Timer::startTimer()
{
	std::lock_guard<std::mutex> lg(mMtx);
	mRunning = true;
	mStartTime = std::chrono::high_resolution_clock::now();
}

void Timer::startTimerFromEnd(uint32_t pEnd)
{
	std::lock_guard<std::mutex> lg(mMtx);
	mRunning = true;
	mEndTime = std::chrono::high_resolution_clock::now();
}

void Timer::stopTimer()
{
	std::lock_guard<std::mutex> lg(mMtx);
	mRunning = false;
	mEndTime = std::chrono::high_resolution_clock::now();
}

void Timer::resetTimer()
{
	mStartTime = std::chrono::high_resolution_clock::now();
}

void Timer::setProperFPS(const Uint32 pDelay)
{
	float elapsedTime = getDeltaTime(false);
	stopTimer();
	if (elapsedTime < pDelay)
		SDL_Delay(static_cast<Uint32>(pDelay - elapsedTime));
	else
		SDL_Delay(pDelay);
}

void Timer::setLimit(const float pLimit)
{
	mLimit = pLimit;
}

void Timer::setDimensionOfTime(const Dimension pType)
{
	mDimension = pType;
}

bool Timer::isRunning() const
{
	return mRunning;
}

bool Timer::isLimit()
{
#ifdef _DEBUG
	std::cout << "Delta time (from isLimit()): "<<getDeltaTime(false) << '\n';
#endif // _DEBUG

	if (getDeltaTime(false) >= mLimit || getDeltaTime(true) <= mLimit)
		return true;
	return false;
}

float Timer::getLimit()
{
	return mLimit;
}

float Timer::getDeltaTime(bool pFromEnd)
{
	using namespace std::chrono;
	
	if (!pFromEnd)
	{
		if (mRunning)
		{
			mEndTime = std::chrono::high_resolution_clock::now();
			if (mDimension == Dimension::MILISECONDS)
				return static_cast<float>(duration_cast<milliseconds>(mEndTime - mStartTime).count());
			else if (mDimension == Dimension::SECONDS)
				return static_cast<float>(duration_cast<seconds>(mEndTime - mStartTime).count());
			else if (mDimension == Dimension::MINUTES)
				return static_cast<float>(duration_cast<minutes>(mEndTime - mStartTime).count());
			else
				return 0.0f;
		}
		else
		{
			LOG("Timer is not running, i cant compute the delta time!");
			return 0.0f;
		}
	}
	else
	{
		if (mRunning)
		{
			mStartTime = std::chrono::high_resolution_clock::now();
			if (mDimension == Dimension::MILISECONDS)
				return static_cast<float>(duration_cast<milliseconds>(mEndTime - mStartTime).count());
			else if (mDimension == Dimension::SECONDS)
				return static_cast<float>(duration_cast<seconds>(mEndTime - mStartTime).count());
			else if (mDimension == Dimension::MINUTES)
				return static_cast<float>(duration_cast<minutes>(mEndTime - mStartTime).count());
			else
				return 0.0f;
		}
		else
		{
			LOG("Timer is not running, i cant compute the delta time!");
			return 0.0f;
		}
	}
	return 0.0f;
}

std::chrono::time_point<std::chrono::high_resolution_clock> Timer::getCurrentTimeOfTimer() const
{
	if (mRunning)
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> time = std::chrono::high_resolution_clock::now();
		if (mDimension == Dimension::SECONDS)
			return std::chrono::time_point_cast<std::chrono::seconds>(time);
		else if (mDimension == Dimension::MINUTES)
			return std::chrono::time_point_cast<std::chrono::minutes>(time);
		else
			return std::chrono::time_point_cast<std::chrono::milliseconds>(time);
	}
	else
	{
		LOG("Timer isnt running!");
		return std::chrono::high_resolution_clock::now();
	}
}
