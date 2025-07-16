#pragma once
#include <utility>
#include <ostream>
#include <math.h>

#define PI 3.14159265
#define DEG_TO_RAD PI / 100.0f

struct Vector2f
{
public:
	Vector2f();
	Vector2f(const float pX, const float pY);
	
	Vector2f operator+(const Vector2f& pOther) const;
	Vector2f operator+=(const Vector2f& pOther);
	Vector2f operator+(const float pScalar) const;

	Vector2f operator-(const Vector2f& pOther) const;
	Vector2f operator-=(const Vector2f& pOther);
	Vector2f operator-(const float pScalar) const;

	Vector2f operator*(const Vector2f pOther) const;
	Vector2f operator*=(const Vector2f& pOther);
	Vector2f operator*(const float pScalar) const;
	
	Vector2f operator/(const Vector2f pOther) const;
	Vector2f operator/=(const Vector2f& pOther);
	Vector2f operator/(const float pScalar) const;

	bool operator==(const Vector2f& pOther) const;
	bool operator!=(const Vector2f& pOther) const;
	
	friend std::ostream& operator<<(std::ostream& os, const Vector2f& vec)
	{
		return os << "(" << vec.mX << " , " << vec.mY << ")";
	}

	Vector2f rotateVector(const Vector2f& pVec, const float pAngle);
	Vector2f normalize() const;

	float magnitudeSqr()const;
	float length() const;
	float distanceTo(const Vector2f& pOther) const;
	float dotProduct(const Vector2f& pFirst, const Vector2f& pSecond);
	
	float mX, mY;
	float mVX, mVY;
};
