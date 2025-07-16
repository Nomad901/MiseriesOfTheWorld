#pragma once
#include <vector>
#include <memory>

#include "Bullets.h"

class BulletsPool
{
public:
	BulletsPool() = default;

	void init(size_t pCapacity, SDL_Renderer* pRenderer, const PATH& pPath, const Vector2f& pPos,
			  int32_t pW, int32_t pH, int32_t pSpeed);
	void release();

	std::unique_ptr<Bullets> getFirstBullet();
	std::unique_ptr<Bullets> getBullet(size_t pIndex);
	std::unique_ptr<Bullets> getBullet(std::unique_ptr<Bullets> pIndex);
	std::unique_ptr<Bullets>& manageFirstBulletInside();
	std::unique_ptr<Bullets>& manageLastBulletInside();
	std::unique_ptr<Bullets>& manageBulletIndise(size_t pIndex);
	std::pair<size_t,bool> hasIntersection(SDL_Rect pRect);

	void returnActiveBullet(std::unique_ptr<Bullets> pActiveBullet);

	void makeBulletsCirclingAround(bool pCircling, float pMultiplier);
	void makeBulletActive();
	void destroyBullet(size_t pIndex);
	void destroyBullet(std::unique_ptr<Bullets> pIndex);
	void destroyFirstBullet(); 
	void destroyLastBullet();

	size_t getSizeBullets() const noexcept;
	size_t getSizeActiveBullets() const noexcept;

	void update();
private:

	void clearEmptyData();
	void clearBulletsOurOfBorder(const std::vector<int>& pStorage);

private:
	bool mCircling{ false };
	float mMultiplier{ 10.0f };

	size_t mCapacity{ 0 };
	SDL_Renderer* mRenderer{ nullptr };

	PATH mPath{ std::filesystem::current_path() };

	std::vector<std::unique_ptr<Bullets>> mStorageBullets;
	std::vector<std::unique_ptr<Bullets>> mStorageActiveBullets;

	std::unique_ptr<Bullets> mOneInstance{ nullptr };

};

