#include "WeaponManager.h"
#include "Weapon.h"
#include "Bullets.h"

void WeaponManager::initWeapons(SDL_Renderer* pRenderer, SDL_FRect pCharRect)
{
	createWeapon<Knife>("Knife");
	createWeapon<Boulder>("Boulder");
	createWeapon<Tango>("Tango");
	createWeapon<Shotgun>("Shotgun");
	createWeapon<Rifle>("Rifle");
	createWeapon<Pistol>("Pistol");

	getExactWeapon<Knife>("Knife")->get().initKnifeAutomatically(pRenderer, pCharRect);
	getExactWeapon<Boulder>("Boulder")->get().initBoulderAutomatically(pRenderer, pCharRect);
	getExactWeapon<Tango>("Tango")->get().initTangoAutomaticaly(pRenderer, pCharRect);
	getExactWeapon<Shotgun>("Shotgun")->get().initShotgunAutomaticaly(pRenderer, pCharRect);
	getExactWeapon<Rifle>("Rifle")->get().initRifleAutomaticaly(pRenderer, RifleType::IRREGULAR, pCharRect);
	getExactWeapon<Pistol>("Pistol")->get().initPistolAutomaticaly(pRenderer, PistolType::FAST, pCharRect);
}

void WeaponManager::appendWeapon(std::unique_ptr<Weapon> pWeapon, std::string_view pName)
{
	WeaponRelationship tmpWR;
	tmpWR.mWeapon  = std::move(pWeapon);
	tmpWR.mBullets = nullptr;
	mStorageWeapons.insert_or_assign(std::string(pName), std::move(tmpWR));
}

void WeaponManager::removeWeapon(std::string_view pName)
{
	auto it = mStorageWeapons.find(std::string(pName));
	if (it != mStorageWeapons.end())
		mStorageWeapons.erase(it);
	else
		LOG("There is no such a name!");
}

Weapon* WeaponManager::getWeapon(std::string_view pName)
{
	if (mStorageWeapons.contains(std::string(pName)))
		return mStorageWeapons[std::string(pName)].mWeapon.get();
	LOG("There is no such a name!");
	return nullptr;
}

void WeaponManager::assignBullets(std::string_view pName, std::unique_ptr<Bullets> pBullets)
{
	if (mStorageWeapons.contains(std::string(pName)))
		mStorageWeapons[std::string(pName)].mBullets = std::move(pBullets);
	else
		LOG("There is no such a name!");
}

Bullets* WeaponManager::getBullets(std::string_view pName)
{
	if (mStorageWeapons.contains(std::string(pName)))
		return mStorageWeapons[std::string(pName)].mBullets.get();
	LOG("There is no such a name!");
	return nullptr;
}

void WeaponManager::throwWeapon(std::string_view pName)
{
	if (!mStorageWeapons.contains(std::string(pName)))
	{
		LOG("There is no such a name!\n");
		return;
	}
	mStorageWeapons[std::string(pName)].mWeapon->makeFreezed(true);
}

void WeaponManager::takeWeapon(std::string_view pName)
{
	if (!mStorageWeapons.contains(std::string(pName)))
	{
		LOG("There is no such a name!\n");
		return;
	}
	SDL_FRect tmpRect = mStorageWeapons[std::string(pName)].mWeapon->getCharCollisions().mWeapon;
	if(mStorageWeapons[std::string(pName)].mWeapon->WeaponIsInView(mFactoryObjects.convertFRect(tmpRect)))
		mStorageWeapons[std::string(pName)].mWeapon->makeFreezed(false);
}

auto WeaponManager::weaponIsGriped() ->std::expected<std::vector<std::string>, std::string_view>
{
	std::vector<std::string> tmpVctr;
	tmpVctr.reserve(mStorageWeapons.size());
	for (auto& [key, value] : mStorageWeapons)
	{
		if (!value.mWeapon->getWeaponStates().mIsFreezed)
			tmpVctr.emplace_back(key);
	}
	return tmpVctr.empty()
		 ? std::unexpected("There is no weapon in hold!\n")
		 : std::expected<std::vector<std::string>, std::string_view>(tmpVctr);
}

auto WeaponManager::weaponIsGripedBy(SDL_FRect pRect) -> std::expected<std::string, std::string_view>
{
	auto tmpVar = weaponIsGriped();
	if (!tmpVar.has_value())
		return std::unexpected("the weapons are not griped!\n");

	for (auto& i : tmpVar.value())
	{
		if (mFactoryObjects.areColliding(mFactoryObjects.convertFRect(mStorageWeapons[i].mWeapon->getCharCollisions().mWeapon),
									     mFactoryObjects.convertFRect(mStorageWeapons[i].mWeapon->getCharCollisions().mChar)))
		{
			return i;
		}
	}
	return std::unexpected("The character does not grip the weapon!\n");
}

auto WeaponManager::weaponIsInView(SDL_Rect pCharRect) -> std::expected<std::vector<std::string>, std::string_view>
{
	std::vector<std::string> tmpVctr;
	for (auto& [key, value] : mStorageWeapons)
	{
		if (value.mWeapon->WeaponIsInView(pCharRect))
			tmpVctr.emplace_back(key);
	}
	return tmpVctr.empty()
		 ? std::unexpected("None of the weapons in view!\n")
		 : std::expected<std::vector<std::string>, std::string_view>(tmpVctr);
}


