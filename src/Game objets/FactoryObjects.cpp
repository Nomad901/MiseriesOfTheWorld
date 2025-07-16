#include "FactoryObjects.h"

FactoryObjects::FactoryObjects()
{
	mStorageOfRects.reserve(50);
	SDL_Rect tmpRect = { 0,0,0,0 };
	SDL_Color tmpColor= { 0,0,0,0 };
	mStorageOfRects.emplace("Empty", std::make_pair(tmpRect, tmpColor));
}

void FactoryObjects::appendObject(std::string_view pName, SDL_Rect pRect, SDL_Color pColor)
{
	mStorageOfRects.insert_or_assign(std::string(pName), std::make_pair(pRect, pColor));
}

void FactoryObjects::appendObject(std::initializer_list<std::string> pNames, SDL_Rect pRect, SDL_Color pColor)
{
	for (auto& i : pNames)
	{
		mStorageOfRects.insert_or_assign(i, std::make_pair(pRect, pColor));
	}
}

void FactoryObjects::deleteObject(std::string_view pName)
{
	auto it = mStorageOfRects.find(std::string(pName));
	if (it != mStorageOfRects.end())
		mStorageOfRects.erase(it);
	else
		LOG("There is no such a rect!");
}

void FactoryObjects::setColor(std::string_view pName, SDL_Color pColor)
{
	if (mStorageOfRects.contains(std::string(pName)))
		mStorageOfRects.at(std::string(pName)).second = pColor;
	else
		LOG("There is no such a rect!");
}

Vector2f FactoryObjects::getPos(std::string_view pName)
{
	if (mStorageOfRects.contains(std::string(pName)))
	{
		SDL_Rect& tmpRect = mStorageOfRects.at(std::string(pName)).first;
		return Vector2f{ static_cast<float>(tmpRect.x), static_cast<float>(tmpRect.y) };
	}
	LOG("There is no such a rect!");
	return { 0.0f,0.0f };
}

void FactoryObjects::setPosition(std::string_view pName, const Vector2f& pPos)
{
	if (mStorageOfRects.contains(std::string(pName)))
	{
		SDL_Rect& tmpRect = mStorageOfRects.at(std::string(pName)).first;
		tmpRect.x = static_cast<int>(pPos.mX);
		tmpRect.y = static_cast<int>(pPos.mY);
	}
	else
		LOG("There is no such a rect!");
}

void FactoryObjects::setDimension(std::string_view pName, const int pW, const int pH)
{
	if (mStorageOfRects.contains(std::string(pName)))
	{
		SDL_Rect& tmpRect = mStorageOfRects.at(std::string(pName)).first;
		tmpRect.w = pW;
		tmpRect.h = pH;
	}
	else
		LOG("There is no such a rect!");
}

void FactoryObjects::highlightObject(std::string_view pName, SDL_Color pColor)
{
	if (mStorageOfRects.contains(std::string(pName)))
	{
		static SDL_Color prevColor = getColor(pName);
		if (InputManager::getInstance().isMouseInside(getRect(pName)))
			setColor(pName, pColor);
		else
			setColor(pName, prevColor);
	}
	else
		LOG("There is no such a name!");
}

SDL_Rect& FactoryObjects::getRect(std::string_view pName)  
{
	if (mStorageOfRects.find(std::string(pName)) != mStorageOfRects.end())
		return mStorageOfRects.at(std::string(pName)).first;
	LOG("There is no such a rect!");
	return mStorageOfRects["Empty"].first;
}

SDL_Color FactoryObjects::getColor(std::string_view pName)
{
	if (mStorageOfRects.find(std::string(pName)) != mStorageOfRects.end())
		return mStorageOfRects.at(std::string(pName)).second;
	LOG("There is no such a rect!");
	return { 0,0,0,0 };
}

auto FactoryObjects::getMap() -> std::unordered_map<std::string, std::pair<SDL_Rect, SDL_Color>>
{
	return mStorageOfRects;
}

bool FactoryObjects::areColliding(SDL_Rect pFirst, SDL_Rect pSecond)
{
	return SDL_HasIntersection(&pFirst, &pSecond);
}

void FactoryObjects::render(std::string_view pName, SDL_Renderer* pRenderer, const bool pFill) const
{
	if (mStorageOfRects.find(std::string(pName)) != mStorageOfRects.end())
	{
		auto [rect, color] = mStorageOfRects.at(std::string(pName));
		
		if (pFill)
		{
			SDL_SetRenderDrawColor(pRenderer, color.r, color.g, color.b, color.a);
			SDL_RenderFillRect(pRenderer, &rect);
		}
		else
		{
			SDL_SetRenderDrawColor(pRenderer, color.r, color.g, color.b, color.a);
			SDL_RenderDrawRect(pRenderer, &rect);
		}
	}
	else
		LOG("There is no such a rect!");
}

void FactoryObjects::drawCircle(SDL_Renderer* pRenderer, const Vector2f& pPos, const Sint16 pRad,
								SDL_Color pColor, const bool pFill) const
{
	if(pFill)
		filledCircleRGBA(pRenderer, static_cast<Sint16>(pPos.mX), static_cast<Sint16>(pPos.mY),
					     pRad, pColor.r, pColor.g, pColor.b, pColor.a);
	else
		circleRGBA(pRenderer, static_cast<Sint16>(pPos.mX), static_cast<Sint16>(pPos.mY),
				   pRad, pColor.r, pColor.g, pColor.b, pColor.a);
}

void FactoryObjects::drawPixel(SDL_Renderer* pRenderer, const Vector2f& pPos, SDL_Color pColor) const
{
	pixelRGBA(pRenderer, static_cast<Sint16>(pPos.mX), static_cast<Sint16>(pPos.mY),
			  pColor.r, pColor.g, pColor.b, pColor.a);
}

SDL_FRect FactoryObjects::convertRect(SDL_Rect pRect)
{
	return { static_cast<float>(pRect.x),
			 static_cast<float>(pRect.y),
			 static_cast<float>(pRect.w),
			 static_cast<float>(pRect.h) };
}

SDL_Rect FactoryObjects::convertFRect(SDL_FRect pFRect)
{
	return { static_cast<int32_t>(pFRect.x),
			 static_cast<int32_t>(pFRect.y),
			 static_cast<int32_t>(pFRect.w),
			 static_cast<int32_t>(pFRect.h) };
}

