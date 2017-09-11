#include "stdafx.h"
#include "Camera2D.h"
#include "common.h"
#include <math.h>



Camera2D::Camera2D(const Vector2& viewOriginalPos, float w, float h)
{
	m_viewOriginalPostion = viewOriginalPos;
	m_viewWidth = w;
	m_viewHeight = h;
	init();
	m_dirty = true;
}

Camera2D::~Camera2D()
{
}


void Camera2D::init()
{
	Matrix4f::createOrthographicOffCenter(m_viewOriginalPostion.x, m_viewOriginalPostion.x+m_viewWidth,
								m_viewOriginalPostion.y+m_viewHeight, m_viewOriginalPostion.y, 
									-1024.0, 1024.0, m_transition);
	//m_transition.transpose();
}

const Matrix4f& Camera2D::getCameraTranlation()
{
	return m_transition;
}

