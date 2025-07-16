#pragma once
#include "Math/Vector2f.h"
#include "Game/Singleton.h"

class Physics : public Singleton<Physics>
{
public:

	//------------
	//-kinematics-
	//------------
	float calculateDisplacement(float pInitialPos, float pVelocity, float pTime) const;
	float calculateVelocity(float pInitialVel, float pAcceleration, float pTime) const;
	Vector2f calculateMovement(Vector2f pPosition, Vector2f pVelocity, float pDeltaTime) const;
	void setVelocity(Vector2f pVelocity);
	Vector2f getVelocity();
	void setAcceleration(float pAcceleration);
	void setDeceleration(float pDeceleration);
	float getDeceleration();
	float getAcceleration();

	//--------------
	//---friction---
	//--------------
	void applyNormalForce(float pForce);
	void applyFrictionKinForce(float pForce);
	void applyFrictionStatForce(float pForce);
	void resetForces();

	virtual void update() {}

private:
	Vector2f mVelocity{ 0.0f, 0.0f };
	float mAcceleration{ 0.0f };
	float mDeceleration{ 0.0f };
	float mForce{ 0.0f };
	float mDistance{ 0.0f };
	float mKineticFric{ 0.0f };
	float mStaticFric{ 0.0f };
};

