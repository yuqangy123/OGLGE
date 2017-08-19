#include "stdafx.h"
#include "Mat4.h"


const Mat4 Mat4::IDENTITY = Mat4(
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);

const Mat4 Mat4::ZERO = Mat4(
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0 );

Mat4::Mat4()
{
	Mat4::identity(*this);
}


Mat4::~Mat4()
{
}

Mat4::Mat4(float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15)
{
	m[0] = f0;
	m[1] = f1;
	m[2] = f2;
	m[3] = f3;

	m[4] = f4;
	m[5] = f5;
	m[6] = f6;
	m[7] = f7;

	m[8] = f8;
	m[9] = f9;
	m[10] = f10;
	m[11] = f11;

	m[12] = f12;
	m[13] = f13;
	m[14] = f14;
	m[15] = f15;
}



void Mat4::identity()
{
	zeroMat();

	m[0] = 1.0f;
	m[5] = 1.0f;
	m[10] = 1.0f;
	m[15] = 1.0f;
}

void Mat4::zeroMat()
{
	for (int i = 0; i < 16; ++i)
	{
		m[i] = .0f;
	}
}


void Mat4::createOrthographicOffCenter(float left, float right, float top, float bottom, float nearZ, float farZ, Mat4& mt)
{
	zeroMat(mt);

	mt.m[0] = 2.0/(right-left);
	mt.m[5] = 2.0/(top-bottom);
	mt.m[10] = 2.0/(nearZ-farZ);

	mt.m[12] = -(right+left)/(right-left);
	mt.m[12] = -(top+bottom)/(top-bottom);
	mt.m[13] = (nearZ+farZ)/(nearZ-farZ);
}

void Mat4::createTranslation(float x, float y, float z, Mat4& mt)
{
	Mat4::identity(mt);

	mt.m[12] = x;
	mt.m[13] = y;
	mt.m[14] = z;
}

/*
m0	m1	m2	m3
m4  m5	m6	m7
m8	m9	m10	m11
m12	m13	m14	m15
*/
Mat4& Mat4::operator *= (Mat4& mt)
{
	float nm[16];
	nm[0] = m[0]*mt.m[0] + m[1]*mt.m[4] + m[2]*mt.m[8] + m[3]*mt.m[12];
	nm[1] = m[0]*mt.m[1] + m[1]*mt.m[5] + m[2]*mt.m[9] + m[3]*mt.m[13];
	nm[2] = m[0]*mt.m[2] + m[1]*mt.m[6] + m[2]*mt.m[10] + m[3]*mt.m[14];
	nm[3] = m[0]*mt.m[3] + m[1]*mt.m[7] + m[2]*mt.m[11] + m[3]*mt.m[15];

	nm[4] = m[4]*mt.m[0] + m[5]*mt.m[4] + m[6]*mt.m[8] + m[7]*mt.m[12];
	nm[5] = m[4]*mt.m[1] + m[5]*mt.m[5] + m[6]*mt.m[9] + m[7]*mt.m[13];
	nm[6] = m[4]*mt.m[2] + m[5]*mt.m[6] + m[6]*mt.m[10] + m[7]*mt.m[14];
	nm[7] = m[4]*mt.m[3] + m[5]*mt.m[7] + m[6]*mt.m[11] + m[7]*mt.m[15];

	nm[8] = m[8]*mt.m[0] + m[9]*mt.m[4] + m[10]*mt.m[8] + m[11]*mt.m[12];
	nm[9] = m[8]*mt.m[1] + m[9]*mt.m[5] + m[10]*mt.m[9] + m[11]*mt.m[13];
	nm[10] = m[8]*mt.m[2] + m[9]*mt.m[6] + m[10]*mt.m[10] + m[11]*mt.m[14];
	nm[11] = m[8]*mt.m[3] + m[9]*mt.m[7] + m[10]*mt.m[11] + m[11]*mt.m[15];

	nm[12] = m[12]*mt.m[0] + m[13]*mt.m[4] + m[14]*mt.m[8] + m[15]*mt.m[12];
	nm[13] = m[12]*mt.m[1] + m[13]*mt.m[5] + m[14]*mt.m[9] + m[15]*mt.m[13];
	nm[14] = m[12]*mt.m[2] + m[13]*mt.m[6] + m[14]*mt.m[10] + m[15]*mt.m[14];
	nm[15] = m[12]*mt.m[3] + m[13]*mt.m[7] + m[14]*mt.m[11] + m[15]*mt.m[15];

	for (int n=0; n<16; ++n)
	{
		m[n] = nm[n];
	}

	return *this;
}