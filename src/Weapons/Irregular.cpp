#include "Irregular.h"

Irregular::Irregular()
{
	mTimer.setDimensionOfTime(Dimension::MILISECONDS);
}

void Irregular::shoot(SDL_Rect pCharRect, SDL_Rect pWeaponRect, bool pWasReload)
{
	static int32_t tmpBreakingWeapon = getFireStat().mOriginalQuantityBullets - mQuantityBulletsPerTime * 2;

	manageReload(tmpBreakingWeapon, pWasReload);

	if (getFireStat().mQuantitySets != 0 && getFireStat().mQuantityBullets != 0)
	{
		if (manageDelay() || manageDelaySequence())
			return;

		mRotateMachine.calculateRadians(getFireStat().mPos, InputManager::getInstance().getMousePos());

		mBulletsPool->makeBulletActive();
		mBulletsPool->manageLastBulletInside()->setSpeed(getFireStat().mSpeed);
		Vector2f tmpPos = { static_cast<float>(pCharRect.x), static_cast<float>(pCharRect.y) };
		mBulletsPool->manageLastBulletInside()->shootBullet({ tmpPos.mX + getFireStat().mW - 40, tmpPos.mY }, InputManager::getInstance().getMousePos(),
															  pWeaponRect, mRotateMachine.getAngle(), { static_cast<float>(pWeaponRect.w / 2 - 10),-5 },
															  pCharRect.w, pCharRect.h);
		mCounterBullets++;

		if (getFireStat().mQuantityBullets <= tmpBreakingWeapon)
		{
			setShouldBreakWeapon(true);
			subtractionBullets(tmpBreakingWeapon);
		}
		setQuantityBullets(getFireStat().mQuantityBullets - 1);
		mTimer.startTimer();
	}
}

void Irregular::render()
{
	mBulletsPool->update();
}

void Irregular::update(SDL_Renderer* pRenderer, const Vector2f& pPos)
{
	if (mBulletsPool->getSizeBullets() <= 0)
	{
		mBulletsPool->init(getFireStat().mCapacity, pRenderer, getFireStat().mPath, pPos,
			getFireStat().mW, getFireStat().mH, getFireStat().mSpeed);
	}	
	if (mBulletsPool->getSizeActiveBullets() >= mQuantityBulletsPerTime)
	{
		if(!mTimer.isRunning())
			mTimer.startTimer();
	}
	FireMode::getFireStat().mPos = pPos;
}

bool Irregular::manageDelay()
{
	if (mTimer.isRunning()) {
		float delta = mTimer.getDeltaTime(false);

		if (delta < getFireStat().mDelay) {
			setDelayGoing(true);
			return true;
		}

		mTimer.stopTimer();
		setDelayGoing(false);
	}
	return false;
}

bool Irregular::manageDelaySequence()
{
	if (mCounterBullets >= mQuantityBulletsPerTime)
	{
		if (!mTimerForBurst.isRunning())
			mTimerForBurst.startTimer();
		if (mTimerForBurst.getDeltaTime(false) >= mBurstCooldown)
		{
			mTimerForBurst.stopTimer();
			mCounterBullets = 0;
			return false;
		}
		return true;
	}
	return false;
}

void Irregular::setAsSpecial()
{
	setDelay(150);
	setQuantityBullets(40);
	setOrigQuantityBullets(40);
	setQuantitySets(6);
	mBurstCooldown = 500;
	mQuantityBulletsPerTime = 3;
} 

void Irregular::setQuantityBulletsPerTime(int32_t pQuantityBullets)
{
	if (pQuantityBullets > 0 && pQuantityBullets < getFireStat().mQuantityBullets)
		mQuantityBulletsPerTime = pQuantityBullets;
}

void Irregular::setBurstCooldown(int32_t pBurstCooldown)
{
	mBurstCooldown = pBurstCooldown;
}

void Irregular::manageReload(int32_t& pTmpBreakingWeapon, bool pWasReload)
{
	if (pWasReload)
	{
		setQuantityBullets(getFireStat().mOriginalQuantityBullets);
		pTmpBreakingWeapon = getFireStat().mOriginalQuantityBullets;
		subtractionBullets(pTmpBreakingWeapon);
		setQuantitySets(getFireStat().mQuantitySets - 1);	
	}
}

void Irregular::subtractionBullets(int32_t& pTmpBreakingWeapon)
{
	if(mQuantityBulletsPerTime*2 != 0)
		pTmpBreakingWeapon -= mQuantityBulletsPerTime * 2;
	else
		pTmpBreakingWeapon -= mQuantityBulletsPerTime;
}
