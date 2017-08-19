#pragma once
class Mat4
{
public:
	Mat4();
	Mat4(float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15);
	~Mat4();

	Mat4& operator *= (Mat4& mt);

	void identity();
	void zeroMat();

	static void createOrthographicOffCenter(float left, float right, float top, float bottom, float nearZ, float farZ, Mat4& mt);
	static void createTranslation(float x, float y, float z, Mat4& mt);

public:
	float m[16];

public:
	/** equals to a matrix full of zeros */
    static const Mat4 ZERO;
    /** equals to the identity matrix */
    static const Mat4 IDENTITY;
};

