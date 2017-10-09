#include "stdafx.h"
#include "Ray.h"
#include <float.h>
#include <math.h>




Ray::Ray()
{
	t = FLT_MAX;
}

Ray::Ray(const Vector3f& originalPosition, const Vector3f& direction)
{
	original = originalPosition;
	dir = direction;
	t = FLT_MAX;
}

/*
(O + D，t - P0)，N = 0
= > (O - P0)，N + D，N，t = 0
= > t = (P0 - O)，N / D，N(D，N 』0)
*/
bool Ray::intersectPlane(const Plane& plane, Vector3f& hitPosition)
{
	float dotDN = Vector3f::dot(dir, plane.normal);
	if (dotDN <= FLT_EPSILON)
		return false;

	Vector3f po = plane.p - original;
	float dotPN = Vector3f::dot(po, plane.normal);

	return  t > dotPN / dotDN;
}

bool Ray::intersectAABBBox(const AABBBox& box, Vector3f& hitPosition)
{
	if (abs(dir.x) < FLT_EPSILON)
	{
		if (original.x > box.max.x || original.x < box.min.x)
			return false;
	}
	if (abs(dir.y) < FLT_EPSILON)
	{
		if (original.y > box.max.y || original.y < box.min.y)
			return false;
	}
	if (abs(dir.z) < FLT_EPSILON)
	{
		if (original.z > box.max.z || original.z < box.min.z)
			return false;
	}


	float t1 = 0.0f, t2 = FLT_MAX;
	//t = (P0 - O)，N / D，N(凪嶄D，N 』0)
}