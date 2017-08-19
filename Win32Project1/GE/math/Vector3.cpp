#include "stdafx.h"
#include "Vector3.h"
#include "quaternion.h"
#include<math.h>
#include "common.h"

Vector3::Vector3()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}


Vector3::~Vector3()
{
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3::normalize()
{
	float m = mag();
	x = x / m;
	y = y / m;
	z = z / m;
}
float Vector3::mag()
{
	return sqrt(x*x + y*y + z*z);
}

void Vector3::cross(const Vector3& a, const Vector3& b, Vector3& res)
{
	//const float _x = y * v.z - z * v.y;
	res.x = a.y * b.z - a.z * b.y;
	//const float _y = z * v.x - x * v.z;
	res.y = a.z * b.x - a.x * b.z;
	//const float _z = x * v.y - y * v.x;
	res.z = a.x * b.y - a.y * b.x;

}

void Vector3::subtract(const Vector3& a, const Vector3& b, Vector3& res)
{
	res.x = a.x - b.x;
	res.y = a.y - b.y;
	res.z = a.z - b.z;
}

float Vector3::dot(const Vector3& v1, const Vector3& v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

