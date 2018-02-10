#include "stdafx.h"
#include "Line.h"
#include <float.h>
#include <math.h>



LineSegment::LineSegment()
{

}
LineSegment::LineSegment(const Vector3f& p1, const Vector3f& p2)
{
	this->p1 = p1;
	this->p2 = p2;

	Vector3 tmpDir = p2 - p1;
	dir = tmpDir;
	dir.normalize();

	t = tmpDir.mag()/dir.mag();
}

bool LineSegment::coplane(const LineSegment& object)
{
	Vector3f l2 = object.p2 - p1;
	Vector3f cro;
	Vector3f::cross(p2, l2, cro);

	float dt = Vector3f::dot(cro, object.p1);
	return dt <= FLT_EPSILON;
}

bool LineSegment::intersectLine(const LineSegment& line)
{
	if (coplane(line))
		return line.dir != dir;
	return false;
}