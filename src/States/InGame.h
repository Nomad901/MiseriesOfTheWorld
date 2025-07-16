#pragma once
#include <set>

#include "States/GameStates.h"
#include "Game/InputManager.h"
#include "Game objets/FactoryOfFonts.h"
#include "Math/Vector2f.h"
#include "Scenes/LevelsScenes.h"
#include "Game/Randomizer.h"
#include "Main Character/Character.h"

enum class levels
{
	LEVEL0 = 0,
	LEVEL1 = 1,
	LEVEL2 = 2,
	LEVEL3 = 3,
};

class InGame : public GameStates
{
public:
	InGame(SDL_Renderer* pRenderer, const PATH& pScenePath, const PATH& pPathMusic,
		   Character* pCharacter);
	~InGame();

	void hopIn() override;
	void getOutta() override;

	void handleEvents(std::weak_ptr<Game> pGame) override;

	void render() override;
	void update() override;

	bool isActive();
private:

	bool isInTheRange(const Vector2f& pPosEntity, const SidesDoor pSides);
	
private:
	bool mActive{ false };
	std::string mCurrentScene{ "" };
	levels mCurrentLevel{ levels::LEVEL0 };

	LevelsScenes mLvlScenes;
	Randomizer mRandomizer{ 0,1 };
	AudioSystem mAudio;
	Character* mCharacter;
	std::vector<std::string> mScenes;
	std::vector<uint16_t> mBuffer;

	SDL_Renderer* mRenderer{ nullptr };
	PATH mCurrentPath{ std::filesystem::current_path() };
};
