#include "Weapons/WeaponVisitor.h"
#include "Weapons/Gun.h"
#include "Weapons/Melee.h"
#include "Weapons/RPG.h"
#include "Weapons/Throwable.h"

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

// ---------------Reload-----------------
void ReloadVisitor::visit(Gun& pGun)
{
	pGun.reload();
}

void ReloadVisitor::visit(RPG& pRPG)
{

}
// --------------------------------------

// ---------------Render-----------------
void RenderUpdateVisitor::updatePositions(const Vector2f& pCharPos, const Vector2f& pWeaponPos)
{
	mCharPos = pCharPos;
	mWeaponPos = pWeaponPos;
}

void RenderUpdateVisitor::visit(Gun& pGun)
{
	pGun.render(mRenderer);
	pGun.updatePositions(mCharPos, mWeaponPos);
	pGun.update(mWeaponPos);
}

void RenderUpdateVisitor::visit(Melee& pMelee)
{
	pMelee.render(mRenderer);
	pMelee.updatePositions(mCharPos, mWeaponPos);
	pMelee.update(mWeaponPos);
}

void RenderUpdateVisitor::visit(RPG& pRPG)
{

}

void RenderUpdateVisitor::visit(Throwable& pThrowable)
{
	pThrowable.render(mRenderer);
	pThrowable.updatePositions(mCharPos, mWeaponPos);
	pThrowable.update(mWeaponPos);
}

// --------------------------------------