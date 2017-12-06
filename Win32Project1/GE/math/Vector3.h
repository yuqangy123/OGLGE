#pragma once
#include "Mat3.h"


#define Vector3f Vector3
struct Vector3
{
	float x;
	float y;
	float z;

	Vector3();
	~Vector3();
	Vector3(float x, float y, float z);
	inline Vector3 operator-(Vector3& Right)const
	{
		Vector3 ret;
		ret.x = (this->x - Right.x);
		ret.y = (this->y - Right.y);
		ret.z = (this->z - Right.z);
		return ret;
	}
	inline Vector3 operator+(const Vector3& Right)
	{
		Vector3 ret;
		ret.x = (this->x + Right.x);
		ret.y = (this->y + Right.y);
		ret.z = (this->z + Right.z);
		return ret;
	}
	inline Vector3& operator+=(const Vector3& Right)
	{
		x = (x + Right.x);
		y = (y + Right.y);
		z = (z + Right.z);
		return *this;
	}
	inline Vector3 operator*(const float& Right)
	{
		Vector3 ret;
		ret.x = (this->x * Right);
		ret.y = (this->y * Right);
		ret.z = (this->z * Right);
		return ret;
	}
	inline Vector3& operator*=(const float& Right)
	{
		x = (this->x * Right);
		y = (this->y * Right);
		z = (this->z * Right);
		return *this;
	}
	inline Vector3 operator*(const Mat3& Right)
	{
		float xx = (x*Right.m[0][0] + x*Right.m[1][0] + x*Right.m[2][0]);
		float yy = (y*Right.m[0][1] + y*Right.m[1][1] + y*Right.m[2][1]);
		float zz = (z*Right.m[0][2] + z*Right.m[1][2] + z*Right.m[2][2]);

		return Vector3(xx, yy, zz);
	}
	inline const Vector3 operator+(const Vector3& v) const
	{
		Vector3 result(*this);
		result.add(v);
		return result;
	}
	inline const bool operator!=(const Vector3& v) const
	{
		return (v.x != x || v.y != y || v.z != z);
	}
	inline const bool operator==(const Vector3& v) const
	{
		return !(v.x != x || v.y != y || v.z != z);
	}

	inline void add(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}
	inline void set(float fx, float fy, float fz)
	{
		x = fx; y = fy; z = fz;
	}

	void normalize();
	float mag();

	static float dot(const Vector3& v1, const Vector3& v2);
	static void cross(const Vector3& a, const Vector3& b, Vector3& res);
	static void subtract(const Vector3& a, const Vector3& b, Vector3& res);


};

