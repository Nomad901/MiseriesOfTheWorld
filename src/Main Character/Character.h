#pragma once
#include <SDL.h>

#include "Loger/Loging.h"
#include "Game objets/FactoryOfEntities.h"
#include "Game/Assets.h"
#include "Time/Timer.h"
#include "Game/TextureManager.h"
#include "Sound/AudioSystem.h"
#include "Game/InputManager.h"
#include "Game/AnimatedTexture.h"
#include "Weapons/Weapon.h"
#include "Physics/SmoothMoving.h"

class Character : public FactoryOfEntities
{
public:
	Character(SDL_Renderer* pRenderer, const PATH& pPath, const Vector2f& pPos, std::string_view pName, 
			  std::unique_ptr<Weapon> pWeapon = nullptr, const int pLives = 3, const float pSpeed = 5);
	~Character();

	
	void render() override;
	void update() override;

private:
	SDL_Renderer* mRenderer;
	
};

// then for weapons i will make a system of changing. like. 
// weapon - main class, pistol - will be class which i will inherit. 
// then i can pass this into character class