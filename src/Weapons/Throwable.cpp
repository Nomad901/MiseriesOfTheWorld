#include "Throwable.h"

void Throwable::initThrowable(SDL_Renderer* pRenderer, const Config::WeaponConfig& pWeaponConfig,
						      SDL_FRect pCharRect)
{
	Weapon::initWeapon(pRenderer);
	Weapon::setCharCollision(pCharRect);
	Weapon::setPaths(pWeaponConfig.mPathBullets, std::filesystem::current_path());
	mConfig.mWeaponConfig = pWeaponConfig;
}

void Throwable::setTexture(const PATH& pPath)
{
	mConfig.mWeaponConfig.mPathBullets = pPath;
}

bool Throwable::WeaponIsInView(SDL_Rect pCharCollision)
{
	SDL_Rect tmpRect = pCharCollision;
	tmpRect.w += 50;
	tmpRect.h += 50;
	if (mObjects.areColliding(tmpRect, mFactoryObjects.convertFRect(Weapon::getCharCollisions().mWeapon)))
		return true;
	return false;
}

void Throwable::render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyExF(pRenderer, TextureManager::getInstance().getTexture(Weapon::getTextures().mStaticPath),
					  nullptr, &Weapon::getCharCollisions().mWeapon, mRotateMachine.getAngle(), nullptr, SDL_FLIP_NONE);
	
}

void Throwable::update(const Vector2f& pPos)
{
	Weapon::setPos(pPos);
	Weapon::setPosChar(pPos);

	manageRotateAround(pPos);
}

void Throwable::manageRotateAround(const Vector2f& pPos)
{
	mRotateMachine.calculateRadians(pPos, InputManager::getInstance().getMousePos());
	SDL_FRect tmpRect = Weapon::getCharCollisions().mWeapon;
	mRotateMachine.rotateAttachedRect(tmpRect, Weapon::getCharCollisions().mChar, mRotateMachine.getAngle(),
									 { static_cast<float>(Weapon::getCharCollisions().mChar.w / 2 - 10),-5 });
	Weapon::setPos({ tmpRect.x, tmpRect.y });
}