#include "SingleOrSequence.h"

void SingleOrSequence::shoot(SDL_Rect pCharRect, SDL_Rect pWeaponRect, bool pWasReload)
{
	static int32_t tmpBreakingWeapon = getFireStat().mOriginalQuantityBullets - 5;
	
	if (pWasReload)
	{
		tmpBreakingWeapon = getFireStat().mOriginalQuantityBullets-5;
		setQuantityBullets(getFireStat().mOriginalQuantityBullets);
		setQuantitySets(getFireStat().mQuantitySets - 1);
	}

	if (getFireStat().mQuantitySets != 0)
	{
		if (mTimer.isRunning())
		{
			if (manageDelay())
				return;
		}

		mRotateMachine.calculateRadians({static_cast<float>(pWeaponRect.x), static_cast<float>(pWeaponRect.y)}, InputManager::getInstance().getMousePos());

		mBulletsPool->makeBulletActive();
		mBulletsPool->manageLastBulletInside()->setSpeed(getFireStat().mSpeed);
		Vector2f tmpPos = { static_cast<float>(pCharRect.x), static_cast<float>(pCharRect.y) };
		mBulletsPool->manageLastBulletInside()->shootBullet({ tmpPos.mX + getFireStat().mW - 40, tmpPos.mY }, InputManager::getInstance().getMousePos(),
															  pWeaponRect, mRotateMachine.getAngle(), { static_cast<float>(pWeaponRect.w / 2 - 10),-5 },
															  pCharRect.w, pCharRect.h);
		if (tmpBreakingWeapon > getFireStat().mQuantityBullets)
		{
			setShouldBreakWeapon(true);
			tmpBreakingWeapon -= 5;
		}
		setQuantityBullets(getFireStat().mQuantityBullets - 1);
		mTimer.startTimer();
	}
}

void SingleOrSequence::update(SDL_Renderer* pRenderer, const Vector2f& pPos)
{
	if (mBulletsPool->getSizeBullets() <= 2)
	{
		mBulletsPool->init(getFireStat().mCapacity, pRenderer, getFireStat().mPath, pPos,
						   getFireStat().mW, getFireStat().mH, getFireStat().mSpeed);
	}
	FireMode::getFireStat().mPos = pPos;
}

void SingleOrSequence::render()
{
	mBulletsPool->update();
}

bool SingleOrSequence::manageDelay() {
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

void SingleOrSequence::setAsSpecial()
{
	if (mMode == Mode::SEQUENCE) {
		setDelay(100);
		setQuantityBullets(30);
		setOrigQuantityBullets(30);
		setQuantitySets(4);
	}
	else
	{
		setDelay(300);
		setQuantityBullets(15);
		setOrigQuantityBullets(15);
		setQuantitySets(15);
	}
}

void SingleOrSequence::setMode(Mode pMode)
{
	mMode = pMode;
}
