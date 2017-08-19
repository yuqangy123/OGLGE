#pragma once
#include "Mat4.h"
#include "Vector3.h"
#include "Rect.h"
#include "Quaternion.h"


typdef enum dimension
{
	d2d,
	d3d
};

class Camera
{
public:
	Camera(float width, float height, const Vector3& pos, const Vector3& target, const Vector3& up);
	~Camera();

	void setViewRect(const Rect& viewRt);
	void setEyePosition(const Vector3& ps);
	void setViewOriginalPosition(const Vector3& ps);
	void setDimension(dimension dim);
	void initDefaultResult();

	void update();

protected:
	void init3d();

protected:
	Vector3 	m_eye;
	Vector3 	m_target;
	Vector3 	m_up;
	float		m_viewWidth;
	float		m_viewHeight;
	
	Mat3 		m_uvn;//uvn

	dimension m_dimension;
	Vector3 	m_eye;
	Rect m_viewRect;
	Vector3 m_viewOriginalPostion;
	Mat4 m_transition;
	Quaternion m_rotation;
};

