#pragma once
#include <filesystem>
#include <unordered_map>
#include <fstream>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Loger/Loging.h"
#include "Game/Singleton.h"
#include "Game/JSONSystem.h"

using PATH = std::filesystem::path;

class TextureManager;
class FactoryOfFonts;
class FireMode;

class Assets : public Singleton<Assets>
{
public:
	~Assets() override;
	auto setRenderer(SDL_Renderer* pRenderer) -> void;
	
	//
	//if wanna download at once all of this
	//
	auto load() -> void;

	auto getAnimation(const PATH& pPath)		 -> std::unordered_map<int, std::pair<SDL_Texture*, SDL_Rect>>;
	auto getTexture(const PATH& pPath)			 -> SDL_Texture*;
	auto getSurface(const PATH& pPath)			 -> SDL_Surface*;
	auto getFonts(std::string_view pAlias)		 -> TTF_Font*;
	auto getTextureFont(std::string_view pAlias) -> SDL_Texture*;
	auto getSound(const PATH& pPath)			 -> Mix_Chunk*;
	auto getMusic(const PATH& pPath)			 -> Mix_Music*;
	auto getScene(std::string_view pScene)		 -> PATH;

protected:

	std::unordered_map<PATH, std::unordered_map<int, std::pair<SDL_Texture*, SDL_Rect>>> mAnimation;
	std::unordered_map<PATH, std::pair<std::pair<SDL_Surface*, SDL_Texture*>, SDL_Rect>> mTextures;
	std::unordered_map<std::string, std::pair<TTF_Font*, SDL_Texture*>> mFonts;
	std::unordered_map<PATH, Mix_Chunk*> mSounds;
	std::unordered_map<PATH, Mix_Music*> mMusic;
	std::unordered_map<std::string, PATH> mScenes;

private:
	SDL_Renderer* mRenderer{ nullptr };
	PATH mPath{ PROJECT_ROOT };
	
	std::array<JSONSystem, 7> mAssets{
		mPath / "Assets" / "json" / "mAnimation.json",
		mPath / "Assets" / "json" / "mTextures.json",
		mPath / "Assets" / "json" / "mSurfaces.json",
		mPath / "Assets" / "json" / "mFonts.json",
		mPath / "Assets" / "json" / "mSounds.json",
		mPath / "Assets" / "json" / "mMusic.json",
		mPath / "Assets" / "json" / "mScenes.json"
	};
	
	friend class Singleton<Assets>;
};

