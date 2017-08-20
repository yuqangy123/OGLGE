#include "stdafx.h"
#include "Mat4.h"
#include <math.h>
#include "Quaternion.h"

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
	identity();
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



void Mat4::createTranslation(float x, float y, float z, Mat4& mt)
{
	mt.identity();

	mt.m[12] = x;
	mt.m[13] = y;
	mt.m[14] = z;
}

void Mat4::createPerspectiveProj(float fov, float aspectRatio, float nearZ, float farZ, Mat4& mt)
{
	const float tanHalfFOV = tanf(MATH_DEG_TO_RAD(fov / 2.0));
	float zRange = farZ - nearZ;
	float factor = 1.0 / tanHalfFOV;
	mt.m[0] = 1.0f / (tanHalfFOV * aspectRatio);
	mt.m[1] = 0.0f;
	mt.m[2] = 0.0f;
	mt.m[3] = 0.0f;
	mt.m[4] = 0.0f;
	mt.m[5] = 1.0f / tanHalfFOV;
	mt.m[6] = 0.0f;
	mt.m[7] = 0.0f;
	mt.m[8] = 0.0f;
	mt.m[9] = 0.0f;
	mt.m[10] = (-nearZ - farZ) / zRange;
	mt.m[11] = -1.0f;
	mt.m[12] = 0.0f;
	mt.m[13] = 0.0f;
	mt.m[14] = -2.0f * farZ * nearZ / zRange;
	mt.m[15] = 0.0f;
}

/*
m0	m1	m2	m3
m4  m5	m6	m7
m8	m9	m10	m11
m12	m13	m14	m15
*/
Mat4& Mat4::operator *= (const Mat4& mt)
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

Mat4 Mat4::operator * (const Mat4& mt)
{
	return operator*=(mt);
}
void Mat4::scale(float sx, float sy, float sz)
{
	Mat4 s; (sx, 0.f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, sz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	*this *= s;
}

void Mat4::transpose()
{
	float tmp = 0.0f;
	for (int a = 0; a < 3; ++a)
	{
		for (int b = a+1; b < 4; ++b)
		{
			tmp = m[a * 4 + b];
			m[a * 4 + b] = m[b * 4 + a];
			m[b * 4 + a] = tmp;
		}
	}
}

void Mat4::lookAtMt4(const Vector3& eye, const Vector3& target, const Vector3& upVector3, Mat4& mt)
{
	Vector3 up(upVector3.x, upVector3.y, upVector3.z);
	up.normalize();

	Vector3 zaxis;
	Vector3::subtract(eye, target, zaxis);
	zaxis.normalize();

	Vector3 xaxis;
	Vector3::cross(up, zaxis, xaxis);
	xaxis.normalize();

	Vector3 yaxis;
	Vector3::cross(zaxis, xaxis, yaxis);
	yaxis.normalize();

	mt.m[0] = xaxis.x;
	mt.m[1] = yaxis.x;
	mt.m[2] = zaxis.x;
	mt.m[3] = 0.0f;

	mt.m[4] = xaxis.y;
	mt.m[5] = yaxis.y;
	mt.m[6] = zaxis.y;
	mt.m[7] = 0.0f;

	mt.m[8] = xaxis.z;
	mt.m[9] = yaxis.z;
	mt.m[10] = zaxis.z;
	mt.m[11] = 0.0f;

	mt.m[12] = -Vector3::dot(xaxis, eye);
	mt.m[13] = -Vector3::dot(yaxis, eye);
	mt.m[14] = -Vector3::dot(zaxis, eye);
	mt.m[15] = 1.0f;
}


float Matrix4f::determinant() const
{
	float m[16] = {};
	int mi = 0;
	for (int i = 1; i < 4; ++i)
		for (int j = 1; j < 4; ++j)
		{
			m[mi++] = this->m[i][j];
		}
	float a0 = m[0] * m[5] - m[1] * m[4];
	float a1 = m[0] * m[6] - m[2] * m[4];
	float a2 = m[0] * m[7] - m[3] * m[4];
	float a3 = m[1] * m[6] - m[2] * m[5];
	float a4 = m[1] * m[7] - m[3] * m[5];
	float a5 = m[2] * m[7] - m[3] * m[6];
	float b0 = m[8] * m[13] - m[9] * m[12];
	float b1 = m[8] * m[14] - m[10] * m[12];
	float b2 = m[8] * m[15] - m[11] * m[12];
	float b3 = m[9] * m[14] - m[10] * m[13];
	float b4 = m[9] * m[15] - m[11] * m[13];
	float b5 = m[10] * m[15] - m[11] * m[14];

	// Calculate the determinant.
	return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
}

bool Matrix4f::decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const
{
	if (translation)
	{
		// Extract the translation.
		translation->x = m[3][0];
		translation->y = m[3][1];
		translation->z = m[3][2];
	}

	// Nothing left to do.
	if (scale == nullptr && rotation == nullptr)
		return true;

	// Extract the scale.
	// This is simply the length of each axis (row/column) in the matrix.
	Vector3 xaxis(m[0][0], m[0][1], m[0][2]);
	float scaleX = xaxis.mag();

	Vector3 yaxis(m[1][0], m[1][1], m[1][2]);
	float scaleY = yaxis.mag();

	Vector3 zaxis(m[2][0], m[2][1], m[2][2]);
	float scaleZ = zaxis.mag();

	// Determine if we have a negative scale (true if determinant is less than zero).
	// In this case, we simply negate a single axis of the scale.
	float det = determinant();
	if (det < 0)
		scaleZ = -scaleZ;

	if (scale)
	{
		scale->x = scaleX;
		scale->y = scaleY;
		scale->z = scaleZ;
	}

	// Nothing left to do.
	if (rotation == nullptr)
		return true;

	// Scale too close to zero, can't decompose rotation.
	if (scaleX < MATH_TOLERANCE || scaleY < MATH_TOLERANCE || fabsf(scaleZ) < MATH_TOLERANCE)
		return false;

	float rn;

	// Factor the scale out of the matrix axes.
	rn = 1.0f / scaleX;
	xaxis.x *= rn;
	xaxis.y *= rn;
	xaxis.z *= rn;

	rn = 1.0f / scaleY;
	yaxis.x *= rn;
	yaxis.y *= rn;
	yaxis.z *= rn;

	rn = 1.0f / scaleZ;
	zaxis.x *= rn;
	zaxis.y *= rn;
	zaxis.z *= rn;

	// Now calculate the rotation from the resulting matrix (axes).
	float trace = xaxis.x + yaxis.y + zaxis.z + 1.0f;

	if (trace > MATH_EPSILON)
	{
		float s = 0.5f / sqrt(trace);
		rotation->w = 0.25f / s;
		rotation->x = (yaxis.z - zaxis.y) * s;
		rotation->y = (zaxis.x - xaxis.z) * s;
		rotation->z = (xaxis.y - yaxis.x) * s;
	}
	else
	{
		// Note: since xaxis, yaxis, and zaxis are normalized, 
		// we will never divide by zero in the code below.
		if (xaxis.x > yaxis.y && xaxis.x > zaxis.z)
		{
			float s = 0.5f / sqrt(1.0f + xaxis.x - yaxis.y - zaxis.z);
			rotation->w = (yaxis.z - zaxis.y) * s;
			rotation->x = 0.25f / s;
			rotation->y = (yaxis.x + xaxis.y) * s;
			rotation->z = (zaxis.x + xaxis.z) * s;
		}
		else if (yaxis.y > zaxis.z)
		{
			float s = 0.5f / sqrt(1.0f + yaxis.y - xaxis.x - zaxis.z);
			rotation->w = (zaxis.x - xaxis.z) * s;
			rotation->x = (yaxis.x + xaxis.y) * s;
			rotation->y = 0.25f / s;
			rotation->z = (zaxis.y + yaxis.z) * s;
		}
		else
		{
			float s = 0.5f / sqrt(1.0f + zaxis.z - xaxis.x - yaxis.y);
			rotation->w = (xaxis.y - yaxis.x) * s;
			rotation->x = (zaxis.x + xaxis.z) * s;
			rotation->y = (zaxis.y + yaxis.z) * s;
			rotation->z = 0.25f / s;
		}
	}

	return true;
}


void Matrix4f::createOrthographicOffCenter(float left, float right, float top, float bottom, float nearZ, float farZ, Matrix4f& mt)
{
	mt.identity();

	mt.m[0][0] = 2.0 / (right - left);
	mt.m[1][1] = 2.0 / (top - bottom);
	mt.m[2][2] = 2.0 / (nearZ - farZ);

	//mt.m[3][0] = -(right + left) / (right - left);
	//mt.m[3][1] = -(top + bottom) / (top - bottom);
	mt.m[3][0] = 0;
	mt.m[3][1] = 0;
	mt.m[3][2] = (nearZ + farZ) / (nearZ - farZ);
	mt.m[3][3] = 1.0f;

}