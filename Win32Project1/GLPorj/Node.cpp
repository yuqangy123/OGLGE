#include "stdafx.h"
#include "Node.h"

Node::Node()
{
	m_scaleX = 1.0f;
	m_scaleY = 1.0f;
}


Node::~Node()
{
}

void Node::setPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

