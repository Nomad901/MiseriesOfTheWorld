#include "AnimatedTexture.h"

AnimatedTexture::~AnimatedTexture()
{
	for (auto& i : mStorageFrames)
	{
		SDL_DestroyTexture(i.second.first);
	}
	mStorageFrames.clear();
}

void AnimatedTexture::appendAnimation(TypeWait pType, SDL_Renderer* pRenderer, const PATH& pPath, const Vector2f& pPos, const int pW, const int pH, HorVer pSide)
{
	mPath = pPath;
	mHorVer = pSide;
	if (pType == TypeWait::WAIT)
		mConditionals.mWaitAnim = true;

	if (mAnimation.find(pPath) == mAnimation.end())
	{
		SDL_Rect tmpRect{ static_cast<int>(pPos.mX), static_cast<int>(pPos.mY), pW, pH };
		if (mTextures.find(mPath) == mTextures.end())
			appendTexture(pRenderer, mPath, tmpRect);
		else
		{
			deleteTexture(mPath);
			appendTexture(pRenderer, mPath, tmpRect);
		}
		setDimensions(mPath, pW, pH);
		TextureManager::setPosition(mPath, pPos);

		mDstRect = {
			pPos.mX,
			pPos.mY,
			static_cast<float>(pW),
			static_cast<float>(pH)
		};

		dividerFrames(pRenderer);
	}
	mTimer.setDimensionOfTime(Dimension::MILISECONDS);
	mTimer.setLimit(100.0f);
}

void AnimatedTexture::setIntensity(const float pIntensity)
{
	mIntensity = pIntensity;
}

void AnimatedTexture::setCurrentSide(const SideOfChar pSide)
{
	mCurrentSide = pSide;
}

void AnimatedTexture::setActive(const bool pActive)
{
	mConditionals.mActive = pActive;
}

void AnimatedTexture::waitWithAnim(const bool pWaitAnim)
{
	mConditionals.mWaitAnim = pWaitAnim;
}

//check for the purple background. each frame - append to the vector through "appendWithoutBack()"
void AnimatedTexture::dividerFrames(SDL_Renderer* pRenderer)
{
	SDL_Color colorOfBack = getBackGroundColor(mPath);
	SDL_Color colorOfPos = { 0,0,0,0 };

	bool inFrame		 = false;
	bool columnHasSprite = false;

	int firstPoint = 0;
	int _x1 = 0, _x2 = 0;
	int _y1 = 0, _y2 = 0;

	SDL_Surface* tmpSurface = getSurface(mPath);
	mSprites.clear();
	mSprites.reserve(50);

	SDL_LockSurface(tmpSurface);

	if (mHorVer == HorVer::HORIZONTAL)
	{
		_y2 = tmpSurface->h;
		for (int x = 0; x < tmpSurface->w; ++x)
		{
			for (int y = 0; y < tmpSurface->h; ++y)
			{
				colorOfPos = getBackGroundColor(tmpSurface, { static_cast<float>(x),static_cast<float>(y) });

				columnHasSprite = !(colorOfBack.r == colorOfPos.r && colorOfBack.g == colorOfPos.g &&
									colorOfBack.b == colorOfPos.b && colorOfBack.a == colorOfPos.a);
				
				if (columnHasSprite)
				{
					inFrame = true;
					if (firstPoint == 0)
						firstPoint = x;
					break;
				}
				else
					inFrame = false;
			}
			if (firstPoint != 0 && !inFrame)
			{
				mSprites.push_back({ firstPoint, 0, x, _y2 });
				firstPoint = 0;
			}
		}
		for (auto& i : mSprites)
		{
			i.w -= i.x;
		}
	}
	else
	{
		_x2 = tmpSurface->w;
		for (int y = 0; y < tmpSurface->h; ++y)
		{
			for (int x = 0; x < tmpSurface->w; ++x)
			{
				colorOfPos = getBackGroundColor(tmpSurface, { static_cast<float>(x),static_cast<float>(y) });

				columnHasSprite = !(colorOfBack.r == colorOfPos.r && colorOfBack.g == colorOfPos.g &&
									colorOfBack.b == colorOfPos.b && colorOfBack.a == colorOfPos.a);

				if (columnHasSprite)
				{
					inFrame = true;
					if (firstPoint == 0)
						firstPoint = y;
					break;
				}
				else
					inFrame = false;
			}
			if (firstPoint != 0 && !inFrame)
			{
				mSprites.push_back({0,firstPoint,_x2,y});
				firstPoint = 0;
			}
		}
		for (auto& i : mSprites)
		{
			i.h -= i.y;
		}
	}

	SDL_UnlockSurface(tmpSurface);

	createTexturesFromFrames(pRenderer);

	if (mConditionals.mWaitAnim)
	{
		for (auto& i : mStorageWaitFrames)
		{
			deleteBackground(pRenderer, i.second.first);
		}
	}
	else
	{
		for (auto& i : mStorageFrames)
		{
			deleteBackground(pRenderer, i.second.first);
		}
	}
}

void AnimatedTexture::createTexturesFromFrames(SDL_Renderer* pRenderer)
{
	SDL_Texture* sourceTexture = getTexture(mPath);

	SDL_SetTextureBlendMode(sourceTexture, SDL_BLENDMODE_BLEND);

	for (size_t i = 0; i < mSprites.size(); ++i)
	{
		SDL_Texture* tmpTexture = SDL_CreateTexture(pRenderer, 
													SDL_PIXELFORMAT_RGBA8888,
													SDL_TEXTUREACCESS_TARGET, 
													mSprites[i].w, mSprites[i].h);
		if (!tmpTexture)
			LOG("couldnt initialize the texture!");

		SDL_SetTextureBlendMode(tmpTexture, SDL_BLENDMODE_BLEND);
		
		SDL_SetRenderTarget(pRenderer, tmpTexture);
		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 0);
		SDL_RenderClear(pRenderer);
		
		SDL_RenderCopy(pRenderer, sourceTexture, &mSprites[i], nullptr);
		
		SDL_SetRenderTarget(pRenderer, nullptr);

		if(mConditionals.mWaitAnim)
			mStorageWaitFrames[static_cast<int>(i)] = { tmpTexture, mSprites[i] };
		else
			mStorageFrames[static_cast<int>(i)] = { tmpTexture, mSprites[i] };
	}
	if(!mStorageFrames.empty())
		mAnimation.emplace(mPath, mStorageFrames);
	if (!mStorageWaitFrames.empty())
		mAnimation.emplace(mPath, mStorageWaitFrames);
	SDL_DestroyTexture(sourceTexture);
}

void AnimatedTexture::setSides(const std::unordered_map<SideOfChar, std::vector<uint32_t>>& pNumbers)
{
	for (const auto& i : pNumbers)
	{
		for (const auto& j : i.second)
		{
			if (j > mStorageFrames.size())
			{
				LOG("Number which u are trying to contain is bigger than size of the storage of frames: " + std::to_string(mStorageFrames.size()));
				return;
			}
		}
	}
	mSteps = pNumbers;
}

void AnimatedTexture::setPosition(const Vector2f& pPos)
{
	mDstRect.x = static_cast<int>(pPos.mX);
	mDstRect.y = static_cast<int>(pPos.mY);
}

void AnimatedTexture::setPositionRotate(SDL_FRect pBaseRect, float pAngle, const Vector2f& pOffset)
{
	mRotateMachine.rotateAttachedRect(mDstRect, pBaseRect, pAngle, pOffset);
}

bool AnimatedTexture::isEnded() const
{
	return mConditionals.mEnded;
}

void AnimatedTexture::setEnded(bool pEnded)
{
	mConditionals.mEnded = pEnded;
}

void AnimatedTexture::enableWaitAnim(bool pWait)
{
	mConditionals.mInWaitAnim = pWait;
}

void AnimatedTexture::setAnimating(bool pAnimating)
{
	mConditionals.mAnimating = pAnimating;
}

bool AnimatedTexture::isInWaitAnim() const noexcept
{
	return mConditionals.mInWaitAnim;
}

bool AnimatedTexture::isAnimating() const noexcept
{
	return mConditionals.mAnimating;
}

Vector2f AnimatedTexture::getPosition() const
{
	return { static_cast<float>(mDstRect.x), static_cast<float>(mDstRect.y) };
}

AnimatedTexture::conditionalVars const& AnimatedTexture::getAllConditions() const noexcept
{
	return mConditionals;
}

bool AnimatedTexture::isActive() const
{
	return mConditionals.mActive;
}

SDL_FRect AnimatedTexture::getAnimRect() const noexcept
{
	return mDstRect;
}

void AnimatedTexture::stopAnimation()
{
	mConditionals.mAnimating = false;
}

void AnimatedTexture::runAnimation()
{
	mTimer.startTimer();
	mConditionals.mAnimating = true;
	mConditionals.mOnceAnim = false;
}

void AnimatedTexture::runAnimationOnlyOnce()
{
	mTimer.startTimer();
	mConditionals.mAnimating = true;
	mConditionals.mOnceAnim = true;
}

void AnimatedTexture::render(SDL_Renderer* pRenderer)
{
	if (mConditionals.mAnimating)
	{
		if (mConditionals.mActive && !mConditionals.mInWaitAnim)
		{
			if (!mSteps.empty())
			{
				if (mCounter >= mSteps[mCurrentSide].size())
				{
					mConditionals.mEnded = true;
					if (!mConditionals.mOnceAnim)
						mCounter = 0;
					else
					{
						mConditionals.mActive = false;
						mConditionals.mAnimating = false;
					}
				}
				else
					SDL_RenderCopyF(pRenderer, mStorageFrames[mSteps[mCurrentSide][mCounter]].first, nullptr, &mDstRect);
				if (mTimer.getDeltaTime(false) > mIntensity)
				{
					mTimer.resetTimer();
					if(mCounter < mSteps[mCurrentSide].size())
						mCounter++;
				}
			}
			else
				MESSAGE("U didnt set steps!");
		}
		else if(mConditionals.mWaitAnim && mConditionals.mInWaitAnim)
		{
			if (mCounter >= mSteps[mCurrentSide].size())
			{
				mConditionals.mEnded = true;
				if (!mConditionals.mOnceAnim)
					mCounter = 0;
				else
				{
					mConditionals.mActive = false;
					mConditionals.mAnimating = false;
				}
			}
			else
				SDL_RenderCopyF(pRenderer, mStorageWaitFrames[mCounter].first, nullptr, &mDstRect);
			if (mTimer.getDeltaTime(false) > mIntensity)
			{
				mTimer.resetTimer();
				if (mCounter < mSteps[mCurrentSide].size())
					mCounter++;
			}
		}
		else
			SDL_RenderCopyF(pRenderer, mStorageFrames[mSteps[mCurrentSide][0]].first, nullptr, &mDstRect);
	}
	else
		mCounter = 0;
}

void AnimatedTexture::renderWithRotate(SDL_Renderer* pRenderer, double pAngle, const Vector2f& pMousePos)
{
	if (mConditionals.mAnimating)
	{
		if (mConditionals.mActive && !mConditionals.mInWaitAnim)
		{
			if (!mSteps.empty())
			{
				if (mCounter >= mSteps[mCurrentSide].size())
				{
					mConditionals.mEnded = true;
					if (!mConditionals.mOnceAnim)
						mCounter = 0;
					else
						SDL_RenderCopyExF(pRenderer, mStorageFrames[mSteps[mCurrentSide][mCounter - 1]].first, nullptr, &mDstRect, pAngle, NULL,
										  pMousePos.mX >= getPosition().mX ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL);
				}
				else
					SDL_RenderCopyExF(pRenderer, mStorageFrames[mSteps[mCurrentSide][mCounter]].first, nullptr, &mDstRect, pAngle, NULL, 
									  pMousePos.mX >= getPosition().mX ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL);
				if (mTimer.getDeltaTime(false) > mIntensity)
				{
					mTimer.resetTimer();
					if (mCounter < mSteps[mCurrentSide].size())
						mCounter++;
				}
			}
			else
				MESSAGE("U didnt set steps!");
		}
		else if (mConditionals.mWaitAnim && mConditionals.mInWaitAnim)
		{
			if (mCounter >= mSteps[mCurrentSide].size())
			{
				mConditionals.mEnded = true;
				if (!mConditionals.mOnceAnim)
					mCounter = 0;
				else
				{
					mConditionals.mActive = false;
					mConditionals.mAnimating = false;
				}
			}
			else
				SDL_RenderCopyExF(pRenderer, mStorageFrames[mSteps[mCurrentSide][mCounter]].first, nullptr, &mDstRect, pAngle, NULL,
								 pMousePos.mX >= getPosition().mX ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL);
			if (mTimer.getDeltaTime(false) > mIntensity)
			{
				mTimer.resetTimer();
				if (mCounter < mSteps[mCurrentSide].size())
					mCounter++;
			}
		}
		else
			SDL_RenderCopyF(pRenderer, mStorageFrames[mSteps[mCurrentSide][0]].first, nullptr, &mDstRect);
	}
	else
		mCounter = 0;
}

void AnimatedTexture::nullTicks()
{
	mCounter = 0;
}


