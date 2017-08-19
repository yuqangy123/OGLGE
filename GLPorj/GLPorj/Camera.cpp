#include "stdafx.h"
#include "Camera2D.h"

const float pi = 3.1415926;

Camera::Camera()(float width, float height, const Vector3& pos, const Vector3& target, const Vector3& up);
{
	m_eye = pos;
	m_target = m_target;
	m_up = up;
	m_viewWidth = width;
	m_viewHeight = height;	
}



Camera::~Camera()
{
}

void Camera::setDimension(dimension dim)
{
	m_dimension = dim;
}

void Camera::setViewRect(const Rect& viewRt)
{
	m_viewRect = viewRt;
}

void Camera::setEyePosition(const Vector3& ps)
{
	m_eye = ps;
}

void Camera::setViewOriginalPosition(const Vector3& ps)
{
	m_viewOriginalPostion = ps;
}


void Camera::initDefaultResult()
{
	switch(m_dimension)
	{
		case dimension::d2d:{
			Mat4::createOrthographicOffCenter(m_viewOriginalPostion.x, m_viewOriginalPostion.x+m_viewRect.width, 
										m_viewOriginalPostion.y+m_viewRect.height, m_viewOriginalPostion.y, 
											-1024.0, 1024.0, m_transition);

			m_rotation.FromEuler(0.0, 0.0, 0.0);

			setEyePosition(Vector3(m_viewOriginalPostion.x+m_viewRect.width/2.0, m_viewOriginalPostion.y+m_viewRect.height/2.0, 10));
		}break;

		case dimension::d3d:{

		}break;
	}
	
}

void Camera::init3d()
{
	Vector3 HTarget(m_target.x, 0, m_target.z);
	HTarget.normalize();

	if(m_target.z >= 0.0)
	{
		if (m_target.x >= 0.0)
		{
			m_angleH = 360.0 - toDegree(asin(m_target.z));
		}
		else
		{
			m_angleH = 180.0 + toDegree(asin(m_target.z));
		}
	}
	else
	{
		if (m_target.x >= 0.0)
		{
			m_angleH = toDegree(asin(-m_target.z));
		}
		else
		{
			m_angleH = 90.0 + toDegree(asin(-m_target.z));
		}
	}

	Vector3 VTarget(0, m_target.y, 0);
	VTarget.normalize();
	m_angleV = toDegree(asin(VTarget.y));

	m_mousePos.x = m_viewWidth/2.0f;
	m_mousePos.y = m_viewHeight/2.0f;
}

void Camera:slide(Vector3 m)
{
	Vector3 res = m_uvn*m;
	m_eye += res;
	m_target += res;
}

//相机自旋转即绕u,v,n轴旋转，uvn坐标系*旋转矩阵
void Camera::pitch(float angle) 
{
	float cs = cos(angle*p/180.0);
	float sn = sin(angle*p/180.0);

 	/*
 	Vector3d t(n);  
    Vector3d s(u);  
    n.x() = cs*t.x()-sn*s.x();  
    n.y() = cs*t.y()-sn*s.y();  
    n.z() = cs*t.z()-sn*s.z();  
  
    u.x() = sn*t.x()+cs*s.x();  
    u.y() = sn*t.y()+cs*s.y();  
    u.z() = sn*t.z()+cs*s.z();  */
	float nx = cs*m_uvn[2][0] + sn*m_uvn[0][0];
	float ny = cs*m_uvn[2][1] + sn*m_uvn[0][1];
	float nz = cs*m_uvn[2][2] + sn*m_uvn[0][2];

	float ux = sn*m_uvn[2][0] + cs*m_uvn[0][0];
	float uy = sn*m_uvn[2][1] + cs*m_uvn[0][1];
	float uz = sn*m_uvn[2][2] + cs*m_uvn[0][2];
	
	m_uvn[0][0] = ux;
	m_uvn[0][1] = uy;
	m_uvn[0][2] = uz;
	m_uvn[2][0] = nx;
	m_uvn[2][1] = ny;
	m_uvn[2][2] = nz;
}

void Camera::pitch(float angle)
{
	float cs = cos(angle*p/180.0);
	float sn = sin(angle*p/180.0);

	/*
	Vector3d t(v);  
    Vector3d s(n);  
  
    v.x() = cs*t.x()-sn*s.x();  
    v.y() = cs*t.y()-sn*s.y();  
    v.z() = cs*t.z()-sn*s.z();  
  
    n.x() = sn*t.x()+cs*s.x();  
    n.y() = sn*t.y()+cs*s.y();  
    n.z() = sn*t.z()+cs*s.z();   
	*/

    float vx = cs*m_uvn[1][0] + sn*m_uvn[2][0];
	float vy = cs*m_uvn[1][1] + sn*m_uvn[2][1];
	float vz = cs*m_uvn[1][2] + sn*m_uvn[2][2];

	float nx = sn*m_uvn[1][0] + cs*m_uvn[2][0];
	float ny = sn*m_uvn[1][1] + cs*m_uvn[2][1];
	float nz = sn*m_uvn[1][2] + cs*m_uvn[2][2];
	
	m_uvn[1][0] = vx;
	m_uvn[1][1] = vy;
	m_uvn[1][2] = vz;
	m_uvn[2][0] = nx;
	m_uvn[2][1] = ny;
	m_uvn[2][2] = nz;
}


void Camera::update()
{
	
}


