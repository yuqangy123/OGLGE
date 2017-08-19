#pragma once
#include "Vector3.h"
#include "common.h"


class Quaternion;
class Matrix4f;
class Mat4
{
public:
	Mat4();
	Mat4(float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15);
	~Mat4();

	Mat4& operator *= (const Mat4& mt);
	Mat4 operator *(const Mat4& mt) const;

	void identity();
	void zeroMat();
	void transpose();
	Mat4 operator*(const Mat4 & Right);
	void scale(float sx = 1.0f, float sy = 1.0f, float sz = 1.0f);

	static void createTranslation(float x, float y, float z, Mat4& mt);
	static void createPerspectiveProj(float fov, float aspectRatio, float nearZ, float farZ, Mat4& mt);
	static void lookAtMt4(const Vector3& eye, const Vector3& target, const Vector3& upVec3, Mat4& mt);
	
	
public:
	float m[16];

public:
	/** equals to a matrix full of zeros */
	static const Mat4 ZERO;
	/** equals to the identity matrix */
	static const Mat4 IDENTITY;
};



class Matrix4f
{
public:
	float m[4][4];
	Matrix4f::Matrix4f()
	{
		for (int x = 0; x < 4; ++x)
			for (int y = 0; y < 4; ++y)
			{
				m[x][y] = 0.f;
			}
	}
	inline Matrix4f operator*(const Matrix4f& Right)
	{
		Matrix4f Ret;

		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				Ret.m[i][j] = m[i][0] * Right.m[0][j] + m[i][1] * Right.m[1][j] +
					m[i][2] * Right.m[2][j] + m[i][3] * Right.m[3][j];
			}
		}

		return Ret;
	}

	inline void identity()
	{
		for (int x = 0; x < 4; ++x)
			for (int y = 0; y < 4; ++y)
			{
				m[x][y] = 0.f;
			}
		m[0][0] = 1.0;
		m[1][1] = 1.0;
		m[2][2] = 1.0;
		m[3][3] = 1.0;
	}

	bool decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const;
	float determinant() const;

	bool getRotation(Quaternion* rotation) const
	{
		return decompose(nullptr, rotation, nullptr);
	}

	static void createOrthographicOffCenter(float left, float right, float top, float bottom, float nearZ, float farZ, Matrix4f& mt);
};
