#include "Vector2f.h"
Vector2f::Vector2f()
{
	mX = 0;
	mY = 0;
	mVX = 0;
	mVY = 0;
}

Vector2f::Vector2f(const float pX, const float pY)
	:mX(pX), mY(pY),
	mVX(0), mVY(0)
{}

 Vector2f Vector2f::operator+(const Vector2f& pOther) const
{
	return Vector2f(mX + pOther.mX, mY + pOther.mY);
}
 Vector2f Vector2f::operator+=(const Vector2f& pOther)
{
	mX += pOther.mX;
	mY += pOther.mY;
	return *this;
}
 Vector2f Vector2f::operator+(const float pScalar) const
{
	return Vector2f(mX + pScalar, mY + pScalar);
}

 Vector2f Vector2f::operator-(const Vector2f& pOther) const
{
	return Vector2f(mX - pOther.mX, mY - pOther.mY);
}
 Vector2f Vector2f::operator-=(const Vector2f& pOther)
{
	mX -= pOther.mX;
	mY -= pOther.mY;
	return *this;
}
 Vector2f Vector2f::operator-(const float pScalar) const
{
	return Vector2f(mX - pScalar, mY - pScalar);
}

 Vector2f Vector2f::operator*(const Vector2f pOther) const
{
	return Vector2f(mX * pOther.mX, mY * pOther.mY);
}
 Vector2f Vector2f::operator*=(const Vector2f& pOther)
{
	mX *= pOther.mX;
	mY *= pOther.mY;
	return *this;
}
 Vector2f Vector2f::operator*(const float pScalar) const
{
	return Vector2f(mX * pScalar, mY * pScalar);
}

 Vector2f Vector2f::operator/(const Vector2f pOther) const
{
	return Vector2f(mX / pOther.mX, mY / pOther.mY);
}
 Vector2f Vector2f::operator/=(const Vector2f& pOther)
{
	mX /= pOther.mX;
	mY /= pOther.mY;
	return *this;
}
 Vector2f Vector2f::operator/(const float pScalar) const
{
	return Vector2f(mX / pScalar, mY / pScalar);
}

 bool Vector2f::operator==(const Vector2f& pOther) const
{
	return mX == pOther.mX && mY == pOther.mY ? true : false;
}
 bool Vector2f::operator!=(const Vector2f& pOther) const
{
	return mX != pOther.mX || mY != pOther.mY ? true : false;
}

Vector2f Vector2f::rotateVector(const Vector2f& pVec, const float pAngle)
{
	float radAngle = (float)(pAngle * DEG_TO_RAD);
	return Vector2f((float)(pVec.mX * cos(radAngle) - pVec.mY * sin(radAngle)), (float)(pVec.mX * sin(radAngle) + pVec.mY * cos(radAngle)));
}

 float Vector2f::magnitudeSqr() const
{
	return mX*mX + mY*mY;
}

 float Vector2f::length() const
{
	return std::sqrt((mX * mX) + (mY * mY));
}
Vector2f Vector2f::normalize() const
{
	const float varLenght = length();
	if(varLenght != 0)
		return Vector2f(mX / varLenght, mY / varLenght);
	return Vector2f{ 0.0f,0.0f };
}

float Vector2f::distanceTo(const Vector2f& pOther) const
{
	return std::sqrt((mX - pOther.mX) * (mX - pOther.mX) + (mY - pOther.mY) * (mY - pOther.mY));
}

float Vector2f::dotProduct(const Vector2f& pFirst, const Vector2f& pSecond)
{
	return (pFirst.mX * pSecond.mX) + (pFirst.mY * pSecond.mY);
}
