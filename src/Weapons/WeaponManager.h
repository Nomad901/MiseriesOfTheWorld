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

enum class WeaponType_
{
	RIFLE = 0,
	PISTOL = 1,
	BOULDER = 2,
	KNIFE = 3,
	SHOTGUN = 4,
	TANGO = 5
};

class Weapon;

template<typename T>
concept properWeapon = std::is_base_of_v<Weapon, T>;

class WeaponManager 
{
public:
	WeaponManager() = default;
	
	//
	// automatically initializes all needed weapons with names and so on;
	//
	auto initWeapons(SDL_Renderer* pRenderer, SDL_FRect pCharRect) -> void;

	//
	// directly appends a weapon into the storage;
	//
	auto appendWeapon(std::unique_ptr<Weapon> pWeapon, std::string_view pName) -> void;

	//
	// creates an instance of a class with parameters, which we can pass, but the class is determined by
	// template type; 
	//
	template<typename T, typename... Args>
		requires properWeapon<T>
	auto createWeapon(std::string_view pName, Args&&... pArgs) -> void;

	//
	// deletes a weapon from the storage of weapons;
	//
	auto removeWeapon(std::string_view pName) -> void;

	//
	// returns weapon with the current name of this class;
	//
	auto getWeapon() -> Weapon*;

	//
	// returns an exact weapon class, which is determined by the passed name and a template type; 
	//
	template<typename T>
		requires properWeapon<T>
	auto getExactWeapon(std::string_view pName) -> std::expected<std::reference_wrapper<T>, std::string_view>;

	//
	// appends bullets 
	//
	auto assignBullets(std::string_view pName, std::unique_ptr<Bullets> pBullets) -> void;

	//
	// returns quantity of left bullets;
	//
	auto getBullets(std::string_view pName) -> Bullets*;

	//
	// throw weapon away. just make a weapon, with the name, freezed and thats it,
	// in order to unfreeze this - just need to push some buttons nearby the weapon;
	//
	auto throwWeapon(std::string_view pName) -> void;

	//
	// correlates a weapon with a rectangle (character), so that
	// the character can bring this, shoot from that and so on;
	//
	auto takeWeapon(SDL_Rect pRect, std::string_view pName)  -> void;

	//
	// checks if weapon with a name is thrown or not (returns bool);
	//
	auto weaponIsThrown(std::string_view pName) -> bool;

	//
	// checks all weapons. if at least one of them is griped - then this function is valid and
	// it will return the storage with names of this weapons;
	//
	auto weaponIsGriped() ->std::expected<std::vector<std::string>, std::string_view>;
	//
	// returns a name of a weapon, which is griped currently by the rect;
	//
	auto weaponIsGripedBy(SDL_FRect pRect) -> std::expected<std::string, std::string_view>;
	//
	// checks an exact weapon, which might be griped right now
	//
	auto weaponIsGriped(std::string_view pName) -> bool;
	//
	// returns true or false if the weapon with this name is griped by the passed character; 
	//
	auto weaponIsGripedBy(std::string_view pName, SDL_FRect pRect) -> bool;

	//
	// checks if a weapon is in a view of the character;
	//
	auto weaponIsInView(SDL_Rect pCharRect)	-> std::expected<std::vector<std::string>, std::string_view>;

	//
	// optional: sets current weapon type for the class, in order to use just usual functions,
	// without needs to write an exact type;
	//
	auto setCurrentWeaponType(WeaponType_ pWeaponType) -> void;
	//
	// the same thing like the function setCurrentWeaponType, but works with names;
	//
	auto setCurrentName(std::string_view pName) -> void;

	//
	// some getters for getting weapon current type and weapon current name;
	//
	// ------------------------------------
	auto getCurrWeaponType() -> WeaponType_;
	auto getCurrName() -> std::string;
	// ------------------------------------

private:
	struct WeaponRelationship
	{
		std::unique_ptr<Weapon> mWeapon;
		std::unique_ptr<Bullets> mBullets;
	} mWR;
	WeaponType_ mCurrWeaponType{ WeaponType_::RIFLE };
	std::string mCurrNameWeapon{ "Rifle" };

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
