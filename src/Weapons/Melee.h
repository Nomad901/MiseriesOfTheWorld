#pragma once
#include "Weapons/Weapon.h"
#include "Game/RotateMachine.h"
#include "Weapons/WeaponVisitor.h"

class Melee : public Weapon
{
protected:
	struct Config
	{
		struct WeaponConfig
		{
			int16_t mWeight{};
			float mDelay{};
			float mRobustness{};
			SDL_FRect mHitBoxAttack{};
			std::pair<int32_t, int32_t> mPower{};
			int32_t mW{}, mH{};
			Vector2f mPosWeapon{ 0.0f,0.0f };
		} mWeaponConfig;
	} mConfig;

public:
	Melee() = default;
	virtual ~Melee() = default;
	
	void initMelee(SDL_Renderer* pRenderer, const Config::WeaponConfig& pConfig, 
				   SDL_FRect pCharRect);
	void setHitBoxAttack(SDL_FRect pHitBoxAttack);
	void setCurrentRobustness(float pCurrentRobustness);
	bool manageRobustness();
	float getCurrentRobustness();
	SDL_FRect getHitBoxAttack() const noexcept;
	
	void showHitBox(bool pShowHitbox);
	bool WeaponIsInView(SDL_Rect pCharCollision) override;
	void render(SDL_Renderer* pRenderer) override;
	void update(const Vector2f& pPos) override;

	virtual void attack() = 0;
	virtual bool checkDamage(SDL_FRect pEnemyRect) = 0;
	virtual void setAsASpecialWeapon() = 0;
	virtual void accept(WeaponVisitor& pWeaponVisitor) = 0;

protected:
	void manageRotateAround(const Vector2f& pPos);
	void rotateHitBox(SDL_FRect pBaseRect);
	void subtractionRobustness();

private:
	bool manageShootState();
	bool manageBrokenState();

private:
	bool mShowHitBox{};
	float mCurrentRobustness{};
	SDL_Renderer* mRenderer{};

	RotateMachine mRotateMachine;
	RotateMachine mRotateHitBoxMachine;
	FactoryObjects mFactoryObjects;
};

