#pragma once
#include "FireMode.h"

enum class Mode
{
	SEQUENCE = 0,
	SINGLE = 1
};

class SingleOrSequence : public FireMode
{
public:
	SingleOrSequence() = default;

	void setMode(Mode pMode);

	void shoot(SDL_Rect pCharRect, SDL_Rect pWeaponRect, bool pWasReload) override;
	void update(SDL_Renderer* pRenderer, const Vector2f& pPos) override;
	void render() override;

	void setAsSpecial() override;
	bool manageDelay() override;
private:
	RotateMachine mRotateMachine;
	Timer mTimer;

	Mode mMode{ Mode::SEQUENCE };
};

