#include "stdafx.h"
#include "Plane.h"

Plane::Plane()
{

}
Plane::Plane(const Vector3f& n, const Vector3f& dotPosition)
{
	normal = n;
	p = dotPosition;
}