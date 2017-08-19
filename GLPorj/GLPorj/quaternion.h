#pragma once
#include "Mat4.h"



class Quaternion
{
public:
	Quaternion();
	void normalise();
	Quaternion getConjugate();
	Quaternion operator* (const Quaternion &rq) const;
	Vector3 operator* (const Vector3 &vec) const;
	void FromAxis(const Vector3 &v, float angle);
	void FromEuler(float pitch, float yaw, float roll);
	Mat4 getMatrix() const;
	void getAxisAngle(Vector3 *axis, float *angle);




public:
	float x;
	float y;
	float z;
	float w;
};