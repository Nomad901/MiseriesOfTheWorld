#include "Physics.h"

float Physics::calculateDisplacement(float pInitialPos, float pVelocity, float pTime) const
{
    return pInitialPos + pVelocity*pTime;
}

float Physics::calculateVelocity(float pInitialVel, float pAcceleration, float pTime) const
{
    return pInitialVel+pAcceleration*pTime;
}

Vector2f Physics::calculateMovement(Vector2f pPosition, Vector2f pVelocity, float pDeltaTime) const
{
    return pPosition + (pVelocity * pDeltaTime);
}

void Physics::setVelocity(Vector2f pVelocity)
{
    mVelocity = pVelocity;
}

Vector2f Physics::getVelocity()
{
    return mVelocity;
}

void Physics::setAcceleration(float pAcceleration)
{
    mAcceleration = pAcceleration;
}

void Physics::setDeceleration(float pDeceleration)
{
    mDeceleration = pDeceleration;
}

float Physics::getDeceleration()
{
    return mDeceleration;
}

float Physics::getAcceleration()
{
    return mAcceleration;
}

void Physics::applyNormalForce(float pForce)
{
    mForce = pForce;
}

void Physics::applyFrictionKinForce(float pForce)
{
    mKineticFric = pForce;
}

void Physics::applyFrictionStatForce(float pForce)
{
    mStaticFric = pForce;
}

void Physics::resetForces()
{
    mForce = 0.0f;
    mKineticFric = 0.0f;
    mStaticFric = 0.0f;
}
