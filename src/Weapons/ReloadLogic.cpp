#include "ReloadLogic.h"

void ReloadLogic::initReloadLogic(SDL_Renderer* pRenderer, SDL_FRect pCharRect, int16_t pReloadingTime,
                                  bool pShowReloadingQuote, 
                                  SDL_Color pColorNumbers, int32_t pSizeNumbers)
{
    mTimerReload.setDimensionOfTime(Dimension::SECONDS);

    mAnimateStateMachine.init(pRenderer);
    mAnimateStateMachine.pushStateW("ReloadQuote", TypeWait::GENERAL,
        PATH(PROJECT_ROOT) / "Assets" / "photos and ttf" / "reloadQuote.png",
        { static_cast<float>(pCharRect.x),static_cast<float>(pCharRect.h) }, pCharRect.w + 50, pCharRect.h, HorVer::HORIZONTAL,
        { {SideOfChar::RIGHT, {0,1,2}} }, 10, 300);
    mColor = pColorNumbers;
    mRenderer = pRenderer;
    mShowingQuote = pShowReloadingQuote;
    mReloadingTime = pReloadingTime;
    mCurrentReloadingTime = pReloadingTime;

    FactoryOfFonts::getInstance().appendNewFont(pRenderer, PATH(PROJECT_ROOT) / "Assets" / "photos and ttf" / "Arial.ttf",
        std::to_string(pReloadingTime), pColorNumbers, pSizeNumbers);
}

bool ReloadLogic::isReloading()
{
    return mIsReloading;
}

void ReloadLogic::startReloading()
{
    mIsReloading = true;
}

void ReloadLogic::showReloadQuote(bool pShowing)
{
    mShowingQuote = pShowing;
}

void ReloadLogic::setRelodingTime(float pReloadingTime)
{
    mReloadingTime = pReloadingTime;
}

float ReloadLogic::getReloadingTime()
{
    return mReloadingTime;
}

void ReloadLogic::render(SDL_Renderer* pRenderer)
{
    if (mTimerReload.isRunning())
    {
        if (mShowingQuote)
        {
            if (!mAnimateStateMachine.getState("ReloadQuote").value().get().isActive() ||
                !mAnimateStateMachine.getState("ReloadQuote").value().get().isAnimating())
            {
                mAnimateStateMachine.getState("ReloadQuote").value().get().setCurrentSide(SideOfChar::RIGHT);
                mAnimateStateMachine.getState("ReloadQuote").value().get().setActive(true);
                mAnimateStateMachine.getState("ReloadQuote").value().get().runAnimation();
            }
            mAnimateStateMachine.render("ReloadQuote", false);
        }
        else
            FactoryOfFonts::getInstance().render(std::to_string(mCurrentReloadingTime), pRenderer, mRectNumbers);
        
    }
    else
    {
        mAnimateStateMachine.getState("ReloadQuote").value().get().setActive(false);
        mAnimateStateMachine.getState("ReloadQuote").value().get().stopAnimation();
        mCurrentReloadingTime = mReloadingTime;
        mCounterTimer = 1;
    }
}

void ReloadLogic::update(SDL_Rect pCharRect)
{
    if(!mShowingQuote)
        mRectNumbers = { pCharRect.x + pCharRect.w, pCharRect.y - pCharRect.h / 2, pCharRect.w-50, pCharRect.h-50 };
    else
        mRectNumbers = { pCharRect.x + pCharRect.w, pCharRect.y - pCharRect.h, pCharRect.w - 50, pCharRect.h - 50 };

    if (mIsReloading)
    {
        if (!mTimerReload.isRunning())
            mTimerReload.startTimer();
        if (!manageDelay())
            mIsReloading = false;
        if (mShowingQuote)
            mAnimateStateMachine.getState("ReloadQuote").value().get().setPosition({ static_cast<float>(mRectNumbers.x),
                                                                                      static_cast<float>(mRectNumbers.y) });
        if (mNeedToSubtract)
        {
            FactoryOfFonts::getInstance().changeText(mRenderer, std::to_string(mReloadingTime),
                                                     std::to_string(mCurrentReloadingTime -= 1), mColor);
            mNeedToSubtract = false;
        }
    }
}

bool ReloadLogic::manageDelay()
{
    if (mTimerReload.isRunning())
    {
        if (mTimerReload.getDeltaTime(false) >= mReloadingTime)
        {
            mTimerReload.stopTimer();
            return false;
        }
        if (mTimerReload.getDeltaTime(false) == mCounterTimer)
        {
            mNeedToSubtract = true;
            mCounterTimer++;
        }
        return true;
    }
    return false;
}
