#pragma once
#include "Weapons/Weapon.h"
#include "Time/Timer.h"
#include "Weapons/Bullets.h"
#include "Weapons/BulletsPool.h"
#include "Game/RotateMachine.h"

class FireMode
{
private:
	struct FireStat;
public:
	FireMode() = default;
	virtual ~FireMode() = default;

	void init(SDL_Renderer* pRenderer, int32_t pCapacity, const PATH& pPath,
			  int32_t pDelay, int32_t pQuantityBullets, int32_t pQuantitySets,
			  const Vector2f& pPos, int32_t pW, int32_t pH, int32_t pSpeed);
	
	void setDelay(int32_t pDelay);
	void setQuantityBullets(int32_t pQuantity);
	void setOrigQuantityBullets(int32_t pQuantity);
	void setQuantitySets(int32_t pQuantity);
	void setShouldBreakWeapon(bool pShould);
	void setWasDamage(bool pWas);
	void setPos(const Vector2f& pPos);
	void setDelayGoing(bool mIsGoing);

	bool activeBulletsExist();
	bool shouldBreakWeapon();
	bool wasDamage();
	bool delayIsGoing();
	int32_t getQuantityBullets();
	int32_t getOrigQuantityBullets();
	int32_t getQuantitySets();
	Vector2f getPos();
	BulletsPool& getBulletsPoolInstance();
	PATH getCurrentPath() const noexcept;

	std::vector<int> isShot(const std::vector<SDL_Rect>& pRects);

	virtual void render() = 0;
	virtual void shoot(SDL_Rect pCharRect, SDL_Rect pWeaponRect, bool pWasReload) = 0;
	virtual void update(SDL_Renderer* pRenderer, const Vector2f& pPos) = 0;
	virtual void setAsSpecial() = 0;
	virtual bool manageDelay() = 0;
protected:
	FireStat& getFireStat();
protected:
	std::unique_ptr<BulletsPool> mBulletsPool;
private:
	bool mShouldBreakWeapon{ false };
	bool mWasDamage{ false };
	bool mDelayIsGoing{ false };

	struct FireStat {
		int32_t mDelay{ 0 };
		int32_t mQuantityBullets{ 0 };
		int32_t mOriginalQuantityBullets{ 0 };
		int32_t mQuantitySets{ 0 };
		int32_t mSpeed{ 0 };
		int32_t mW{ 0 }, mH{ 0 };
		int32_t mCapacity{ 0 };

		PATH mPath{ std::filesystem::current_path() };

		Vector2f mPos{ 0.0f,0.0f };
	} mFireStat;

};

