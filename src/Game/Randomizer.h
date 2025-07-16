#pragma once
#include <random>

class Randomizer
{
public:
	Randomizer(int32_t pBeginning, int32_t pEnd);

	void generateNew();
	void setBeginning(int32_t pBeginning);
	void setEnd(int32_t pEnd);
	void setBeginAndEnd(int32_t pBeginning, int32_t pEnd);

	int getNumber();
	int getPrevNumber() const noexcept;

private:
	int32_t mNumber, mPrevNumber;
	int32_t mBeginning, mEnd;
	std::default_random_engine mGenerator{ std::random_device{}() };
	std::uniform_int_distribution<int> mDice;

};


