#pragma once
#include "Camera.h"
#include "Mat4.h"
#include "Vector2.h"


class Camera2D : public Camera
{
public:
	Camera2D(const Vector2& viewOriginalPos, float w, float h);
	~Camera2D();
	
	const Matrix4f& getCameraTranlation();

protected:
	void init();

protected:
	Vector2		m_viewOriginalPostion;
	float		m_viewWidth;
	float		m_viewHeight;
	Matrix4f	m_transition;


	
	Matrix4f	m_Camera2DMat;
};

