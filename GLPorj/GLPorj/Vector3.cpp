#include "stdafx.h"
#include "Vector3.h"



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


