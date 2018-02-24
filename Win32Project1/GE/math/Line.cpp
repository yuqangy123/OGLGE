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

Vector3 LineSegment::getPointAt(float yt)
{
	return p1 + dir*yt;
}

bool LineSegment::intersectPoint(const Vector3& point)
{
	float t = point.x - p1.x;
	if (point != p1*t)
		return false;

	t = point.y - p1.y;
	if (point != p1*t)
		return false;

	t = point.z - p1.z;
	if (point != p1*t)
		return false;

	return true;
}

bool LineSegment::coplane(const LineSegment& object)
{
	Vector3f l2 = object.p2 - p1;
	Vector3f cro;
	Vector3f::cross(p2, l2, cro);

	float dt = Vector3f::dot(cro, object.p1);
	return dt <= FLT_EPSILON;
}

/*
p1+d1u = q1+d2s
(p1+d1u)*s = q1*s
p1*s+d1u*s = q1*s
d1 = ((q1-p1)*s)/(u*s)
*/
bool LineSegment::intersectLine(const LineSegment& line, Vector3& interPoint)
{
	bool bCoplane = coplane(line);

	if (bCoplane)
	{
 		if (line.dir == dir)
		{
			if (intersectPoint(line.p1))//¹²Ïß
				return true;
		}

		vtr3 pline(line.p1 - this->p1);
		vtr3 crossA;
		vtr3::cross(pline, line.dir, crossA);

		vtr3 crossB;
		vtr3::cross(this->dir, line.dir, crossB);
		
		float c1 = crossA.mag();
		float c2 = crossB.mag();
		float d = c1 / c2;
		if (d <= this->t)
		{
			interPoint = p1 + dir*d;
			return true;
		}
		return false;
	}
	
	return false;
}