#include "FireMode.h"

void FireMode::init(SDL_Renderer* pRenderer, int32_t pCapacity, const PATH& pPath,
					int32_t pDelay, int32_t pQuantityBullets, int32_t pQuantitySets, 
					const Vector2f& pPos, int32_t pW, int32_t pH, int32_t pSpeed)
{
	mFireStat.mPath = pPath;
	mFireStat.mCapacity = pCapacity;
	mFireStat.mDelay = pDelay;
	mFireStat.mH = pH;
	mFireStat.mW = pW;
	mFireStat.mPos = pPos;
	mFireStat.mQuantityBullets = pQuantityBullets;
	mFireStat.mOriginalQuantityBullets = pQuantityBullets;
	mFireStat.mQuantitySets = pQuantitySets;
	mFireStat.mSpeed = pSpeed;

	mBulletsPool = std::make_unique<BulletsPool>();
	mBulletsPool->init(pCapacity, pRenderer, pPath, pPos, pW, pH, pSpeed);
}

void FireMode::setDelay(int32_t pDelay)
{
	mFireStat.mDelay = pDelay;
}

void FireMode::setQuantityBullets(int32_t pQuantity)
{
	mFireStat.mQuantityBullets = pQuantity;
}

void FireMode::setOrigQuantityBullets(int32_t pQuantity)
{
	mFireStat.mOriginalQuantityBullets = pQuantity;
}

void FireMode::setQuantitySets(int32_t pQuantity)
{
	mFireStat.mQuantitySets = pQuantity;
}

void FireMode::setShouldBreakWeapon(bool pShould)
{
	mShouldBreakWeapon = pShould;
}

void FireMode::setWasDamage(bool pWas)
{
	mWasDamage = pWas;
}

void FireMode::setPos(const Vector2f& pPos)
{
	mFireStat.mPos = pPos;
}

void FireMode::setDelayGoing(bool mIsGoing)
{
	mDelayIsGoing = mIsGoing;
}

bool FireMode::activeBulletsExist()
{
	return mBulletsPool->getSizeActiveBullets() != 0 ? true : false;
}

bool FireMode::shouldBreakWeapon()
{
	return mShouldBreakWeapon;
}

bool FireMode::wasDamage()
{
	return mWasDamage;
}

bool FireMode::delayIsGoing()
{
	return mDelayIsGoing;
}

int32_t FireMode::getQuantityBullets()
{
	return mFireStat.mQuantityBullets;
}

int32_t FireMode::getOrigQuantityBullets()
{
	return mFireStat.mOriginalQuantityBullets;
}

int32_t FireMode::getQuantitySets()
{
	return mFireStat.mQuantitySets;
}

Vector2f FireMode::getPos()
{
	return mFireStat.mPos;
}

BulletsPool& FireMode::getBulletsPoolInstance()
{
	return *mBulletsPool;
}

PATH FireMode::getCurrentPath() const noexcept
{
	return PROJECT_ROOT;
}

std::vector<int> FireMode::isShot(const std::vector<SDL_Rect>& pRects)
{
	static std::vector<int> shotIndexes;

	auto lIsColiding = [&](SDL_Rect pRect) -> std::pair<bool, int32_t>
		{
			for (size_t i = 0; i < mBulletsPool->getSizeActiveBullets(); ++i)
			{
				if (mBulletsPool->manageBulletIndise(i)->isColliding(pRect))
					return std::make_pair(true, i);
			}
			return std::make_pair(false, 0);
		};

	if (mBulletsPool->getSizeActiveBullets() != 0 && mBulletsPool->getSizeBullets() != 0)
	{
		for (size_t i = 0; i < pRects.size(); ++i)
		{
			auto it = lIsColiding(pRects[i]);
			if (it.first)
			{
				if (std::ranges::find(shotIndexes, i) == shotIndexes.end())
				{
					shotIndexes.push_back(i);
					mWasDamage = true;
				}
				else
				{
					mWasDamage = false;	
					mBulletsPool->destroyBullet(it.second);
					shotIndexes.clear();
				}
			}
			else
				shotIndexes.clear();
		}
		//std::cout << std::format("Indexes: {}\n", shotIndexes);
	}
	return shotIndexes;
}

FireMode::FireStat& FireMode::getFireStat()
{
	return mFireStat;
}
