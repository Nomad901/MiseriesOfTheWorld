#include "Game objets/FactoryOfEntities.h"
#include "Weapons/Weapon.h"
#include "Game objets/FactoryObjects.h"
#include "Weapons/WeaponManager.h"

FactoryOfEntities::FactoryOfEntities()
{
	mWeaponManager = std::make_unique<WeaponManager>();
}

void FactoryOfEntities::setName(std::string_view pName)
{
	mName = pName;
}

std::string FactoryOfEntities::getName() const noexcept
{
	return mName;
}

void FactoryOfEntities::setVisibility(bool pVisibility)
{
	mVisible = pVisibility;
}

bool FactoryOfEntities::isVisible() const noexcept
{
	return mVisible;
}

bool FactoryOfEntities::isGripping(std::string_view pName)
{
	mWeaponManager->setCurrentName(pName);
	return mWeaponManager->getWeapon()->getWeaponStates().mIsFreezed;
}

bool FactoryOfEntities::isGripping(std::unique_ptr<FactoryObjects> pObject)
{
	return pObject == mObject ? true : false;
}

bool FactoryOfEntities::isGripping() const noexcept
{
	return mIsGripping;
}

bool FactoryOfEntities::isAlive() const noexcept
{
	return mIsAlive;
}

void FactoryOfEntities::makeAlive(bool pAlive)
{
	mIsAlive = pAlive;
}

int FactoryOfEntities::getHealth() const noexcept
{
	return mHealth;
}

void FactoryOfEntities::setHealth(int pHealth)
{
	mHealth = pHealth;
}

int FactoryOfEntities::getLives() const noexcept
{
	return mLives;
}

void FactoryOfEntities::setLives(int pLives)
{
	mLives = pLives;
}

float FactoryOfEntities::getSpeed() const noexcept
{
	return mSpeed;
}

void FactoryOfEntities::setSpeed(const float pSpeed)
{
	mSpeed = pSpeed;
}

Vector2f FactoryOfEntities::getPos() const noexcept
{
	return mPosition;
}	

void FactoryOfEntities::setPos(Vector2f pPos)
{
	mPosition = pPos;
}

void FactoryOfEntities::makeParent(std::unique_ptr<FactoryOfEntities> pParent)
{
	mParent = std::move(pParent);
}

void FactoryOfEntities::takeObject(std::unique_ptr<FactoryObjects> pObject)
{
	if (pObject == nullptr)
	{
		mIsGripping = false;
		mObject = nullptr;
	}
	else
	{
		mIsGripping = true;
		mObject = std::move(pObject);	
	}
}
