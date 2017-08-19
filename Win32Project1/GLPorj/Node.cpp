#include "stdafx.h"
#include "Node.h"

Node::Node()
{
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

