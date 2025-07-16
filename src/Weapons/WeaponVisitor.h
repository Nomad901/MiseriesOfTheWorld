#pragma once
#include <vector>

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
	
	virtual std::pair<int32_t, bool> getDamage() = 0;
	virtual void updatePositions(const Vector2f& pCharPos, 
								 const Vector2f& pWeaponPos,
								 std::vector<SDL_FRect> pEnemyPos) = 0;
	virtual void visit(Gun& pGun) = 0;
	virtual void visit(Melee& pMelee) = 0;
	virtual void visit(RPG& pRPG) = 0;
	virtual void visit(Throwable& pThrowable) = 0;
};

struct ShootVisitor : public WeaponVisitor
{
	std::pair<int32_t, bool> getDamage() override;
	void updatePositions(const Vector2f& pCharPos,
						 const Vector2f& pWeaponPos,
						 std::vector<SDL_FRect> pEnemyPos) override;
	void visit(Gun& pGun) override;
	void visit(Melee& pMelee) override;
	void visit(RPG& pRPG) override;
	void visit(Throwable& pThrowable) override;
};

struct ReloadVisitor : public WeaponVisitor
{
	std::pair<int32_t, bool> getDamage() override;
	void updatePositions(const Vector2f& pCharPos,
						 const Vector2f& pWeaponPos,
						 std::vector<SDL_FRect> pEnemyPos) override;
	void visit(Gun& pGun) override;
	void visit(Melee& pMelee) override;
	void visit(RPG& pRPG) override;
	void visit(Throwable& pThrowable) override;
};

class RenderUpdateVisitor : public WeaponVisitor
{
public:
	RenderUpdateVisitor(SDL_Renderer* pRenderer)
		: mRenderer(pRenderer), mCharPos{}, 
		  mWeaponPos{} {}

	std::pair<int32_t, bool> getDamage() override;
	void updatePositions(const Vector2f& pCharPos,
						 const Vector2f& pWeaponPos,
						 std::vector<SDL_FRect> pEnemyPos) override;
	void visit(Gun& pGun) override;
	void visit(Melee& pMelee) override;
	void visit(RPG& pRPG) override;
	void visit(Throwable& pThrowable) override;

private:
	std::pair<int32_t, bool> mDamage;

	SDL_Renderer* mRenderer;

	Vector2f mCharPos;
	Vector2f mWeaponPos;
	std::vector<SDL_FRect> mEnemyPos;
};

