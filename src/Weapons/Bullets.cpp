#include "Bullets.h"

Bullets::Bullets(SDL_Renderer* pRenderer,
				 const PATH& pPath, const Vector2f& pPos,
				 int32_t pW, int32_t pH, float pSpeed)
	: mPos{ pPos }, mRenderer(pRenderer), 
	  mSpeed{pSpeed}, mPath{pPath}
{
	mRect = { pPos.mX,pPos.mY, static_cast<float>(pW),static_cast<float>(pH) };
	SDL_Rect tmpRect = { static_cast<int>(mRect.x),static_cast<int>(mRect.y),
						 static_cast<int>(mRect.w),static_cast<int>(mRect.h) };
	TextureManager::getInstance().appendTextureWithoutBack(pRenderer, pPath, tmpRect, { 34,177,76,255 });
}

void Bullets::shootBullet(const Vector2f& pPlayerPos, const Vector2f& pMousePos, SDL_Rect pWeaponRect,
						  double pAngle, const Vector2f& pOffset, int32_t pW, int32_t pH)
{
	Vector2f centerPos = { pPlayerPos.mX + pW / 2.0f, pPlayerPos.mY + pH / 2.0f };
	mPos = centerPos;
	mAttachedOffset = pOffset;

	mRotateMachine.calculateDirection(pPlayerPos, pMousePos);
	mRotateMachine.calculateLength(mRotateMachine.getDirection());

	if (mRotateMachine.getLength() > 0.1f) {
		mRotateMachine.calculateSpeed(mRotateMachine.getDirection(),
			mRotateMachine.getLength(),
			mSpeed);

		mRotateMachine.setAngle(DimensionDegOrRad::RADIANS,pAngle);

		mActive = true;
		updateAttachedRect(pWeaponRect);
	}
}

void Bullets::makeBulletCirclingAround(bool pCircling, float pMultiplier)
{
	mCircling = pCircling;
	mMultiplier = pMultiplier;
}

bool Bullets::isColliding(SDL_Rect pRect)
{
	SDL_FRect tmpRect = { static_cast<float>(pRect.x + 20),static_cast<float>(pRect.y + 20),
						  static_cast<float>(pRect.w - 32),static_cast<float>(pRect.h - 30) };

	return SDL_HasIntersectionF(&mRect, &tmpRect);
}

bool Bullets::isActive()
{
	return mActive;
}

bool Bullets::checkBoundaries()
{
	if (mPos.mX >= WIN_WIDTH + mRect.w  || mPos.mX <= -50 ||
		mPos.mY >= WIN_HEIGHT + 100		|| mPos.mY <= -50)
		return true;
	return false;
}

SDL_Rect& Bullets::getShape()
{
	return TextureManager::getInstance().getRect(mPath);
}

Vector2f Bullets::getPos() const noexcept
{
	return { static_cast<float>(mRect.x), static_cast<float>(mRect.y) };
}

void Bullets::setPos(const Vector2f& pPos)
{
	mPos = pPos;
	mRect.x = pPos.mX;
	mRect.y = pPos.mY;
}

int32_t Bullets::getSpeed() const noexcept
{
	return mSpeed;
}

void Bullets::setSpeed(float pSpeed)
{
	mSpeed = pSpeed;
}

PATH Bullets::getPath()
{
	return mPath;
}

void Bullets::setPath(const PATH& pPath)
{
	mPath = pPath;
	SDL_Rect tmpRect = { static_cast<int>(mRect.x),static_cast<int>(mRect.y),
						 static_cast<int>(mRect.w),static_cast<int>(mRect.h) };
	if (TextureManager::getInstance().containsPath(pPath))
		TextureManager::getInstance().appendTextureWithoutBack(mRenderer, pPath, tmpRect, { 34,177,76,255 });
}

void Bullets::render()
{
	if (mActive)
	{
		if (!TextureManager::getInstance().containsPath(mPath))
		{
			SDL_Rect tmpRect = { static_cast<int>(mRect.x),static_cast<int>(mRect.y),
							     static_cast<int>(mRect.w),static_cast<int>(mRect.h) };
			TextureManager::getInstance().appendTextureWithoutBack(mRenderer, mPath, tmpRect, { 34,177,76,255 });
		}
		if (!mCircling)
		{
			SDL_RenderCopyExF(mRenderer, TextureManager::getInstance().getTexture(mPath), nullptr, &mRect, mRotateMachine.getAngle() * 180 / M_PI, nullptr,
							  InputManager::getInstance().getMousePos().mX >= mPos.mX ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL);
		}
		else
		{
			static float tmpAngle = mRotateMachine.getAngle();
			SDL_RenderCopyExF(mRenderer, TextureManager::getInstance().getTexture(mPath), nullptr, &mRect, (tmpAngle += mMultiplier) * 180 / M_PI, nullptr,
				InputManager::getInstance().getMousePos().mX >= mPos.mX ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL);
			if (tmpAngle >= 180.0f)
				tmpAngle = -180.0f;
		}
	}
}

void Bullets::update()
{
	if (mActive)
	{
		if (checkBoundaries())
			mActive = false;

		mRect.x += mRotateMachine.getSpeed().mX;
		mRect.y += mRotateMachine.getSpeed().mY;

		mPos.mX = static_cast<float>(mRect.x);
		mPos.mY = static_cast<float>(mRect.y);
	}
}

void Bullets::updateAttachedRect(SDL_Rect pWeaponRect)
{
	float angleRect = mRotateMachine.getAngle();
	
	float weaponCenterX = static_cast<float>(pWeaponRect.x) + static_cast<float>(pWeaponRect.w) / 2.0f;
	float weaponCenterY = static_cast<float>(pWeaponRect.y) + static_cast<float>(pWeaponRect.h) / 2.0f;
	 
	float rotatedX = mAttachedOffset.mX * cos(angleRect) - mAttachedOffset.mY * sin(angleRect);
	float rotatedY = mAttachedOffset.mX * sin(angleRect) + mAttachedOffset.mY * cos(angleRect);
	
	mRect.x = weaponCenterX + rotatedX - mRect.w / 2.0f;
	mRect.y = weaponCenterY + rotatedY - mRect.h / 2.0f;
}