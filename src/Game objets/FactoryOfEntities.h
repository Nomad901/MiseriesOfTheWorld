#pragma once
#include <string>
#include <memory>
#include <vector>

#include "Math/Vector2f.h"
#include "FactoryObjects.h"

class WeaponManager;
class Weapon;
class FactoryObjects;

class FactoryOfEntities
{
public:
	FactoryOfEntities();
	virtual ~FactoryOfEntities() = default;

	void setName(std::string_view pName);
	inline std::string getName() const noexcept;

	void setVisibility(bool pVisibility);
	bool isVisible() const noexcept;

	bool isGripping(std::string_view pName);
	bool isGripping(std::unique_ptr<FactoryObjects> pObject);
	bool isGripping() const noexcept;

	bool isAlive() const noexcept;
	void makeAlive(bool pAlive);

	int getHealth() const noexcept;
	void setHealth(int pHealth);

	int getLives() const noexcept;
	void setLives(int pLives);

	float getSpeed() const noexcept;
	void setSpeed(const float pSpeed);

	Vector2f getPos() const noexcept;
	void setPos(Vector2f pPos);

	//in order to throw this away - just make this nullptr and 
	//the object will be frozen on the same spot where the character
	void makeParent(std::unique_ptr<FactoryOfEntities> pParent);
	void takeObject(std::unique_ptr<FactoryObjects> pObject);
	//void takeWeapon(std::unique_ptr<Weapon> pWeapon);
	
	virtual void render() = 0;
	virtual void update() = 0;

private:
	bool mVisible{ false };
	bool mIsAlive{ false };
	bool mIsGripping{ false };

	int mHealth{ 0 };
	int mLives{ 0 };

	float mSpeed{ 0.0f };

	std::string mName{ "" };

	std::unique_ptr<FactoryOfEntities> mParent{ nullptr };
	std::unique_ptr<FactoryObjects> mObject{ nullptr };

	std::unique_ptr<WeaponManager> mWeaponManager;

	Vector2f mPosition{ 0.0f, 0.0f };
};


