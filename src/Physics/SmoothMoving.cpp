#include "SmoothMoving.h"

void SmoothMoving::moving(Vector2f& pPosition, float pTime)
{
    pPosition += mTargetVelocity * pTime;
}

void SmoothMoving::stopping()
{

}

void SmoothMoving::setTargetVelocity(Vector2f pVelocity)
{
    mTargetVelocity = pVelocity;
}

Vector2f SmoothMoving::getTargetVelocity()
{
    return mTargetVelocity;
}
