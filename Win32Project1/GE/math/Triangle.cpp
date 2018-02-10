#include "stdafx.h"
#include "Triangle.h"


Triangle::Triangle()
{

}
Triangle::Triangle(const Vector3f& pp1, const Vector3f& pp2, const Vector3f& pp3)
{
	p1 = pp1;
	p2 = pp2;
	p3 = pp3;

	
	Vector3f::cross(p2 - p1, p3 - p1, normal);
	normal.normalize();

	distance = -Vector3f::dot(normal, p1);
}