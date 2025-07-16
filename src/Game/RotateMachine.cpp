#include "RotateMachine.h"

void RotateMachine::calculateDirection(const Vector2f& pPosPlayer, const Vector2f& pPosMouse)
{
    mDirection = { pPosMouse.mX - pPosPlayer.mX,
                   pPosMouse.mY - pPosPlayer.mY };
}

void RotateMachine::calculateLength(const Vector2f& pPos)
{
    mLength = sqrt((pPos.mX * pPos.mX) + (pPos.mY * pPos.mY));
}

void RotateMachine::calculateSpeed(const Vector2f& pDirection, double pLength, float pSpeed)
{
    mSpeed = { static_cast<float>((pDirection.mX / pLength) * static_cast<float>(pSpeed)), 
               static_cast<float>((pDirection.mY / pLength) * static_cast<float>(pSpeed)) };
}

void RotateMachine::calculateSpeed(const Vector2f& pNormilizedVec, float pSpeed)
{
    mSpeed = { pNormilizedVec.mX * pSpeed,
               pNormilizedVec.mY * pSpeed };
}

void RotateMachine::calculateRadians(const Vector2f& pPos)
{
    mAngle = atan2(pPos.mY, pPos.mX);
    mDimension = DimensionDegOrRad::RADIANS;
}

void RotateMachine::calculateDegrees(const Vector2f& pPos)
{
    mAngle = atan2(pPos.mY, pPos.mX) * 180 / M_PI;
    mDimension = DimensionDegOrRad::DEGREES;
}

void RotateMachine::calculateRadians(const Vector2f& pPos1, const Vector2f pPos2)
{
    mAngle = atan2(pPos2.mY - pPos1.mY, pPos2.mX - pPos1.mX);
    mDimension = DimensionDegOrRad::RADIANS;
}

void RotateMachine::calculateDegrees(const Vector2f& pPos1, const Vector2f pPos2)
{
    mAngle = atan2(pPos2.mY - pPos1.mY, pPos2.mX - pPos1.mX) * 180 / M_PI;
    mDimension = DimensionDegOrRad::DEGREES;
}

void RotateMachine::convertRadiansIntoDegrees(double pAngle)
{
    mAngle = pAngle * 180 / M_PI;
    mDimension = DimensionDegOrRad::DEGREES;
}

void RotateMachine::convertDegreesIntoRadians(double pAngle)
{
    mAngle = pAngle * M_PI / 180;
    mDimension = DimensionDegOrRad::RADIANS;
}

void RotateMachine::setAngle(DimensionDegOrRad pDimension, double pAngle)
{
    mDimension = pDimension;
    mAngle = pAngle;
}

void RotateMachine::setLength(double pLength)
{
    mLength = pLength;
}

void RotateMachine::setDirection(const Vector2f& pDirection) 
{
    mDirection = pDirection;
}

void RotateMachine::setSpeed(const Vector2f& pSpeed) 
{
    mSpeed = pSpeed;
}

void RotateMachine::setDimension(DimensionDegOrRad pDimension)
{
    mDimension = pDimension;
}

void RotateMachine::rotateAttachedRect(SDL_FRect& pAttachedRect, SDL_FRect pBaseRect, float pAngle, const Vector2f& pOffsets)
{
    float weaponCenterX = pBaseRect.x + pBaseRect.w / 2.0f;
    float weaponCenterY = pBaseRect.y + pBaseRect.h / 2.0f;

    float rotatedX = pOffsets.mX * cos(pAngle) - pOffsets.mY * sin(pAngle);
    float rotatedY = pOffsets.mX * sin(pAngle) + pOffsets.mY * cos(pAngle);

    pAttachedRect.x = weaponCenterX + rotatedX - pAttachedRect.w / 2.0f;
    pAttachedRect.y = weaponCenterY + rotatedY - pAttachedRect.h / 2.0f;
}

double RotateMachine::getAngle() const noexcept
{
    return mAngle;
}

double RotateMachine::getLength() const noexcept
{
    return mLength;
}

Vector2f RotateMachine::getDirection() const noexcept
{
    return mDirection;
}

Vector2f RotateMachine::getSpeed() const noexcept
{
    return mSpeed;
}

DimensionDegOrRad RotateMachine::getDimension() const noexcept
{
    return mDimension;
}
