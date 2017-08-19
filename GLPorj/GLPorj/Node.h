#pragma once
#include "Vector3.h"
#include "Mat4.h"
#include "Quaternion.h"


class Node
{
public:
	Node();
	~Node();
	const Mat4& Node::getTransformMatrix();

protected:
	Vector3 m_position;
	Mat4 m_transform;
	Quaternion m_rotation;
};

