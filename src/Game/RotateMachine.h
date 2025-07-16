#pragma once
#include <math.h>

#include "Loger/Loging.h"
#include "Math/Vector2f.h"

enum class DimensionDegOrRad
{
	DEGREES = 0,
	RADIANS = 1
};

class RotateMachine
{	
public:
	void calculateDirection(const Vector2f& pPos1, const Vector2f& pPos2);
	void calculateLength(const Vector2f& pPos);
	void calculateSpeed(const Vector2f& pDirection, double pLength, float pSpeed);
	void calculateSpeed(const Vector2f& pNormilizedVec, float pSpeed);

	void calculateRadians(const Vector2f& pPos);
	void calculateDegrees(const Vector2f& pPos);
	//pos2 - pos1
	void calculateRadians(const Vector2f& pPos1, const Vector2f pPos2);
	//pos2 - pos1
	void calculateDegrees(const Vector2f& pPos1, const Vector2f pPos2);

	void convertRadiansIntoDegrees(double pAngle);
	void convertDegreesIntoRadians(double pAngle);

	void setAngle(DimensionDegOrRad pDimension, double pAngle);
	void setLength(double pLength);
	void setDirection(const Vector2f& pDirection);
	void setSpeed(const Vector2f& pSpeed);
	void setDimension(DimensionDegOrRad pDimension);

	void rotateAttachedRect(SDL_FRect& pAttachedRect, SDL_FRect pBaseRect, float pAngle, const Vector2f& pOffsets);

	double getAngle() const noexcept;
	double getLength() const noexcept;
	Vector2f getDirection() const noexcept;
	Vector2f getSpeed() const noexcept;
	DimensionDegOrRad getDimension() const noexcept;

private:
	DimensionDegOrRad mDimension{ DimensionDegOrRad::DEGREES };

	double mAngle{ 0.0f };
	double mLength{ 0.0f };
	Vector2f mDirection{ 0.0f,0.0f };
	Vector2f mSpeed{ 0.0f,0.0f };

};

