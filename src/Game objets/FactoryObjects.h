#pragma once
#include <filesystem>
#include <unordered_map>
#include <string>

#include "Loger/Loging.h"
#include "Math/Vector2f.h"
#include "Game/InputManager.h"

#include "SDL/SDL2_gfxPrimitives.h"
#include "SDL/SDL2_gfxPrimitives_font.h"

class FactoryObjects
{
public:
	FactoryObjects();
	~FactoryObjects() = default;

	void appendObject(std::string_view pName, SDL_Rect pRect, SDL_Color pColor);
	void appendObject(std::initializer_list<std::string> pNames, SDL_Rect pRect, SDL_Color pColor);
	void deleteObject(std::string_view pName);

	void setColor(std::string_view pName, SDL_Color pColor);
	Vector2f getPos(std::string_view pName);
	void setPosition(std::string_view pName, const Vector2f& pPos);
	void setDimension(std::string_view pName, const int pW, const int pH);
	void highlightObject(std::string_view pName, SDL_Color pColor);

	SDL_Rect& getRect(std::string_view pName);
	SDL_Color getColor(std::string_view pName);
	auto getMap() -> std::unordered_map<std::string, std::pair<SDL_Rect, SDL_Color>>;

	bool areColliding(SDL_Rect pFirst, SDL_Rect pSecond);

	void render(std::string_view pName, SDL_Renderer* pRenderer, const bool pFill) const;
	void drawCircle(SDL_Renderer* pRenderer, const Vector2f& pPos, const Sint16 pRad,
					SDL_Color pColor, const bool pFill) const;
	void drawPixel(SDL_Renderer* pRenderer, const Vector2f& pPos, SDL_Color pColor) const;

	SDL_FRect convertRect(SDL_Rect pRect);
	SDL_Rect convertFRect(SDL_FRect pFRect);

private:
	std::unordered_map<std::string, std::pair<SDL_Rect,SDL_Color>> mStorageOfRects;

};

