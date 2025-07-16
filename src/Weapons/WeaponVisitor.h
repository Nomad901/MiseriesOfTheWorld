#pragma once
#include "Math/Vector2f.h"
#include "SDL/SDL.h"

class Gun;
class Melee;
class RPG;
class Throwable;

class WeaponVisitor
{
public:
	virtual ~WeaponVisitor() = default;
	
	virtual void visit(Gun& pGun) = 0;
	virtual void visit(Melee& pMelee) = 0;
	virtual void visit(RPG& pRPG) = 0;
	virtual void visit(Throwable& pThrowable) = 0;
};

struct ShootVisitor : public WeaponVisitor
{
	void visit(Gun& pGun) override;
	void visit(Melee& pMelee) override;
	void visit(RPG& pRPG) override;
	void visit(Throwable& pThrowable) override;
};

struct ReloadVisitor : public WeaponVisitor
{
	void visit(Gun& pGun) override;
	void visit(RPG& pRPG) override;
};

class RenderUpdateVisitor : public WeaponVisitor
{
public:
	RenderUpdateVisitor(SDL_Renderer* pRenderer)
		: mRenderer(pRenderer), mCharPos{}, 
		  mWeaponPos{} {}

	void updatePositions(const Vector2f& pCharPos, const Vector2f& pWeaponPos);

	void visit(Gun& pGun) override;
	void visit(Melee& pMelee) override;
	void visit(RPG& pRPG) override;
	void visit(Throwable& pThrowable) override;

private:
	SDL_Renderer* mRenderer;

	Vector2f mCharPos;
	Vector2f mWeaponPos;
};

