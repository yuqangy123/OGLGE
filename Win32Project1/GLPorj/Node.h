#pragma once
#include "Vector3.h"
#include "Mat4.h"
#include "Quaternion.h"


class Node
{
public:
	Node();
	~Node();

	virtual void update(float ft) {};
	virtual void draw() {};
	virtual void keyInput(unsigned char param, int x, int y) {};
	virtual void mouseInput(int button, int state, int x, int y) {};

	void setPosition(const float x, const float y, const float z);

protected:
	Vector3 m_position;
	Mat4 m_transform;
	Quaternion m_rotation;
	float m_scaleX;
	float m_scaleY;
};

