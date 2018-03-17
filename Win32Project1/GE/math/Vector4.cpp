

#include "Vector4.h"
#include "quaternion.h"
#include<math.h>
#include "base.h"

Vector4::Vector4()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 1.0;
}


Vector4::~Vector4()
{
}

Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

