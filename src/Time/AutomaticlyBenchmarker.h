#pragma once
#include <iostream>
#include <chrono>
#include "Loger/Loging.h"

//when lifetime of the scope is out - time of this will be written in the console

enum class Dimension_ { MICROSECONDS,MILISECONDS, SECONDS, MINUTES,HOURS };

class AutomaticlyBenchmarker
{
public:
	AutomaticlyBenchmarker()
	{
		mStart = std::chrono::high_resolution_clock::now();
	}
	~AutomaticlyBenchmarker()
	{
		mEnd = std::chrono::high_resolution_clock::now();
		writerTime();
	}
	inline void resetTimer()
	{
		mStart = std::chrono::high_resolution_clock::now();
	}
	inline void setDimension(const Dimension_ pDimension)
	{
		mDimenson = pDimension;
	}

	inline float getTimeNow()
	{
		mEnd = std::chrono::high_resolution_clock::now();
		writerTime();
	}

private:
	void writerTime()
	{
		if (mDimenson == Dimension_::MILISECONDS)
			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(mEnd - mStart).count() << '\n';
		else if (mDimenson == Dimension_::MICROSECONDS)
			std::cout << std::chrono::duration_cast<std::chrono::microseconds>(mEnd - mStart).count() << '\n';
		else if (mDimenson == Dimension_::SECONDS)
			std::cout << std::chrono::duration_cast<std::chrono::seconds>(mEnd - mStart).count() << '\n';
		else if (mDimenson == Dimension_::MINUTES)
			std::cout << std::chrono::duration_cast<std::chrono::minutes>(mEnd - mStart).count() << '\n';
		else if (mDimenson == Dimension_::HOURS)
			std::cout << std::chrono::duration_cast<std::chrono::hours>(mEnd - mStart).count() << '\n';
	}

	std::chrono::time_point<std::chrono::steady_clock> mStart, mEnd;
	Dimension_ mDimenson{ Dimension_::MILISECONDS };
};

