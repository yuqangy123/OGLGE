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
	dir.normalize();
	t = FLT_MAX;
}

/*
(O + D·t - P0)·N = 0
= > (O - P0)·N + D·N·t = 0
= > t = (P0 - O)·N / D·N(D·N ≠0)
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

//射线穿过轴对称包围盒，进入点的t(t1)值，在正对射线的三个候选面的交叉点里最大。离开点的t(t2)值，在其他背对射线的面里，t值最小。t1永远小于t2，交叉点才算在包围盒的六面体上
bool Ray::intersectAABBBox(const AABBBox& box, Vector3f& hitPosition)
{
	if (fabs(dir.x) < FLT_EPSILON)
	{
		if (original.x > box.max.x || original.x < box.min.x)
			return false;
	}
	if (fabs(dir.y) < FLT_EPSILON)
	{
		if (original.y > box.max.y || original.y < box.min.y)
			return false;
	}
	if (fabs(dir.z) < FLT_EPSILON)
	{
		if (original.z > box.max.z || original.z < box.min.z)
			return false;
	}

	//t = (P0 - O)·N / D·N(其中D·N ≠0)
	float t1 = 0.0f, t2 = FLT_MAX;
	float maxt, mint, tmp;

	//calculate x axis value
	mint = (box.min.x - original.x) / dir.x;
	maxt = (box.max.x - original.x) / dir.x;
	if (maxt<mint){ tmp = maxt; maxt = mint; mint = tmp; }
	if (mint > t1)t1 = mint;
	if (maxt < t2)t2 = maxt;
	if (t1 > t2)
		return false;

	//calculate y axis value
	mint = (box.min.y - original.y) / dir.y;
	maxt = (box.max.y - original.y) / dir.y;
	if (maxt<mint){ tmp = maxt; maxt = mint; mint = tmp; }
	if (mint > t1)t1 = mint;
	if (maxt < t2)t2 = maxt;
	if (t1 > t2)
		return false;

	//calculate z axis value
	mint = (box.min.z - original.z) / dir.z;
	maxt = (box.max.z - original.z) / dir.z;
	if (maxt<mint){ tmp = maxt; maxt = mint; mint = tmp; }
	if (mint > t1)t1 = mint;
	if (maxt < t2)t2 = maxt;
	if (t1 > t2)
		return false;

	//ensure t
	t = t1;
	hitPosition.x = original.x + t*dir.x;
	hitPosition.y = original.y + t*dir.y;
	hitPosition.z = original.z + t*dir.z;
	return true;
}

bool Ray::intersectPoint(Vector3f point)
{
	Vector3f pdir = point - original;
	pdir.normalize();

	return pdir == dir;
}