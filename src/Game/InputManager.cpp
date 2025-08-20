#include "InputManager.h"
#include "Game.h"
#include "TextureManager.h"

InputManager::InputManager()
    : mLocked{false}
{
    mNowKeyboardState = SDL_GetKeyboardState(&mKeyboardLength);
    mPrevKeyboardState = new Uint8[mKeyboardLength];
    memcpy(mPrevKeyboardState, mNowKeyboardState, mKeyboardLength);

    mStorageOfFunctions.reserve(20);
    
}

void InputManager::updatePrevStates()
{
    SDL_PumpEvents();

    memcpy(mPrevKeyboardState, mNowKeyboardState, mKeyboardLength);
    mNowKeyboardState = SDL_GetKeyboardState(nullptr);
    mPrevMouseState = mNowMouseState;
    mNowMouseState = SDL_GetMouseState(&mMouseX, &mMouseY);
}

void InputManager::update(std::weak_ptr<Game> pGame, const SDL_Event& pEvents)
{
    if (auto game = pGame.lock())
    {
        if (pEvents.type == SDL_QUIT)
            game->stopLoop();
        if (isPressed(SDL_SCANCODE_L))
        {
            std::cout << "keyboard is locked!\n";
            lockKeyboard();
        }
    }
    else
    {
        LOG("Game is NULL and cant be seted up!");
        return;
    }
}

void InputManager::setActive(bool pActive)
{
    mActive = pActive;
}

void InputManager::lockKeyboard()
{
    mLocked = !mLocked;
}

bool InputManager::isPressed(SDL_Scancode pScanCode) const
{
    return ((mNowKeyboardState[pScanCode] != 0) && (mPrevKeyboardState[pScanCode] == 0));
}

bool InputManager::isReleased(SDL_Scancode pScanCode) const
{
    return ((mNowKeyboardState[pScanCode] == 0) && (mPrevKeyboardState[pScanCode] != 0));
}

bool InputManager::isHeld(SDL_Scancode pScanCode) const
{
    return ((mNowKeyboardState[pScanCode] != 0) && (mPrevKeyboardState[pScanCode] != 0));
}

bool InputManager::buttonWasPressed() const
{
    for (size_t i = 0; i < mKeyboardLength; ++i)
    {
        if (mNowKeyboardState[i] != 0)
            return true;
    }
    return false;
}

bool InputManager::isMousePressed(MouseButton mMouse)
{
    Uint32 mask = 0;

    switch (mMouse)
    {
    case MouseButton::LEFT:
        mask = SDL_BUTTON_LMASK;
        break;
    case MouseButton::RIGHT:
        mask = SDL_BUTTON_RMASK;
        break;
    case MouseButton::MIDDLE:
        mask = SDL_BUTTON_MMASK;
        break;
    case MouseButton::BACK:
        mask = SDL_BUTTON_X1MASK;
        break;
    case MouseButton::FORWARD:
        mask = SDL_BUTTON_X2MASK;
        break;
    }

    return ((mNowMouseState & mask) != 0);
}

bool InputManager::isMouseReleased(MouseButton mMouse)
{
    Uint32 mask = 0;

    switch (mMouse)
    {
    case MouseButton::LEFT:
        mask = SDL_BUTTON_LMASK;
        break;
    case MouseButton::RIGHT:
        mask = SDL_BUTTON_RMASK;
        break;
    case MouseButton::MIDDLE:
        mask = SDL_BUTTON_MMASK;
        break;
    case MouseButton::BACK:
        mask = SDL_BUTTON_X1MASK;
        break;
    case MouseButton::FORWARD:
        mask = SDL_BUTTON_X2MASK;
        break;
    }

    return ((mNowMouseState & mask) == 0 && (mPrevMouseState & mask) != 0);
}

bool InputManager::isMouseHeld(MouseButton mMouse)
{
    Uint32 mask = 0;

    switch (mMouse)
    {
    case MouseButton::LEFT:
        mask = SDL_BUTTON_LMASK;
        break;
    case MouseButton::RIGHT:
        mask = SDL_BUTTON_RMASK;
        break;
    case MouseButton::MIDDLE:
        mask = SDL_BUTTON_MMASK;
        break;
    case MouseButton::BACK:
        mask = SDL_BUTTON_X1MASK;
        break;
    case MouseButton::FORWARD:
        mask = SDL_BUTTON_X2MASK;
        break;
    }

    return ((mNowMouseState & mask) != 0 && (mPrevMouseState & mask) !=0);
}

bool InputManager::isLocked() const
{
    return mLocked;
}

bool InputManager::isMouseInside(const SDL_Rect& pRect)
{
    return (mMouseX > pRect.x && mMouseX < (pRect.x + pRect.w) &&
            mMouseY > pRect.y && mMouseY < (pRect.y + pRect.h));
}

Vector2f InputManager::getMousePos()
{
    return Vector2f((float) mMouseX, (float) mMouseY);
}

std::unordered_map<SDL_Scancode, bool>& InputManager::getStorageKeyCodes() noexcept
{
    return mStorageKeyCodes;
}

void InputManager::setScancode(const SDL_Scancode& pKeycode, bool pValue)
{
    mStorageKeyCodes[pKeycode] = pValue;
}


