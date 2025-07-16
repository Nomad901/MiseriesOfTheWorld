#include "FactoryOfFonts.h"

FactoryOfFonts* FactoryOfFonts::mInstance = nullptr;

void FactoryOfFonts::init()
{
	if (TTF_Init() == -1)
		LOG("couldnt initilize the ttf!");

	if (mInstance != nullptr)
		shutDown();
	mInstance = new FactoryOfFonts();
}

FactoryOfFonts& FactoryOfFonts::getInstance()
{
	return *mInstance;
}

void FactoryOfFonts::shutDown()
{
	delete mInstance;
	mInstance = nullptr;
}

void FactoryOfFonts::appendNewFont(SDL_Renderer* pRenderer, const PATH& pPath, std::string_view pText,
								   const SDL_Color& pColor, const int pSize)
{
	mColor = pColor;
	mText.push_back(std::string(pText));
	mSize = pSize;
	mPath = pPath;

	TTF_Font* tmpFont = TTF_OpenFont(pPath.string().c_str(), pSize);
	if (!tmpFont)
		LOG("Couldnt download the font!");
	SDL_Surface* tmpSurface = TTF_RenderText_Solid(tmpFont, std::string(pText).c_str(), pColor);
	if (!tmpSurface)
		LOG("Couldnt download the surface!");
	SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(pRenderer, tmpSurface);
	if (!tmpTexture)
		LOG("Couldnt download the texture!");
	mFonts.emplace(std::string(pText), std::make_pair(tmpFont, tmpTexture));
	SDL_FreeSurface(tmpSurface);
}

void FactoryOfFonts::appendNewFonts(SDL_Renderer* pRenderer, const PATH& pPath, std::initializer_list<std::string> pTexts, const SDL_Color& pColor, const int pSize)
{
	mColor = pColor;
	mSize = pSize;
	mPath = pPath;
	for (auto& i : pTexts)
	{
		mText.push_back(i);

		TTF_Font* tmpFont = TTF_OpenFont(pPath.string().c_str(), pSize);
		if (!tmpFont)
			LOG("Couldnt download the font!");
		SDL_Surface* tmpSurface = TTF_RenderText_Solid(tmpFont, i.c_str(), pColor);
		if (!tmpSurface)
			LOG("Couldnt download the surface!");
		SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(pRenderer, tmpSurface);
		if (!tmpTexture)
			LOG("Couldnt download the texture!");
		mFonts.emplace(i, std::make_pair(tmpFont, tmpTexture));
		SDL_FreeSurface(tmpSurface);
	}
}

void FactoryOfFonts::changeText(SDL_Renderer* pRenderer, std::string_view pOrigText, std::string_view pText, SDL_Color pColor)
{
	if (mFonts.contains(std::string(pOrigText)))
	{
		SDL_Surface * tmpSurface = TTF_RenderText_Solid(getFonts(std::string(pOrigText)), std::string(pText).c_str(), pColor);
		if (!tmpSurface)
			LOG("Couldnt download the surface!");
		SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(pRenderer, tmpSurface);
		if (!tmpTexture)
			LOG("Couldnt download the texture!");
		mFonts.emplace(std::string(pText), std::make_pair(getFonts(std::string(pOrigText)), tmpTexture));
		SDL_FreeSurface(tmpSurface);
	}
	else
		LOG("There is no such a OrigText!");
}
void FactoryOfFonts::deleteFont(std::string_view pAlias)
{
	auto it = mFonts.find(std::string(pAlias));
	if (it != mFonts.end())
	{
		TTF_CloseFont(it->second.first);
		SDL_DestroyTexture(it->second.second);
	}
	else
		LOG("there is no such an alias!");
}

void FactoryOfFonts::fontIntoRect(std::string_view pText, SDL_Rect& pRect)
{
	int tmpWidth, tmpHeight;
	TTF_SizeText(getFonts(pText), std::string(pText).c_str(), &tmpWidth, &tmpHeight);
	pRect = { pRect.x + (pRect.w - tmpWidth) / 2, pRect.y + (pRect.h - tmpHeight) / 2, tmpWidth, tmpHeight };
}

TTF_Font* FactoryOfFonts::getFont(std::string_view pAlias)
{
	return getFonts(pAlias);
}

SDL_Texture* FactoryOfFonts::getTexture(std::string_view pAlias)
{
	return getTextureFont(pAlias);
}

void FactoryOfFonts::render(std::string_view pName, SDL_Renderer* pRenderer, SDL_Rect pRect)
{
	if (mFonts.contains(std::string(pName)))
		SDL_RenderCopy(pRenderer, mFonts[std::string(pName)].second, nullptr, &pRect);
	else
		LOG("There is no such a name!");
}




