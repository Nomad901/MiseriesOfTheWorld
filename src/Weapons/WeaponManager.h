#pragma once
#include <vector>
#include <string>

#include "Weapons/Bullets.h"
#include "Game objets/FactoryObjects.h"
#include "Weapons/Rifle.h"
#include "Weapons/Pistol.h"
#include "Weapons/Boulder.h"
#include "Weapons/Knife.h"
#include "Weapons/Shotgun.h"
#include "Weapons/Tango.h"

class Weapon;

template<typename T>
concept properWeapon = std::is_base_of_v<Weapon, T>;

class WeaponManager 
{
public:
	WeaponManager() = default;
	
	auto initWeapons(SDL_Renderer* pRenderer, SDL_FRect pCharRect) -> void;
	auto appendWeapon(std::unique_ptr<Weapon> pWeapon, std::string_view pName) -> void;
	template<typename T, typename... Args>
		requires properWeapon<T>
	auto createWeapon(std::string_view pName, Args&&... pArgs) -> void;
	auto removeWeapon(std::string_view pName) -> void;

	auto getWeapon(std::string_view pName) -> Weapon*;
	template<typename T>
		requires properWeapon<T>
	auto getExactWeapon(std::string_view pName) -> std::expected<std::reference_wrapper<T>, std::string_view>;

	auto assignBullets(std::string_view pName, std::unique_ptr<Bullets> pBullets) -> void;
	auto getBullets(std::string_view pName) -> Bullets*;

	auto throwWeapon(std::string_view pName) -> void;
	auto takeWeapon(std::string_view pName)  -> void;
	auto weaponIsGriped()				     -> std::expected<std::vector<std::string>, std::string_view>;
	auto weaponIsGripedBy(SDL_FRect pRect)   -> std::expected<std::string, std::string_view>;
	auto weaponIsInView(SDL_Rect pCharRect)	 -> std::expected<std::vector<std::string>, std::string_view>;

private:
	struct WeaponRelationship
	{
		std::unique_ptr<Weapon> mWeapon;
		std::unique_ptr<Bullets> mBullets;
	} mWR;

	std::unordered_map<std::string, WeaponRelationship> mStorageWeapons;
	FactoryObjects mFactoryObjects;
};

template<typename T, typename ...Args>
	requires properWeapon<T>
inline void WeaponManager::createWeapon(std::string_view pName, Args&&... pArgs)
{
	appendWeapon(std::make_unique<T>(std::forward<Args>(pArgs)...), pName);
}

template<typename T>
	requires properWeapon<T>
inline auto WeaponManager::getExactWeapon(std::string_view pName) -> std::expected<std::reference_wrapper<T>, std::string_view>
{
	auto it = mStorageWeapons.find(std::string(pName));
	if (it != mStorageWeapons.end())
	{
		Weapon* tmpWeapon = it->second.mWeapon.get();
		if (T* exactWeapon = dynamic_cast<T*>(tmpWeapon))
			return std::ref(*exactWeapon);
	}
	LOG("There is no such a name!\n");
	return std::unexpected("Smth is wrong!");
}
