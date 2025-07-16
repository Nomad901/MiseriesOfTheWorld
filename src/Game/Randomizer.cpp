#include "Randomizer.h"

Randomizer::Randomizer(int32_t pBeginning, int32_t pEnd)
	: mBeginning(pBeginning),
	  mEnd(pEnd),
	  mDice(mBeginning, mEnd)
{
	mNumber = 0;
	mPrevNumber = 0;
}

void Randomizer::generateNew()
{
	mPrevNumber = mNumber; 
	mNumber = mDice(mGenerator);
}

void Randomizer::setBeginning(int32_t pBeginning)
{
	mBeginning = pBeginning;
	mDice = std::uniform_int_distribution<int>(pBeginning, mEnd);
}

void Randomizer::setEnd(int32_t pEnd)
{
	mEnd = pEnd;
	mDice = std::uniform_int_distribution<int>(mBeginning, pEnd);
}

void Randomizer::setBeginAndEnd(int32_t pBeginning, int32_t pEnd)
{
	mBeginning = pBeginning;
	mEnd = pEnd;
	mDice = std::uniform_int_distribution<int>(pBeginning, pEnd);
}

int Randomizer::getNumber()
{
	while (mPrevNumber == mNumber)
	{
		generateNew();
	}
	return mNumber;
}

int Randomizer::getPrevNumber() const noexcept
{
	return mPrevNumber;
}