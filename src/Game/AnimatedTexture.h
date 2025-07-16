#pragma once
#include "Game/Singleton.h"
#include "Game/TextureManager.h"
#include "Time/Timer.h"
#include "Game/Assets.h"
#include "Game/InputManager.h"
#include "Game/RotateMachine.h"

enum class SideOfChar
{
	UP = 0,
	DOWN = 1,
	RIGHT = 2,
	LEFT = 3
};
enum class HorVer
{
	VERTICAL = 0,
	HORIZONTAL = 1
};
enum class TypeWait
{
	GENERAL = 0,
	WAIT = 1
};

class AnimatedTexture : public TextureManager
{
private:
	struct conditionalVars;
public:
	AnimatedTexture() = default;
	~AnimatedTexture();

	void appendAnimation(TypeWait pType, SDL_Renderer* pRenderer, const PATH& pPath, const Vector2f& pPos, 
						 const int pW, const int pH, HorVer pSide);
	//than bigger - then slower
	void setIntensity(const float pIntensity);
	void setCurrentSide(const SideOfChar pSide);
	void setActive(const bool pActive);
	void waitWithAnim(const bool pWaitAnim);
	void setSides(const std::unordered_map<SideOfChar, std::vector<uint32_t>>& pNumbers);
	void setPosition(const Vector2f& pPos);
	void setPositionRotate(SDL_FRect pBaseRect, float pAngle, const Vector2f& pOffset);
	void setEnded(bool pEnded);
	void enableWaitAnim(bool pWait);
	void setAnimating(bool pAnimating);

	Vector2f getPosition() const;
	conditionalVars const& getAllConditions() const noexcept;
	bool isActive() const;
	bool isEnded() const;
	bool isInWaitAnim() const noexcept;
	bool isAnimating() const noexcept;

	SDL_FRect getAnimRect() const noexcept;

	void stopAnimation();
	void runAnimation();
	void runAnimationOnlyOnce();

	void render(SDL_Renderer* pRenderer);
	void renderWithRotate(SDL_Renderer* pRenderer, double pAngle, const Vector2f& pMousePos);
	// ticks for animation. like every tick - will be playing each frame 
	void nullTicks();

private:
	void dividerFrames(SDL_Renderer* pRenderer);
	void createTexturesFromFrames(SDL_Renderer* pRenderer);

private:
	struct conditionalVars
	{
		bool mInWaitAnim{ false };
		bool mWaitAnim{ false };
		bool mAnimating{ false };
		bool mActive{ false };
		bool mOnceAnim{ false };
		bool mEnded{ false };
	} mConditionals;

	float mIntensity{ 0.0f };
	int32_t mCounter{ 0 };
	Timer mTimer;

	SDL_FRect mDstRect{50,50,50,50};
	std::vector<SDL_Rect> mSprites;
	std::unordered_map<int, std::pair<SDL_Texture*, SDL_Rect>> mStorageFrames;
	std::unordered_map<int, std::pair<SDL_Texture*, SDL_Rect>> mStorageWaitFrames;
	std::unordered_map<SideOfChar, std::vector<uint32_t>> mSteps;

	RotateMachine mRotateMachine;

	HorVer mHorVer{ HorVer::HORIZONTAL };
	SideOfChar mCurrentSide{ SideOfChar::UP };

	PATH mPath{ std::filesystem::current_path() };
};