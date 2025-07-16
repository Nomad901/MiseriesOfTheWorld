#pragma once
#include "GameStates.h"
#include "Game/InputManager.h"
#include "Game objets/FactoryOfFonts.h"
#include "Math/Vector2f.h"
#include "Game objets/FactoryObjects.h"
#include "Sound/AudioSystem.h"

//extensible
enum class currentState
{
	MENU = 0,
	SETTINGS = 1,
	EXIT = 2,
	WORKINGONIT = 3
};

class Menu : public GameStates
{
public:
	Menu(SDL_Renderer* pRenderer, const PATH& pBackGround, const PATH& pMusic);
	~Menu();

	void hopIn() override;
	void getOutta() override;

	void setOffsetButtons(const uint32_t pOffset);
	void setPosButtons(std::string_view pName, const Vector2f& pPos);

	void handleEvents(std::weak_ptr<Game> pGame) override;

	void render() override;
	void update(){}

	bool isActive() const noexcept;

private:
	//extensible
	void menuState();
	void settingsState();
	void workingState();

private:
	bool mActive{ false };
	
	uint32_t mOffset{ 0 };
	
	SDL_Rect mDstRect{ 100,100,100,100 };
	SDL_Rect mScreenRect = SCREEN_RECT;
	
	currentState mCurrentState{ currentState::MENU };

	SDL_Renderer* mRenderer{ nullptr };
	
	FactoryObjects mFctrObj;
	AudioSystem mAudio;

	PATH mCurrentPath{ std::filesystem::current_path() };	
};

