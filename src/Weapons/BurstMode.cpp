#include "BurstMode.h"

BurstMode::BurstMode(SDL_Renderer* pRenderer)
{
	mTimer.setDimensionOfTime(Dimension::MILISECONDS);
	mBurstTimer.setDimensionOfTime(Dimension::MILISECONDS);
	FactoryOfFonts::getInstance().appendNewFonts(pRenderer, getCurrentPath() / "Assets" / "photos and ttf" / "Arial.ttf",
												{ {"Ready!"}, {"Waiting..."} }, { 255,255,255,255 }, 25);
}

void BurstMode::shootChargedBullets(SDL_Rect pCharRect, SDL_Rect pWeaponRect)
{
	if (manageDelay())
		return;
	mRotateMachine.calculateRadians(getFireStat().mPos, InputManager::getInstance().getMousePos());
	int32_t tmpCounter = 0;
	for (size_t i = 0; i < mCurrentQuantityBullets; i++)
	{
		mBulletsPool->makeBulletActive();
		mBulletsPool->manageLastBulletInside()->setSpeed(static_cast<float>(getFireStat().mSpeed));
		Vector2f tmpPos = { static_cast<float>(pCharRect.x), static_cast<float>(pCharRect.y) };
		mBulletsPool->manageLastBulletInside()->shootBullet({ tmpPos.mX + getFireStat().mW - 40, tmpPos.mY + tmpCounter }, InputManager::getInstance().getMousePos(),
															pWeaponRect, mRotateMachine.getAngle(), { static_cast<float>(pWeaponRect.w / 2 - 10),-5 },
															pCharRect.w, pCharRect.h);
		tmpCounter += mMultiplier;
	}
	if (getFireStat().mQuantityBullets - 1 <= mMeasureBreakingWeapon)
	{
		setShouldBreakWeapon(true);
		subtractionBullets();
		getFireStat().mQuantityBullets -= 1;
	}
	mCurrentQuantityBullets = 0;
	mTimer.startTimer();
}

void BurstMode::shoot(SDL_Rect pCharRect, SDL_Rect pWeaponRect, bool pWasReload)
{
	manageReload(pWasReload);

	if (manageDelay())
		return;

	mShowReady = false;
	mCharRect = pCharRect;
	if (getFireStat().mQuantitySets != 0 && getFireStat().mQuantityBullets != 0)
	{
		if (mSparing && mCurrentQuantityBullets < mQuantityBulletsPerTime)
		{
			if (!managingBurst())
			{
				if (mCurrentQuantityBullets < mQuantityBulletsPerTime)
				{
					mCurrentQuantityBullets++;
					setQuantityBullets(getFireStat().mQuantityBullets - 1);
				}
			}
			if (!mBurstTimer.isRunning())
				mBurstTimer.startTimer();
		}
	}
}

void BurstMode::update(SDL_Renderer* pRenderer, const Vector2f& pPos)
{
	if (mBulletsPool->getSizeBullets() <= 0)
	{
		mBulletsPool->init(getFireStat().mCapacity, pRenderer, getFireStat().mPath, pPos,
			getFireStat().mW, getFireStat().mH, getFireStat().mSpeed);
	}
	FireMode::getFireStat().mPos = pPos;

	mAnimatedStateMachine.getState("ChargingAnim").value().get().setPosition({ pPos.mX + mCharRect.w,
																			   pPos.mY - mCharRect.h / 2.0f });
	if (mShowReady)
	{
		FactoryOfFonts::getInstance().render("Ready!", pRenderer, { static_cast<int>(pPos.mX),
																	static_cast<int>(pPos.mY - mCharRect.h / 2),
																	50,50 });
	}
	if (mTimer.isRunning())
	{
		FactoryOfFonts::getInstance().render("Waiting...", pRenderer, { static_cast<int>(pPos.mX),
																		static_cast<int>(pPos.mY - mCharRect.h / 2),
																		80,50 });
	}
}

void BurstMode::render()
{
	mBulletsPool->update();

	if (manageDelay())
		return;

	mAnimatedStateMachine.setCurrentState("ChargingAnim");
	auto& tmpAnimState = mAnimatedStateMachine.getState().value().get();
	if (mSparing && getFireStat().mQuantityBullets > 0)
	{
		tmpAnimState.setActive(true);
		if (tmpAnimState.isEnded())
		{
			tmpAnimState.setActive(false);
			tmpAnimState.enableWaitAnim(true);
			tmpAnimState.runAnimation();
		}
		if (!tmpAnimState.isAnimating())
		{
			tmpAnimState.enableWaitAnim(false);
			tmpAnimState.runAnimationOnlyOnce();
		}
		mAnimatedStateMachine.render("ChargingAnim", false);
	}
	else
	{
		tmpAnimState.nullTicks();
		tmpAnimState.setActive(false);
		tmpAnimState.enableWaitAnim(false);
	}
}

void BurstMode::manageAnim(SDL_Renderer* pRenderer, const Vector2f pWeaponPos)
{
	mAnimatedStateMachine.init(pRenderer);
	mAnimatedStateMachine.pushStateW("ChargingAnim", TypeWait::GENERAL, 
									  getCurrentPath() / "Assets" / "photos and ttf" / "BurstCharging.png",
									  { pWeaponPos.mX + 20.0f, pWeaponPos.mY - 20.0f },
									  50, 50, HorVer::HORIZONTAL, { {SideOfChar::UP, {0,1,2,3,4,5,6,7} } },
									  0, static_cast<float>(mDelayStoraging));
	mAnimatedStateMachine.pushStateW("ChargingAnim", TypeWait::WAIT, 
									  getCurrentPath() / "Assets" / "photos and ttf" / "BurstDone.png",
									  { pWeaponPos.mX + 20.0f, pWeaponPos.mY - 20.0f },
									  50, 50, HorVer::HORIZONTAL, { {SideOfChar::UP, {0,1,2,3,4} } },
									  0, static_cast<float>(mDelayStoraging));
	mAnimatedStateMachine.getState("ChargingAnim").value().get().waitWithAnim(true);
}

void BurstMode::setAsSpecial()
{
	setDelay(500);
	setQuantityBullets(60);
	setOrigQuantityBullets(60);
	setQuantitySets(6);
	mDelayStoraging = 500;
	mQuantityBulletsPerTime = 8;
	mMeasureBreakingWeapon = getFireStat().mOriginalQuantityBullets - mQuantityBulletsPerTime;
}

void BurstMode::setQuantityBulletsPerTime(int32_t pQuantityBullets)
{
	mQuantityBulletsPerTime = std::clamp(pQuantityBullets, 0, 8);
	mMeasureBreakingWeapon = getFireStat().mOriginalQuantityBullets - mQuantityBulletsPerTime;
}

void BurstMode::setRangeOfSpread(int32_t pRange)
{
	mMultiplier = pRange;
}

void BurstMode::setDelayOfStoraging(int32_t pDelay)
{
	mDelayStoraging = pDelay;
}

void BurstMode::setSparing(bool pSparing)
{
	mSparing = pSparing;
}

bool BurstMode::isSparing() const noexcept
{
	return mSparing;
}

int32_t BurstMode::getItensisty() const noexcept
{
	return mDelayStoraging;
}

bool BurstMode::manageDelay()
{
	if (mTimer.isRunning())
	{
		auto delta = mTimer.getDeltaTime(false);
		
		if (delta < 3000)
			return true;

		mShowReady = true;
		mTimer.stopTimer();
	}
	return false;
}

bool BurstMode::managingBurst()
{
	if (mBurstTimer.isRunning())
	{
		auto delta = mBurstTimer.getDeltaTime(false);

		if (delta < mDelayStoraging)
			return true;

		mBurstTimer.stopTimer();
	}
	return false;
}

void BurstMode::manageReload(bool pWasReload)
{
	if (pWasReload)
	{
		setQuantityBullets(getFireStat().mOriginalQuantityBullets);
		mMeasureBreakingWeapon = getFireStat().mOriginalQuantityBullets;
		subtractionBullets();
		setQuantitySets(getFireStat().mQuantitySets - 1);
	}
}

void BurstMode::subtractionBullets()
{
	mMeasureBreakingWeapon = getFireStat().mQuantityBullets - mQuantityBulletsPerTime;
}
