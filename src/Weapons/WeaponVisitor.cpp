#include "Weapons/WeaponVisitor.h"
#include "Weapons/Gun.h"
#include "Weapons/Melee.h"
#include "Weapons/RPG.h"
#include "Weapons/Throwable.h"

std::pair<int32_t, bool> ShootVisitor::getDamage()
{
	return std::pair<int32_t, bool>();
}

void ShootVisitor::updatePositions(const Vector2f& pCharPos,
								   const Vector2f& pWeaponPos,
								   std::vector<SDL_FRect> pEnemyPos)
{
}

// ---------------Shoot-----------------
void ShootVisitor::visit(Gun& pGun)
{
	pGun.shoot();
}

void ShootVisitor::visit(Melee& pMelee)
{
	pMelee.attack();
}

void ShootVisitor::visit(RPG& pRPG)
{

}

void ShootVisitor::visit(Throwable& pThrowable)
{
	pThrowable.shoot();
}
// --------------------------------------

std::pair<int32_t, bool> ReloadVisitor::getDamage()
{
	return std::pair<int32_t, bool>();
}

void ReloadVisitor::updatePositions(const Vector2f& pCharPos, 
									const Vector2f& pWeaponPos,
									std::vector<SDL_FRect> pEnemyPos)
{
}


// ---------------Reload-----------------
void ReloadVisitor::visit(Gun& pGun)
{
	pGun.reload();
}

void ReloadVisitor::visit(Melee& pMelee)
{
}
void ReloadVisitor::visit(RPG& pRPG)
{
}
void ReloadVisitor::visit(Throwable& pThrowable)
{
}
// --------------------------------------

std::pair<int32_t, bool> RenderUpdateVisitor::getDamage()
{
	return mDamage;
}

// ---------------Render-----------------
void RenderUpdateVisitor::updatePositions(const Vector2f& pCharPos,
										  const Vector2f& pWeaponPos,
										  std::vector<SDL_FRect> pEnemyPos)
{
	mCharPos = pCharPos;
	mWeaponPos = pWeaponPos;
	mEnemyPos = pEnemyPos;
}

void RenderUpdateVisitor::visit(Gun& pGun)
{
	pGun.render(mRenderer);
	pGun.updatePositions(mCharPos, mWeaponPos);
	pGun.updateBullets(mRenderer);
	pGun.update(mWeaponPos);
	for (auto& i : mEnemyPos)
	{
		mDamage = pGun.manageDamage(i);
	}
}

void RenderUpdateVisitor::visit(Melee& pMelee)
{
	pMelee.render(mRenderer);
	pMelee.updatePositions(mCharPos, mWeaponPos);
	pMelee.update(mWeaponPos);
	for (auto& i : mEnemyPos)
	{
		mDamage = pMelee.manageDamage(i);
	}
}

void RenderUpdateVisitor::visit(RPG& pRPG)
{

}

void RenderUpdateVisitor::visit(Throwable& pThrowable)
{
	pThrowable.render(mRenderer);
	pThrowable.updatePositions(mCharPos, mWeaponPos);
	pThrowable.updateBullets(mRenderer);
	pThrowable.update(mWeaponPos);
	for (auto& i : mEnemyPos)
	{
		mDamage = pThrowable.manageDamage(i);
	}
}

// --------------------------------------