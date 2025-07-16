#pragma once
#include "Weapons/Weapon.h"
#include "Game/RotateMachine.h"
#include "Weapons/WeaponVisitor.h"

class Throwable : public Weapon
{
protected:
	struct Config
	{
		struct WeaponConfig
		{
			PATH mPathBullets{};
			std::pair<int32_t, int32_t> mPower{};
			int32_t mW{}, mH{};
			int32_t mDelayBullets{};
			int32_t mSpeedBullets{};
			Vector2f mPosWeapon{ 0.0f,0.0f };
		} mWeaponConfig;
	} mConfig;

public:
	Throwable() = default;
	virtual ~Throwable() = default;
		
	void initThrowable(SDL_Renderer* pRenderer, const Config::WeaponConfig& pWeaponConfig,
					   SDL_FRect pCharRect);
	void setTexture(const PATH& pPath);

	bool WeaponIsInView(SDL_Rect pCharCollision) override;
	void render(SDL_Renderer* pRenderer) override;
	void update(const Vector2f& pPos) override;

	virtual std::pair<int32_t, bool> manageDamage(SDL_FRect pEnemyRect) = 0;
	virtual void updateBullets(SDL_Renderer* pRenderer) = 0;
	virtual void shoot() = 0;
	virtual bool checkDamage(SDL_FRect pEnemyRect) = 0;
	virtual void setAsASpecialWeapon() = 0;
	virtual void accept(WeaponVisitor& pWeaponVisitor) = 0;

protected:
	void manageRotateAround(const Vector2f& pPos);

private:
	SDL_Renderer* mRenderer{};
	RotateMachine mRotateMachine;
	FactoryObjects mFactoryObjects;
};

