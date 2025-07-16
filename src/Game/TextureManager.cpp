#include "TextureManager.h"

TextureManager* TextureManager::mInstance = nullptr;

void TextureManager::init()
{
	if (mInstance != nullptr)
		shutDown();
	mInstance = new TextureManager();
}

TextureManager& TextureManager::getInstance()
{
	return *mInstance;
}

void TextureManager::shutDown()
{
	delete mInstance;
	mInstance = nullptr;
}

void TextureManager::appendTexture(SDL_Renderer* pRenderer, const PATH& pPath, SDL_Rect pRect)
{
	SDL_Rect mRect = pRect;
	SDL_Surface* surface = IMG_Load(pPath.string().c_str());
	if (!surface)
		LOG("Cant create a surface");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, surface);
	if (!texture)
		LOG("Cant create a texture");
	mTextures.insert_or_assign(pPath, std::make_pair(std::make_pair(surface,texture), mRect));	
}

void TextureManager::appendTextureWithoutBack(SDL_Renderer* pRenderer, const PATH& pPath, SDL_Rect pRect, SDL_Color pColor)
{
	SDL_Rect mRect = pRect;
	SDL_Surface* mSurface = IMG_Load(pPath.string().c_str());
	if (!mSurface)
		LOG("Cant create a surface");
	SDL_SetColorKey(mSurface, SDL_TRUE, SDL_MapRGB(mSurface->format, pColor.r, pColor.g, pColor.b));
	if (!mSurface)
		LOG("Cant create a surface");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, mSurface);
	if (!texture)
		LOG("Cant create a texture");
	mTextures.insert_or_assign(pPath, std::make_pair(std::make_pair(mSurface, texture), mRect));
}

void TextureManager::deleteTexture(const PATH& pPath)
{
	if (auto it = mTextures.find(pPath); it != mTextures.end()) {
		SDL_DestroyTexture(it->second.first.second);
		SDL_FreeSurface(it->second.first.first);
		mTextures.erase(it);
	}
	else 
		LOG("Theres is no such a path!");
}

void TextureManager::deleteBackground(SDL_Renderer* pRenderer, SDL_Texture*& pTexture)
{
	int width, height;
	SDL_QueryTexture(pTexture, nullptr, nullptr, &width, &height);

	SDL_Surface* tmpSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
	if (!tmpSurface)
	{
		LOG("couldnt create a surface!");
		return;
	}

	SDL_SetRenderTarget(pRenderer, pTexture);
	SDL_RenderReadPixels(pRenderer, nullptr, tmpSurface->format->format, tmpSurface->pixels, tmpSurface->pitch);
	SDL_SetRenderTarget(pRenderer, nullptr);

	deleteBackground(tmpSurface, getBackGroundColor(tmpSurface));
	
	SDL_DestroyTexture(pTexture);
	pTexture = SDL_CreateTextureFromSurface(pRenderer, tmpSurface);
	if (!pTexture)
	{
		LOG("couldnt create a texture!");
		return;
	}
	SDL_FreeSurface(tmpSurface);
}

void TextureManager::deleteBackground(SDL_Surface*& pSurface, SDL_Color pColor)
{
	SDL_SetColorKey(pSurface, SDL_TRUE, SDL_MapRGB(pSurface->format, pColor.r, pColor.g, pColor.b));
}

void TextureManager::setDimensions(const PATH& pPath, const int pW, const int pH)
{
	if (mTextures.find(pPath) == mTextures.end())
		LOG("There is no such a part!");
	else
	{
		mTextures[pPath].second.w = pW;
		mTextures[pPath].second.h = pH;
	}
}

void TextureManager::setPosition(const PATH& pPath, const Vector2f& pPosition)
{
	if (mTextures.find(pPath) == mTextures.end())
		LOG("There is no such a part!");
	else
	{
		mTextures[pPath].second.x = static_cast<int>(pPosition.mX);
		mTextures[pPath].second.y = static_cast<int>(pPosition.mY);
	}
}

SDL_Rect TextureManager::convertToSDLRect(const Vector2f& pPos1, const Vector2f& pPos2)
{
	return {
		static_cast<int>(pPos1.mX),
		static_cast<int>(pPos1.mY),
		static_cast<int>(pPos2.mX - pPos1.mX),
		static_cast<int>(pPos2.mY - pPos1.mY)
	};
}

bool TextureManager::containsPath(const PATH& pPath)
{
	return mTextures.contains(pPath);
}

bool TextureManager::colorsEqual(SDL_Color pFirstColor, SDL_Color pSecondColor)
{
	return (pFirstColor.r == pSecondColor.r && 
			pFirstColor.g == pSecondColor.g && 
			pFirstColor.b == pSecondColor.b &&
			pFirstColor.a == pSecondColor.a);
}

Vector2f TextureManager::getPosition(const PATH& pPath)
{
	if (mTextures.find(pPath) == mTextures.end())
		LOG("There is no such a part!");
	else
		return { static_cast<float>(mTextures[pPath].second.x),static_cast<float>(mTextures[pPath].second.y) };
}

SDL_Color TextureManager::getBackGroundColor(SDL_Surface* pSurface, const Vector2f& pPosition)
{
	if (!pSurface)
		return { 0,0,0,0 };

	const int x = static_cast<int>(pPosition.mX);
	const int y = static_cast<int>(pPosition.mY);

	if (x < 0 || x >= pSurface->w || y < 0 || y >= pSurface->h)
		return SDL_Color{ 0,0,0,0 };

	
	SDL_Surface* converted = nullptr;
	if (pSurface->format->format != SDL_PIXELFORMAT_RGBA32) 
		converted = SDL_ConvertSurfaceFormat(pSurface, 
			SDL_PIXELFORMAT_RGBA32, 
			0);
	
	SDL_Surface* whichUse = nullptr;
	if (converted)
		whichUse = converted;
	else
		whichUse = pSurface;
	
	SDL_LockSurface(whichUse);
	const int pitch = whichUse->pitch / 4;
	const Uint32* pixels = static_cast<Uint32*>(whichUse->pixels);
	const Uint32 pixel = pixels[y * pitch + x];

	SDL_Color color;
	SDL_GetRGBA(pixel, whichUse->format, &color.r, &color.g, &color.b, &color.a);
	SDL_UnlockSurface(whichUse);

	whichUse = nullptr;
	if (converted)
		SDL_FreeSurface(converted);
	
	return color;
}

SDL_Color TextureManager::getBackGroundColor(const PATH& pPath, const Vector2f& pPosition)
{
	const int y = static_cast<int>(pPosition.mY);
	const int x = static_cast<int>(pPosition.mX);

	SDL_Surface* tmpSurface = SDL_ConvertSurfaceFormat(getSurface(pPath), 
		SDL_PIXELFORMAT_RGBA32,
		0);

	if (x < 0 || x >= tmpSurface->w || y < 0 || y >= tmpSurface->h)
	{
		SDL_FreeSurface(tmpSurface);
		return SDL_Color{ 0,0,0,0 };
	}

	SDL_LockSurface(tmpSurface);
	Uint32* pixels = (Uint32*)tmpSurface->pixels;
	const int pitch = tmpSurface->pitch / 4;		
	const Uint32 pixel = pixels[y * pitch + x];
	SDL_UnlockSurface(tmpSurface);

	SDL_Color tmpColor;
	SDL_GetRGBA(pixel, tmpSurface->format, &tmpColor.r, &tmpColor.g, &tmpColor.b, &tmpColor.a);
	SDL_FreeSurface(tmpSurface);

	return tmpColor;
}

SDL_Color TextureManager::getBackGroundColor(const PATH& pPath)
{
	if (mTextures.find(pPath) == mTextures.end())
	{
		LOG("There is no such a part!");
		return { 0,0,0,0 };
	}
	else
	{
		SDL_Color color;
		SDL_Surface* tmpSurface = getSurface(pPath);
		uint32_t* pixels = (uint32_t*)tmpSurface->pixels;
		SDL_GetRGBA(pixels[0], tmpSurface->format, &color.r, &color.g, &color.b, &color.a);

		return color;
	}
}

SDL_Color TextureManager::getBackGroundColor(SDL_Surface* pSurface)
{
	SDL_Color color;
	uint32_t* pixels = (uint32_t*)pSurface->pixels;
	SDL_GetRGBA(pixels[0], pSurface->format, &color.r, &color.g, &color.b, &color.a);

	return color;
}

SDL_Rect& TextureManager::getRect(const PATH& pPath)
{
	if (mTextures.find(pPath) == mTextures.end())
		LOG("There is no such a part!");
	return mTextures[pPath].second;
}

SDL_Texture* TextureManager::getTexture(const PATH& pPath)
{
	if (mTextures.find(pPath) == mTextures.end())
	{
		LOG("There is no such a part!");
		return nullptr;
	}
	else
		return mTextures[pPath].first.second;
}

SDL_Surface* TextureManager::getSurface(const PATH& pPath)
{
	if (mTextures.find(pPath) == mTextures.end())
	{
		LOG("There is no such a part!");
		return nullptr;
	}
	else
		return mTextures[pPath].first.first;
}
