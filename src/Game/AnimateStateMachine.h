#pragma once
#include <unordered_set>
#include <algorithm>
#include <expected>

#include "Game/AnimatedTexture.h"
#include "Game/TextureManager.h"
#include "Game/Assets.h"
#include "Time/Timer.h"

class AnimateStateMachine
{
public:
	AnimateStateMachine() = default;
	
	void init(SDL_Renderer* pRenderer);

	void pushState(std::string_view pName, std::unique_ptr<AnimatedTexture> pState,
				   const std::unordered_map<SideOfChar, std::vector<uint32_t>>& pNumbers, int32_t pDelay, float pItensity);
	void pushStateW(std::string_view pName, TypeWait pType, const PATH& pPath,
				   const Vector2f& pPos, const int pW, const int pH, HorVer pSide,
				   const std::unordered_map<SideOfChar, std::vector<uint32_t>>& pNumbers, int32_t pDelay, float pItensity);
	void popState(std::string_view pName);
	bool findState(std::string_view pName) const;
	void setCurrentState(std::string_view pName);

	void adjustState(std::string_view pName, const std::unordered_map<SideOfChar, std::vector<uint32_t>>& pNumbers, 
					 int32_t pDelay, float pItensity);

	//delay between animations
	void setDelay(int32_t pDelay);
	void setDelay(std::string_view pName, int32_t pDelay);
	int32_t getDelay();
	int32_t getDelay(std::string_view pName);

	void renderAndUpdate(bool pRotate, float pAngle = 0.0f);
	void render(std::string_view pName, bool pRotate, float pAngle = 0.0f);
	void render(const std::vector<std::string>& pNames, bool pRotate, float pAngle = 0.0f);

	auto getState(std::string_view pName) -> std::expected<std::reference_wrapper<AnimatedTexture>, std::string_view>;
	auto getState()						  -> std::expected<std::reference_wrapper<AnimatedTexture>, std::string_view>;

private:
	SDL_Renderer* mRenderer{ nullptr };

	Timer mTimer;

	struct AnimatedTextureStats
	{
		int32_t mDelay{ 0 };
		std::unique_ptr<AnimatedTexture> mAnimatedTexture{ nullptr };
	};
	std::string mState;
	std::unordered_set<std::string> mNames;
	std::unordered_map<std::string, AnimatedTextureStats> mStorageTextures;
};
