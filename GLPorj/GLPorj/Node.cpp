#include "stdafx.h"
#include "Node.h"

Node::Node()
{
}


Node::~Node()
{
}

const Mat4& Node::getTransformMatrix()
{
	m_rotation.getMatrix(m_transform);

	Mat4 translation;
	Mat4::createTranslation(m_position.x, m_position.y, m_position.z, translation);

	m_transform *= translation;

	return m_transform;
}
