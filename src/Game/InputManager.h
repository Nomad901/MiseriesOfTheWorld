#pragma once
#include <filesystem>
#include <functional>

#include <SDL.h>

#include "Loger/Loging.h"
#include "Math/Vector2f.h"
#include "Singleton.h"

enum class MouseButton {
	LEFT = 1,
	RIGHT = 2,
	MIDDLE = 3,
	BACK = 4,
	FORWARD = 5
};

class Game;

//TODO: isReleased doesnt work cuz of hurring up

class InputManager : public Singleton<InputManager>
{
public:
	// --- here is you can find the state of your keyboard. like "handler buttons" ---
	void update(std::weak_ptr<Game> pGame, const SDL_Event& pEvents);
	void updatePrevStates();
	void setActive(bool pActive);
	inline bool isActive() const noexcept { return mActive; }

	void lockKeyboard();
	inline bool isLocked() const;

	bool isPressed(SDL_Scancode pScanCode)  const;
	bool isReleased(SDL_Scancode pScanCode) const;
	bool isHeld(SDL_Scancode pScanCode)     const;
	bool buttonWasPressed()					const;
	
	bool isMousePressed(MouseButton mMouse);
	bool isMouseReleased(MouseButton mMouse);
	bool isMouseHeld(MouseButton mMouse);
	bool isMouseInside(const SDL_Rect& pRect);

	Vector2f getMousePos();

private:
	InputManager();
	~InputManager()
	{
		delete[] mPrevKeyboardState;
		mPrevKeyboardState = nullptr;
	}


private:
	const Uint8* mNowKeyboardState{ nullptr };
	Uint8* mPrevKeyboardState{ nullptr };
	
	bool mActive{ false };
	bool mLocked{ false };
	
	int mKeyboardLength{ 0 };
	int32_t mMouseX, mMouseY{ 0 };
	uint32_t mNowMouseState{ 0 };
	uint32_t mPrevMouseState{ 0 };
	MouseButton mMouseButton{ MouseButton::LEFT };

	std::vector<std::function<void(void)>> mStorageOfFunctions;

	friend class Singleton<InputManager>;
};

