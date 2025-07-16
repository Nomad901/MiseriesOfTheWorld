#pragma once
#include "Weapons/Weapon.h"
#include "Weapons/ReloadLogic.h"
#include "Game/AnimateStateMachine.h"
#include "Weapons/FireModeFactory.h"
#include "Game/RotateMachine.h"
#include "Weapons/WeaponVisitor.h"

class Gun : public Weapon
{
protected:
	struct Config
	{
		struct WeaponConfig
		{
			float mRobustness{};
			int16_t mWeight{};
			std::pair<int32_t, int32_t> mPower{};
			int32_t mW{}, mH{};
			Vector2f mPosWeapon{ 0.0f,0.0f };
		} mWeaponConfig;
		struct BulletsConfig
		{
			int32_t mCapacityBullets{};
			PATH mPathBullets = std::filesystem::current_path();
			int32_t mDelayBullets{};
			int32_t mQuantityBullets{};
			int32_t mQuantitySetsBullets{};
			int32_t mW{}, mH{};
			int32_t mSpeedBullets{};
			Vector2f mPosBullets{ 0.0f,0.0f };
		} mBulletsConfig;
		struct ReloadConfig
		{
			bool mShowReloadingQuote{ false };
			int16_t mReloadingTime{};
			int32_t mSizeNumbersReload{ 25 };
			SDL_Color mColorNumbersReload{ 255,255,255,255 };
		} mReloadConfig;
	} mConfig;

public:
	Gun() = default;
	virtual ~Gun() = default;

	void initGun(SDL_Renderer* pRenderer, const Config::ReloadConfig& pReloadConfig, SDL_FRect pCharRect);
	void manageRobustness();
	void setReloadAnimationEndless(bool pEndlessAnim);
	void setTangoIsCharging(bool pTangoIsCharging);
	void setTangoIsWaiting(bool pTangoIsWaiting);
	bool getTangoIsWaiting() const noexcept;
	float getCurrentRobustness() const noexcept;
	float getAngleOfWeapon() const noexcept;

	bool WeaponIsInView(SDL_Rect pCharCollision) override;
	void render(SDL_Renderer* pRenderer) override;
	void update(const Vector2f& pPos) override;

	virtual void shoot() = 0;
	virtual void reload() = 0;
	virtual bool checkDamage(SDL_FRect pEnemyRect) = 0;
	virtual void setAsASpecialWeapon() = 0;
	virtual void accept(WeaponVisitor& pWeaponVisitor) = 0;

protected:
	bool brokenChecker();
	void manageRotateAround(const Vector2f& pPos);

private:
	bool manageShootState(SDL_Renderer* pRenderer);
	bool manageReloadState(SDL_Renderer* pRenderer);
	bool manageBrokenState(SDL_Renderer* pRenderer);

private:
	bool mTangoIsWaiting{ false };
	bool mTangoIsCharging{ false };
	bool mDelayIsGoing{ false };
	bool mEndlessAnim{ false };
	float mCurrentRobustness{ Weapon::getWeaponStats().mScaleOfRobustness };

	ReloadLogic mReloadLogic;
	RotateMachine mRotateMachine;
	FactoryObjects mFactoryObjects;
};

