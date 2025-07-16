#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>

#include "Game/Game.h"
#include "Game objets/FactoryOfEntities.h"
#include "Game/Assets.h"
#include "Game/InputManager.h"

class TextureManager : protected Assets
{
public:
	static void init();
	static TextureManager& getInstance();
	static void shutDown();

	void appendTexture(SDL_Renderer* pRenderer, const PATH& pPath,
					   SDL_Rect pRect);
	void appendTextureWithoutBack(SDL_Renderer* pRenderer, const PATH& pPath, 
								  SDL_Rect pRect, SDL_Color pColor);
	void deleteTexture(const PATH& pPath);
	void deleteBackground(SDL_Renderer* pRenderer, SDL_Texture*& pTexture);
	void deleteBackground(SDL_Surface*& pSurface, SDL_Color pColor);
	
	bool colorsEqual(SDL_Color pFirstColor, SDL_Color pSecondColor);

	void setDimensions(const PATH& pPath, const int pW, const int pH);
	void setPosition(const PATH& pPath, const Vector2f& pPosition);
	SDL_Rect convertToSDLRect(const Vector2f& pPos1, const Vector2f& pPos2);

	bool containsPath(const PATH& pPath);

	Vector2f getPosition(const PATH& pPath);
	SDL_Color getBackGroundColor(SDL_Surface* pSurface, const Vector2f& pPosition);
	SDL_Color getBackGroundColor(const PATH& pPath, const Vector2f& pPosition);
	SDL_Color getBackGroundColor(const PATH& pPath);
	SDL_Color getBackGroundColor(SDL_Surface* pSurface);
	SDL_Rect& getRect(const PATH& pPath);
	SDL_Texture* getTexture(const PATH& pPath);
	SDL_Surface* getSurface(const PATH& pPath);

	
	TextureManager(const TextureManager&) = delete;
	TextureManager operator=(const TextureManager&) = delete;
	TextureManager(TextureManager&&) = delete;
	TextureManager operator=(TextureManager&&) = delete;
protected:
	TextureManager() = default;
	~TextureManager() = default;

private:
	static TextureManager* mInstance;
};

