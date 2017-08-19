#include "stdafx.h"
#include "Mat3.h"
#include <math.h>

Mat3::Mat3()
{
	zero();
}


Mat3::~Mat3()
{
}

Mat3::Mat3(float f00, float f01, float f02, float f10, float f11, float f12, float f20, float f21, float f22)
{
	m[0][0] = f00;
	m[0][1] = f01;
	m[0][2] = f02;

	m[1][0] = f10;
	m[1][1] = f11;
	m[1][2] = f12;

	m[2][0] = f20;
	m[2][1] = f21;
	m[2][2] = f22;
}

void Mat3::zero()
{
	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
		{
			m[x][y] = 0;
		}
}
