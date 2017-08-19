#include "Quaternion.h"

Quaternion::Quaternion():x(0.0f),y(0.0f),z(0.0f),w(0.0f)
{

}

// normalising a quaternion works similar to a vector. This method will not do anything
// if the quaternion is close enough to being unit-length. define TOLERANCE as something
// small like 0.00001f to get accurate results
void Quaternion::normalise()
{
// Don't normalize if we don't have to
float mag2 = w * w + x * x + y * y + z * z;
if (  mag2!=0.f && (fabs(mag2 - 1.0f) > TOLERANCE)) {
float mag = sqrt(mag2);
w /= mag;
x /= mag;
y /= mag;
z /= mag;
}
}

//The complex conjugate of a quaternion

// We need to get the inverse of a quaternion to properly apply a quaternion-rotation to a vector
// The conjugate of a quaternion is the same as the inverse, as long as the quaternion is unit-length
Quaternion Quaternion::getConjugate()
{
return Quaternion(-x, -y, -z, w);
}

//Multiplying quaternions

// Multiplying q1 with q2 applies the rotation q2 to q1
Quaternion Quaternion::operator* (const Quaternion &rq) const
{
// the constructor takes its arguments as (x, y, z, w)
return Quaternion(w * rq.x + x * rq.w + y * rq.z - z * rq.y,
                 w * rq.y + y * rq.w + z * rq.x - x * rq.z,
                 w * rq.z + z * rq.w + x * rq.y - y * rq.x,
                 w * rq.w - x * rq.x - y * rq.y - z * rq.z);
}

//Rotating vectors

// Multiplying a quaternion q with a vector v applies the q-rotation to v
Vector3 Quaternion::operator* (const Vector3 &vec) const
{
Vector3 vn(vec);
vn.normalise();

Quaternion vecQuat, resQuat;
vecQuat.x = vn.x;
vecQuat.y = vn.y;
vecQuat.z = vn.z;
vecQuat.w = 0.0f;

resQuat = vecQuat * getConjugate();
resQuat = *this * resQuat;

return (Vector3(resQuat.x, resQuat.y, resQuat.z));
}

//How to convert to/from quaternions1
//Quaternion from axis-angle

// Convert from Axis Angle
void Quaternion::FromAxis(const Vector3 &v, float angle)
{
float sinAngle;
angle *= 0.5f;
Vector3 vn(v);
vn.normalise();

sinAngle = sin(angle);

x = (vn.x * sinAngle);
y = (vn.y * sinAngle);
z = (vn.z * sinAngle);
w = cos(angle);
}

//Quaternion from Euler angles


// Convert from Euler Angles
void Quaternion::FromEuler(float pitch, float yaw, float roll)
{
// Basically we create 3 Quaternions, one for pitch, one for yaw, one for roll
// and multiply those together.
// the calculation below does the same, just shorter

float p = pitch * PIOVER180 / 2.0;
float y = yaw * PIOVER180 / 2.0;
float r = roll * PIOVER180 / 2.0;

float sinp = sin(p);
float siny = sin(y);
float sinr = sin(r);
float cosp = cos(p);
float cosy = cos(y);
float cosr = cos(r);

this->x = sinr * cosp * cosy - cosr * sinp * siny;
this->y = cosr * sinp * cosy + sinr * cosp * siny;
this->z = cosr * cosp * siny - sinr * sinp * cosy;
this->w = cosr * cosp * cosy + sinr * sinp * siny;

normalise();
}

//Quaternion to Matrix

// Convert to Matrix
Mat4 Quaternion::getMatrix() const
{
float x2 = x * x;
float y2 = y * y;
float z2 = z * z;
float xy = x * y;
float xz = x * z;
float yz = y * z;
float wx = w * x;
float wy = w * y;
float wz = w * z;

// This calculation would be a lot more complicated for non-unit length quaternions
// Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
//   OpenGL
return Mat4( 1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
0.0f, 0.0f, 0.0f, 1.0f)
}


void Quaternion::getMatrix(Matrix4& mat) const
{
	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

	mat.m[0] = 1.0f - 2.0f * (y2 + z2);
	mat.m[1] = 2.0f * (xy - wz);
	mat.m[2] = 2.0f * (xz + wy);
	mat.m[3] = 0.0f;
	mat.m[4] = 2.0f * (xy + wz);
	mat.m[5] = 1.0f - 2.0f * (x2 + z2);
	mat.m[6] = 2.0f * (yz - wx);
	mat.m[7] = 0.0f;
	mat.m[8] = 2.0f * (xz - wy);
	mat.m[9] = 2.0f * (yz + wx);
	mat.m[10] = 1.0f - 2.0f * (x2 + y2);
	mat.m[11] = 0.0f;
	mat.m[12] = 0.0f;
	mat.m[13] = 0.0f;
	mat.m[14] = 0.0f;
	mat.m[15] = 0.0f;
}

//Quaternion to axis-angle

// Convert to Axis/Angles
void Quaternion::getAxisAngle(Vector3 *axis, float *angle)
{
float scale = sqrt(x * x + y * y + z * z);
axis->x = x / scale;
axis->y = y / scale;
axis->z = z / scale;
*angle = acos(w) * 2.0f;
}