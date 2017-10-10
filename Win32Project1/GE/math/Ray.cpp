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
(O + D��t - P0)��N = 0
= > (O - P0)��N + D��N��t = 0
= > t = (P0 - O)��N / D��N(D��N ��0)
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

//���ߴ�����Գư�Χ�У�������t(t1)ֵ�����������ߵ�������ѡ��Ľ����������뿪���t(t2)ֵ���������������ߵ����tֵ��С��t1��ԶС��t2�����������ڰ�Χ�е���������
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

	//t = (P0 - O)��N / D��N(����D��N ��0)
	float t1 = 0.0f, t2 = FLT_MAX;
	float maxt, mint;

	//calculate x axis value
	if (original.x > box.min.x)
	{
		mint = (box.min.x - original.x) / dir.x;
		maxt = (box.max.x - original.x) / dir.x;
	}
	else
	{
		mint = (box.max.x - original.x) / dir.x;
		maxt = (box.min.x - original.x) / dir.x;
	}
	if (mint > t1)t1 = mint;
	if (maxt < t2)t2 = maxt;
	if (t1 > t2)
		return false;

	//calculate y axis value
	if (original.y > box.min.y)
	{
		mint = (box.min.y - original.y) / dir.y;
		maxt = (box.max.y - original.y) / dir.y;
	}
	else
	{
		mint = (box.max.y - original.y) / dir.y;
		maxt = (box.min.y - original.y) / dir.y;
	}
	if (mint > t1)t1 = mint;
	if (maxt < t2)t2 = maxt;
	if (t1 > t2)
		return false;

	//calculate z axis value
	if (original.z > box.min.z)
	{
		mint = (box.min.z - original.z) / dir.z;
		maxt = (box.max.z - original.z) / dir.z;
	}
	else
	{
		mint = (box.max.z - original.z) / dir.z;
		maxt = (box.min.z - original.z) / dir.z;
	}
	if (mint > t1)t1 = mint;
	if (maxt < t2)t2 = maxt;
	if (t1 > t2)
		return false;

	//ensure t
	hitPosition.x = original.x + t*dir.x;
	hitPosition.y = original.y + t*dir.y;
	hitPosition.z = original.z + t*dir.z;
	return true;
}