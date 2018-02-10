#include "Vector3.h"

struct Vertex
{
	Vertex(const Vector3f& pos_, const Vector3f& uv_, const Vector3f& normal_)
	{
		pos = pos_;
		uv = uv_;
		normal = normal_;
	}

	Vector3f pos;
	Vector3f uv;
	Vector3f normal;
};

