#include "Vector.h"
#include <cmath>

void Vector::operator-(void)
{
	x = -x;
	y = -y;
	z = -z;
}

Vector& Vector::operator=(const Vector& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}

inline Vector Vector::operator+(const Vector& rhs) const
{
	return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
}

inline Vector Vector::operator-(const Vector& rhs) const
{
	return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
}

inline Vector& Vector::operator+=(const Vector& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

inline Vector& Vector::operator-=(const Vector& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

inline float Vector::operator*(const Vector& rhs) const 
{
	return std::sqrt(x * rhs.x + y * rhs.y + z * rhs.z);
}

inline Vector Vector::operator*(const float rhs) const 
{
	return Vector(x * rhs, y * rhs, z * rhs);
}

inline Vector Vector::operator/(const float rhs) const
{
	if(rhs > 0.0f)
		return Vector(x / rhs, y / rhs, z / rhs);
	else
		return Vector();
}

inline Vector& Vector::operator*=(const float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

inline Vector& Vector::operator/=(const float rhs)
{
	if(rhs > 0.0f)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	} else {
		return *this;
	}
}

inline void Vector::normalize()
{
	float m = this->magnitude();
	x /= m;
	y /= m;
	z /= m;
}

inline float Vector::magnitude()
{
	return std::sqrt(x * x + y * y + z * z);
}

inline void Vector::reverse()
{
	x = -x;
	y = -y;
	z = -z;
}

Vector cross(const Vector& lhs, const Vector& rhs)
{
	return Vector(lhs.z * rhs.y - lhs.z * rhs.y,
		      lhs.z * rhs.x - lhs.x * rhs.z,
		      lhs.x * rhs.y - lhs.y * rhs.x);	
}
