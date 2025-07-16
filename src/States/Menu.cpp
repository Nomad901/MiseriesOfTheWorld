#include "Menu.h"

Menu::Menu(SDL_Renderer* pRenderer, const PATH& pBackGround, const PATH& pMusic)
	:mRenderer{ pRenderer }
{
	mCurrentPath = pBackGround;
	
	mAudio.setMusic(Music::MAIN_MENU, pMusic);
	mAudio.setSound(Sounds::BUTTONS, std::filesystem::current_path() / "Assets" / "music and sounds" / "Hit_Hurt2.wav");
	
	mAudio.setVolumeMusic(Music::MAIN_MENU, 1);
	mAudio.setVolumeSound(Sounds::BUTTONS, 10);

	mFctrObj.appendObject("StoryRect",	    { 100,100,200,100 }, { 0,0,0,255 });
	mFctrObj.appendObject("InfiniteRect",	{ 100,200,200,100 }, { 0,0,0,255 });
	mFctrObj.appendObject("SettingsRect",	{ 100,300,200,100 }, { 0,0,0,255 });
	mFctrObj.appendObject("ExitRect",		{ 100,400,200,100 }, { 0,0,0,255 });
	mFctrObj.appendObject("BackRect",		{ 0,0,100,100 },	 { 0,0,0,255 });
	
	TextureManager::getInstance().appendTexture(pRenderer, pBackGround, mScreenRect);
	TextureManager::getInstance().appendTexture(pRenderer, 
												std::filesystem::current_path() / "Assets" / "photos and ttf" / "workingOnIT.png",
												mScreenRect);
	FactoryOfFonts::init();
	FactoryOfFonts::getInstance().appendNewFonts(pRenderer,
												 std::filesystem::current_path() / "Assets" / "photos and ttf" / "arial.ttf",
												 { "Story mode","Infinite mode", "Settings", "Exit", "Back"},
												 { 141,86,179,255 }, 24);
}

Menu::~Menu()
{
	FactoryOfFonts::shutDown();
	SDL_DestroyRenderer(mRenderer);
}

void Menu::hopIn()
{
	if(!mAudio.isPlaying())
		mAudio.playMusic(Music::MAIN_MENU);
	mActive = true;	
}

void Menu::getOutta()
{
	mAudio.stopMusic(Music::MAIN_MENU);
	mActive = false;
}

void Menu::setOffsetButtons(const uint32_t pOffset)
{
	mOffset = pOffset;
}

void Menu::setPosButtons(std::string_view pName, const Vector2f& pPos)
{
	mFctrObj.setPosition(pName, pPos);
}

//todo: upgrade this function, in order to erase a lotta reduntant code
void Menu::handleEvents(std::weak_ptr<Game> pGame)
{
	if (auto game = pGame.lock())
	{
		mFctrObj.highlightObject("StoryRect",	 { 255, 0, 0, 255 });
		mFctrObj.highlightObject("InfiniteRect", { 255, 0, 0, 255 });
		mFctrObj.highlightObject("SettingsRect", { 255, 0, 0, 255 });
		mFctrObj.highlightObject("ExitRect",	 { 255, 0, 0, 255 });
		mFctrObj.highlightObject("BackRect",	 { 255, 0, 0, 255 });

		if (InputManager::getInstance().isMousePressed(MouseButton::LEFT))
		{
			mAudio.playSound(Sounds::BUTTONS);
			if (mCurrentState == currentState::MENU)
			{ 
				if (InputManager::getInstance().isMouseInside(mFctrObj.getRect("StoryRect")))
					getOutta();
				if (InputManager::getInstance().isMouseInside(mFctrObj.getRect("InfiniteRect")))
					mCurrentState = currentState::WORKINGONIT;
				if (InputManager::getInstance().isMouseInside(mFctrObj.getRect("SettingsRect")))
					mCurrentState = currentState::SETTINGS;
				if (InputManager::getInstance().isMouseInside(mFctrObj.getRect("ExitRect")))
					game->stopLoop();
			}
			else if(mCurrentState == currentState::SETTINGS)
			{
			
			}
			if (InputManager::getInstance().isMouseInside(mFctrObj.getRect("BackRect")))
				mCurrentState = currentState::MENU;
			
		}
	}
	else
		LOG("Couldnt lock the pointer!");
}

void Menu::render()
{
	if (mActive)
	{
		if(mCurrentState == currentState::WORKINGONIT)
			SDL_RenderCopy(mRenderer, TextureManager::getInstance().getTexture(std::filesystem::current_path() / "Assets" / "photos and ttf" / "workingOnIT.png"), 
						   nullptr, &mScreenRect);
		else
			SDL_RenderCopy(mRenderer, TextureManager::getInstance().getTexture(mCurrentPath), nullptr, &mScreenRect);
		
		if (mCurrentState == currentState::MENU)
			menuState();
		else if (mCurrentState == currentState::SETTINGS)
			settingsState();
		else if (mCurrentState == currentState::WORKINGONIT)
			workingState();
	}
}

bool Menu::isActive() const noexcept
{
	return mActive;
}

void Menu::menuState()
{
	FactoryOfFonts::getInstance().render("Story mode", mRenderer, mFctrObj.getRect("StoryRect"));
	FactoryOfFonts::getInstance().render("Infinite mode", mRenderer, mFctrObj.getRect("InfiniteRect"));
	FactoryOfFonts::getInstance().render("Settings", mRenderer, mFctrObj.getRect("SettingsRect"));
	FactoryOfFonts::getInstance().render("Exit", mRenderer, mFctrObj.getRect("ExitRect"));
	mFctrObj.render("StoryRect", mRenderer, false);
	mFctrObj.render("InfiniteRect", mRenderer, false);
	mFctrObj.render("SettingsRect", mRenderer, false);
	mFctrObj.render("ExitRect", mRenderer, false);
}

void Menu::settingsState()
{
	FactoryOfFonts::getInstance().render("Back", mRenderer, mFctrObj.getRect("BackRect"));
	mFctrObj.render("BackRect", mRenderer, false);
}

void Menu::workingState()
{
	FactoryOfFonts::getInstance().render("Back", mRenderer, mFctrObj.getRect("BackRect"));
	mFctrObj.render("BackRect", mRenderer, false);
}

