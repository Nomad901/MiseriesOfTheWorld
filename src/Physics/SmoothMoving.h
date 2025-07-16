#pragma once
#include <SDL.h>

#include "Math/Vector2f.h"
#include "Physics/Physics.h"

class SmoothMoving : public Physics
{
public:
	SmoothMoving() = default;

	void moving(Vector2f& pPosition, float pTime);
	void stopping();
	
	

	void setTargetVelocity(Vector2f pVelocity);
	Vector2f getTargetVelocity();

private:
	Vector2f mTargetVelocity{ 2.0f, 2.0f };

};

