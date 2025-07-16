#pragma once
#include <filesystem>
#include <vector>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Loger/Loging.h"
#include "Game/Assets.h"
#include "Game/InputManager.h"

class FactoryOfFonts : protected Assets
{
public:
	static void init();
	static FactoryOfFonts& getInstance();
	static void shutDown();

	void appendNewFont(SDL_Renderer* pRenderer, const PATH& pPath, std::string_view pText, 
					   const SDL_Color& pColor, const int pSize);
	void appendNewFonts(SDL_Renderer* pRenderer, const PATH& pPath, std::initializer_list<std::string> pTexts,
						const SDL_Color& pColor, const int pSize);
	void changeText(SDL_Renderer* pRenderer, std::string_view pOrigText, std::string_view pText, SDL_Color pColor);
	void deleteFont(std::string_view pAlias);

	void fontIntoRect(std::string_view pText, SDL_Rect& pRect);

	TTF_Font* getFont(std::string_view pAlias);
	SDL_Texture* getTexture(std::string_view pAlias);
	
	void render(std::string_view pName, SDL_Renderer* pRenderer, SDL_Rect pRect);

private:
	FactoryOfFonts() = default;
	~FactoryOfFonts() = default;
	FactoryOfFonts(const FactoryOfFonts&) = delete;
	FactoryOfFonts& operator=(const FactoryOfFonts&) = delete;
	FactoryOfFonts(FactoryOfFonts&&) = delete;
	FactoryOfFonts& operator=(FactoryOfFonts&&) = delete;

private:
	SDL_Color mColor;
	int mSize;
	std::vector<std::string> mText;

	PATH mPath{ std::filesystem::current_path() };

	static FactoryOfFonts* mInstance;
};

