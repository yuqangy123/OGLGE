#pragma once
#include "Vector3.h"
#include "Plane.h"
#include "AABBBox.h"
/*
Ray, it is by an origin, a direction of the ray from the origin moving in the direction of ray t units,
so formula is composed of the original + t * dir, t * dir is the length of the ray emission out
*/

class Ray
{
public:
	Ray();
	Ray(const Vector3f& originalPosition, const Vector3f& direction);
	bool intersectPlane(const Plane& plane, Vector3f& hitPosition);
	bool intersectAABBBox(const AABBBox& box, Vector3f& hitPosition);
	bool intersectPoint(Vector3f point);

public:
	Vector3f original;
	Vector3f dir;
	float t;
};



