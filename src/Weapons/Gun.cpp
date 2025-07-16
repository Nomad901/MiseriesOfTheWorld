#include "Gun.h"

bool Gun::brokenChecker()
{
	if (mCurrentRobustness <= 0)
	{
		Weapon::makeBroken(true);
		return true;
	}
	Weapon::makeBroken(false);
	return false;
}

bool Gun::manageShootState(SDL_Renderer* pRenderer)
{	
	auto& animTexture = Weapon::getAnimatedStateMachine().getState("ShootAnimWeapon").value().get();

	if (!Weapon::getWeaponStates().mShootingState) 
	{
		if (animTexture.isAnimating()) 
		{
			animTexture.setActive(false);
			animTexture.stopAnimation();
			animTexture.nullTicks();
		}
		return false;
	}

	if (!animTexture.isAnimating()) 
	{
		animTexture.setCurrentSide(SideOfChar::RIGHT);
		animTexture.setActive(true);
		animTexture.runAnimationOnlyOnce();  
	}
	Weapon::getAnimatedStateMachine().render("ShootAnimWeapon", true, mRotateMachine.getAngle());

	if (animTexture.isEnded()) 
	{
		Weapon::makeShoot(false);
		return false;
	}
	return true;
}

bool Gun::manageReloadState(SDL_Renderer* pRenderer)
{
	auto tmpAnimTexture = Weapon::getAnimatedStateMachine().getState("ReloadAnimWeapon").value();

	if (Weapon::getWeaponStates().mRealodingState &&
		!Weapon::getWeaponStates().mIsFreezed)
	{
		if (!tmpAnimTexture.get().isAnimating())
		{
			tmpAnimTexture.get().setCurrentSide(SideOfChar::RIGHT);
			tmpAnimTexture.get().setActive(true);
			if(mEndlessAnim)
				tmpAnimTexture.get().runAnimation();
			else
				tmpAnimTexture.get().runAnimationOnlyOnce();
			mReloadLogic.startReloading();
		}
		Weapon::getAnimatedStateMachine().render("ReloadAnimWeapon", true, mRotateMachine.getAngle());
		mReloadLogic.update(mFactoryObjects.convertFRect(Weapon::getCharCollisions().mChar));
		mReloadLogic.render(pRenderer);
		if (!mReloadLogic.isReloading())
		{
			Weapon::makeReload(false);
			return false;
		}
		return true;
	}
	if (tmpAnimTexture.get().isAnimating())
	{
		tmpAnimTexture.get().setActive(false);
		tmpAnimTexture.get().stopAnimation();
		tmpAnimTexture.get().nullTicks();
	}
	return false;
}

bool Gun::manageBrokenState(SDL_Renderer* pRenderer)
{
	brokenChecker();
	if (Weapon::getWeaponStates().mIsBroken)
	{
		if (!Weapon::getWeaponStates().mIsFreezed)
		{
			SDL_RenderCopyExF(pRenderer, TextureManager::getInstance().getTexture(Weapon::getTextures().mBrokenWeapon),
							  nullptr, &Weapon::getCharCollisions().mWeapon, mRotateMachine.getAngle(), nullptr,
							  InputManager::getInstance().getMousePos().mX >= Weapon::getWeaponStats().mPos.mX ?
							  SDL_FLIP_NONE : SDL_FLIP_VERTICAL);
		}
		else
		{
			SDL_RenderCopyF(pRenderer, TextureManager::getInstance().getTexture(Weapon::getTextures().mBrokenWeapon),
						    nullptr, &Weapon::getCharCollisions().mWeapon);
		}
		return true;
	}
	return false;
}

void Gun::initGun(SDL_Renderer* pRenderer, const Config::ReloadConfig& pReloadConfig, SDL_FRect pCharRect)
{
	Weapon::initWeapon(pRenderer);
	Weapon::setCharCollision(pCharRect);
	mReloadLogic.initReloadLogic(pRenderer, pCharRect, pReloadConfig.mReloadingTime, pReloadConfig.mShowReloadingQuote,
								 pReloadConfig.mColorNumbersReload, pReloadConfig.mSizeNumbersReload);
	mConfig.mReloadConfig = pReloadConfig;
}

void Gun::manageRobustness()
{
	mCurrentRobustness -= Weapon::getNumberRobustness();
}

float Gun::getCurrentRobustness() const noexcept
{
	return mCurrentRobustness;
}

float Gun::getAngleOfWeapon() const noexcept
{
	return mRotateMachine.getAngle();
}

void Gun::setReloadAnimationEndless(bool pEndlessAnim)
{
	mEndlessAnim = pEndlessAnim;
}

void Gun::setTangoIsCharging(bool pTangoIsCharging)
{
	mTangoIsCharging = pTangoIsCharging;
}

void Gun::setTangoIsWaiting(bool pTangoIsWaiting)
{
	mTangoIsWaiting = pTangoIsWaiting;
}

bool Gun::getTangoIsWaiting() const noexcept
{
	return mTangoIsWaiting;
}

bool Gun::WeaponIsInView(SDL_Rect pCharCollision)
{
	SDL_Rect tmpRect = pCharCollision;
	tmpRect.w += 50;
	tmpRect.h += 50;
	if (mObjects.areColliding(tmpRect, mFactoryObjects.convertFRect(Weapon::getCharCollisions().mWeapon)))
		return true;
	return false;
}

void Gun::render(SDL_Renderer* pRenderer)
{
	mRotateMachine.calculateDegrees(Weapon::getWeaponStats().mPos, InputManager::getInstance().getMousePos());

	if (!manageBrokenState(pRenderer) && 
		!mTangoIsCharging)
	{
		if (!manageShootState(pRenderer) && 
			!manageReloadState(pRenderer))
		{
			if (Weapon::getWeaponStates().mIsFreezed)
				SDL_RenderCopyF(pRenderer, TextureManager::getInstance().getTexture(Weapon::getTextures().mStaticPath),
							   nullptr, &Weapon::getCharCollisions().mWeapon);
			else
				SDL_RenderCopyExF(pRenderer, TextureManager::getInstance().getTexture(Weapon::getTextures().mStaticPath),
								  nullptr, &Weapon::getCharCollisions().mWeapon, mRotateMachine.getAngle(), nullptr,
								  InputManager::getInstance().getMousePos().mX >= Weapon::getWeaponStats().mPos.mX ?
								  SDL_FLIP_NONE : SDL_FLIP_VERTICAL);
		}
	}

}

void Gun::update(const Vector2f& pPos)
{
	Weapon::setPos(pPos);
	Weapon::setPosChar(pPos);
	
	manageRotateAround(pPos);

	Weapon::getAnimatedStateMachine().getState("ShootAnimWeapon").value().get().setPositionRotate(Weapon::getCharCollisions().mChar,
									  mRotateMachine.getAngle(), { static_cast<float>(Weapon::getCharCollisions().mChar.w / 2 - 10),-5 });
	Weapon::getAnimatedStateMachine().getState("ReloadAnimWeapon").value().get().setPositionRotate(Weapon::getCharCollisions().mChar,
									  mRotateMachine.getAngle(), { static_cast<float>(Weapon::getCharCollisions().mChar.w / 2 - 10),-5 });
}

void Gun::manageRotateAround(const Vector2f& pPos)
{
	mRotateMachine.calculateRadians(pPos, InputManager::getInstance().getMousePos());
	SDL_FRect tmpRect = Weapon::getCharCollisions().mWeapon;
	mRotateMachine.rotateAttachedRect(tmpRect, Weapon::getCharCollisions().mChar, mRotateMachine.getAngle(),
									{ static_cast<float>(Weapon::getCharCollisions().mChar.w / 2 - 10),-5 });
	Weapon::setPos({ tmpRect.x, tmpRect.y });
}
