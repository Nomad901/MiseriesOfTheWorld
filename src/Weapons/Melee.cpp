#include "Melee.h"

void Melee::initMelee(SDL_Renderer* pRenderer, const Config::WeaponConfig& pConfig, 
					  SDL_FRect pCharRect)
{
	mRenderer = pRenderer;
	Weapon::initWeapon(pRenderer);
	Weapon::setCharCollision(pCharRect);
	mConfig.mWeaponConfig = pConfig;
	mCurrentRobustness = Weapon::getWeaponStats().mScaleOfRobustness;
}

void Melee::setHitBoxAttack(SDL_FRect pHitBoxAttack)
{
	mConfig.mWeaponConfig.mHitBoxAttack = pHitBoxAttack;
}

SDL_FRect Melee::getHitBoxAttack() const noexcept
{
	return mConfig.mWeaponConfig.mHitBoxAttack;
}

bool Melee::manageRobustness()
{
	if (mCurrentRobustness <= 0)
	{
		Weapon::makeBroken(true);
		return true;
	}
	Weapon::makeBroken(false);
	return false;
}

void Melee::setCurrentRobustness(float pCurrentRobustness)
{
	mCurrentRobustness = pCurrentRobustness;
}

float Melee::getCurrentRobustness()
{
	return mCurrentRobustness;
}

void Melee::showHitBox(bool pShowHitbox)
{
	mShowHitBox = pShowHitbox;
}

bool Melee::WeaponIsInView(SDL_Rect pCharCollision)
{
	SDL_Rect tmpRect = pCharCollision;
	tmpRect.w += 50;
	tmpRect.h += 50;
	if (mObjects.areColliding(tmpRect, mFactoryObjects.convertFRect(Weapon::getCharCollisions().mWeapon)))
		return true;
	return false;
}

void Melee::render(SDL_Renderer* pRenderer)
{
	mRotateMachine.calculateDegrees(Weapon::getWeaponStats().mPos, InputManager::getInstance().getMousePos());

	if (!manageBrokenState() && 
		!manageShootState())
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

void Melee::update(const Vector2f& pPos)
{
	Weapon::setPos(pPos);
	Weapon::setPosChar(pPos);
	Weapon::getAnimatedStateMachine().getState("ShootAnimWeapon")->get().setPositionRotate(Weapon::getCharCollisions().mChar,
																						   mRotateMachine.getAngle() * M_PI / 180, 
																						   { static_cast<float>(Weapon::getCharCollisions().mChar.w / 2 - 10),-5 });
	//Weapon::getAnimatedStateMachine().getState("ShootAnimWeapon")->get().setPosition({ mConfig.mWeaponConfig.mHitBoxAttack.x = pPos.mX + 50,
	//																				   mConfig.mWeaponConfig.mHitBoxAttack.y = pPos.mY });
	if (Weapon::getWeaponStates().mShootingState)
	{
		mConfig.mWeaponConfig.mHitBoxAttack.x = pPos.mX+50;
		mConfig.mWeaponConfig.mHitBoxAttack.y = pPos.mY;
	}
	else
	{
		mConfig.mWeaponConfig.mHitBoxAttack.x = -500;
		mConfig.mWeaponConfig.mHitBoxAttack.y = -500;
	}

	manageRotateAround(pPos);
	rotateHitBox(Weapon::getCharCollisions().mChar);
}

void Melee::manageRotateAround(const Vector2f& pPos)
{
	mRotateMachine.calculateRadians(pPos, InputManager::getInstance().getMousePos());
	SDL_FRect tmpRect = Weapon::getCharCollisions().mWeapon;
	mRotateMachine.rotateAttachedRect(tmpRect, Weapon::getCharCollisions().mChar, mRotateMachine.getAngle(),
									  { static_cast<float>(Weapon::getCharCollisions().mChar.w / 2 - 10),-5 });
	Weapon::setPos({ tmpRect.x, tmpRect.y });
}

void Melee::rotateHitBox(SDL_FRect pBaseRect)
{
	mRotateHitBoxMachine.rotateAttachedRect(mConfig.mWeaponConfig.mHitBoxAttack, pBaseRect, mRotateMachine.getAngle(),
										   { static_cast<float>(Weapon::getCharCollisions().mChar.w / 2 - 10),-5 });
}

void Melee::subtractionRobustness()
{
	mCurrentRobustness -= Weapon::getNumberRobustness();
}

bool Melee::manageShootState()
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

	if (mShowHitBox)
	{
		SDL_SetRenderDrawColor(mRenderer, 34, 177, 76, 255);
		SDL_RenderFillRectF(mRenderer, &mConfig.mWeaponConfig.mHitBoxAttack);
		SDL_RenderCopyExF(mRenderer, nullptr, nullptr, &mConfig.mWeaponConfig.mHitBoxAttack,
						  mRotateMachine.getAngle(), nullptr, SDL_FLIP_NONE);
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

bool Melee::manageBrokenState()
{
	manageRobustness();
	if (Weapon::getWeaponStates().mIsBroken)
	{
		if (!Weapon::getWeaponStates().mIsFreezed)
		{
			SDL_RenderCopyExF(mRenderer, TextureManager::getInstance().getTexture(Weapon::getTextures().mBrokenWeapon),
							  nullptr, &Weapon::getCharCollisions().mWeapon, mRotateMachine.getAngle(), nullptr,
							  InputManager::getInstance().getMousePos().mX >= Weapon::getWeaponStats().mPos.mX ?
							  SDL_FLIP_NONE : SDL_FLIP_VERTICAL);
		}
		else
		{
			SDL_RenderCopyF(mRenderer, TextureManager::getInstance().getTexture(Weapon::getTextures().mBrokenWeapon),
							nullptr, &Weapon::getCharCollisions().mWeapon);
		}
		return true;
	}
	return false;
}
