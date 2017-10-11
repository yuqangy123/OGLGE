#pragma once
#include "Mat3.h"
#include <cfloat>

#define Vector4f Vector4
struct Vector4
{
	float x;
	float y;
	float z;
	float w;

	Vector4();
	~Vector4();
	Vector4(float x, float y, float z, float w=1.0);

	inline Vector4 operator-(Vector4& Right)const
	{
		Vector4 ret;
		ret.x = (this->x - Right.x);
		ret.y = (this->y - Right.y);
		ret.z = (this->z - Right.z);
		ret.w = 1.f;
		return ret;
	}

	inline void normalizieW()
	{
		if (w > FLT_MIN)
		{
			x /= w;
			y /= w;
			z /= w;
			w = 1.0f;
		}
	}
	
};

