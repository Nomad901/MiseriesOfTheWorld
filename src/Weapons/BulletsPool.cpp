#include "BulletsPool.h"

void BulletsPool::init(size_t pCapacity, SDL_Renderer* pRenderer, const PATH& pPath, const Vector2f& pPos, 
					   int32_t pW, int32_t pH, int32_t pSpeed)
{
	mCapacity = pCapacity;
	mRenderer = pRenderer;
	mPath = pPath;
	release();

	mStorageBullets.reserve(pCapacity);
	for (size_t i = 0; i < pCapacity +1; ++i)
	{
		mStorageBullets.push_back(std::make_unique<Bullets>(pRenderer,pPath, pPos, pW, pH, pSpeed));
	}
	mOneInstance = std::move(mStorageBullets[pCapacity]);
	mStorageBullets.pop_back();
}

void BulletsPool::release()
{
	mStorageBullets.clear();
}

std::unique_ptr<Bullets> BulletsPool::getFirstBullet()
{
	if (mStorageActiveBullets.empty())
		makeBulletActive();
	clearEmptyData();
	auto bullet = std::move(mStorageActiveBullets.front());
	return std::move(bullet);
}

std::unique_ptr<Bullets> BulletsPool::getBullet(size_t pIndex)
{
	if (mStorageActiveBullets.empty())
		makeBulletActive();
	if (pIndex > mStorageActiveBullets.size())
		return nullptr;
	auto bullet = std::move(mStorageActiveBullets[pIndex]);
	return std::move(bullet);
}

std::unique_ptr<Bullets> BulletsPool::getBullet(std::unique_ptr<Bullets> pIndex)
{
	if (mStorageActiveBullets.empty())
	{
		LOG("Bullet is not active!");
		return nullptr;
	}
	else
	{
		auto it = std::find(mStorageActiveBullets.begin(), mStorageActiveBullets.end(), pIndex);
		return std::move(*it);
	}
}

std::unique_ptr<Bullets>& BulletsPool::manageFirstBulletInside()
{
	if (mStorageActiveBullets.empty())
		makeBulletActive();
	clearEmptyData();
	return mStorageActiveBullets.front();
}

std::unique_ptr<Bullets>& BulletsPool::manageLastBulletInside()
{
	if (mStorageActiveBullets.empty())
		makeBulletActive();
	clearEmptyData();
	if(!mStorageActiveBullets.empty() && mStorageActiveBullets.back()!=nullptr)
		return mStorageActiveBullets.back();
}

std::unique_ptr<Bullets>& BulletsPool::manageBulletIndise(size_t pIndex)
{
	if (mStorageActiveBullets.empty())
		makeBulletActive();
	if (pIndex < mStorageActiveBullets.size())
		return mStorageActiveBullets[pIndex];
}

std::pair<size_t, bool> BulletsPool::hasIntersection(SDL_Rect pRect)
{
	clearEmptyData();
	if (!mStorageActiveBullets.empty())
	{
		for (size_t i = 0; i < mStorageActiveBullets.size(); ++i)
		{
			if (mStorageActiveBullets[i]->isColliding(pRect))
				return std::make_pair(i, true);
		}
	}
	return std::make_pair(0, false);
}

void BulletsPool::returnActiveBullet(std::unique_ptr<Bullets> pActiveBullet)
{
	mStorageActiveBullets.push_back(std::move(pActiveBullet));
}

void BulletsPool::makeBulletsCirclingAround(bool pCircling, float pMultiplier)
{
	mCircling = pCircling;
	mMultiplier = pMultiplier;
}

void BulletsPool::makeBulletActive()
{
	mStorageActiveBullets.push_back(std::move(mStorageBullets.back()));
	mStorageActiveBullets.back()->makeBulletCirclingAround(mCircling, mMultiplier);
	mStorageBullets.pop_back();
}

void BulletsPool::destroyBullet(size_t pIndex)
{
	if (mStorageActiveBullets.empty() || pIndex > mStorageActiveBullets.size())
		LOG("There is no active bullets or index is invalid!");
	else
	{
		auto it = mStorageActiveBullets.begin() + pIndex;
		mStorageActiveBullets.erase(it);
	}
}

void BulletsPool::destroyBullet(std::unique_ptr<Bullets> pIndex)
{
	if (mStorageActiveBullets.empty())
		LOG("Bullet is not active!");
	else
	{
		auto it = std::find(mStorageActiveBullets.begin(), mStorageActiveBullets.end(), pIndex);
		mStorageActiveBullets.erase(it);
	}
}

void BulletsPool::destroyFirstBullet()
{
	if (mStorageActiveBullets.empty())
		LOG("Storage is empty!");
	else
	{
		auto it = std::move(mStorageActiveBullets.begin());
		mStorageActiveBullets.erase(it);
	}
}

void BulletsPool::destroyLastBullet()
{
	if (mStorageActiveBullets.empty())
		LOG("Storage is empty!");
	else
	{
		auto it = std::move(mStorageActiveBullets.end()-1);
		mStorageActiveBullets.erase(it);
	}
}

size_t BulletsPool::getSizeBullets() const noexcept
{
	return mStorageBullets.size();
}

size_t BulletsPool::getSizeActiveBullets() const noexcept
{
	return mStorageActiveBullets.size();
}

void BulletsPool::update()
{
#ifdef _DEBUG
	/*std::cout << std::format("Size of mStorageActiveBullets: {}\n", mStorageActiveBullets.size());
	std::cout << std::format("Size of mStorageBullets: {}\n\n", mStorageBullets.size());*/
#endif // _DEBUG

	if (mStorageBullets.size() <= 2)
	{
		init(mCapacity, mRenderer, mPath, mOneInstance->getPos(), 
		  	 mOneInstance->getShape().w, mOneInstance->getShape().h, mOneInstance->getSpeed());
		mStorageBullets.pop_back();
	}
	if (!mStorageActiveBullets.empty())
	{
		static std::vector<int> indexesOutOf;
		clearEmptyData();
		for (size_t i = 0; i < mStorageActiveBullets.size(); ++i)
		{
			if (mStorageActiveBullets[i] != nullptr)
			{
				mStorageActiveBullets[i]->render();
				mStorageActiveBullets[i]->update();
				if (mStorageActiveBullets[i]->checkBoundaries())
					indexesOutOf.push_back(i);
			}
		}
		clearBulletsOurOfBorder(indexesOutOf);
		indexesOutOf.clear();
	}
}

void BulletsPool::clearEmptyData()
{
	static std::vector<size_t> indexes;
	for (size_t i = 0; i < mStorageActiveBullets.size(); ++i)
	{
		if (mStorageActiveBullets[i] == nullptr)
			indexes.push_back(i);
	}
	if (!indexes.empty())
	{
		for(auto it = indexes.rbegin(); it != indexes.rend(); ++it)
		{
			if (!mStorageActiveBullets.empty())
			{
				if (mStorageActiveBullets.begin() + *it != mStorageActiveBullets.end())
					mStorageActiveBullets.erase(mStorageActiveBullets.begin() + *it);
			}
		}
		indexes.clear();
	}
}

void BulletsPool::clearBulletsOurOfBorder(const std::vector<int>& pStorage)
{
	for (auto& i : pStorage)
	{
		mStorageActiveBullets[i] = nullptr;
	}
	clearEmptyData();
}


