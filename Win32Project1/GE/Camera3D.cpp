

#include "Camera3D.h"
#include "base.h"
#include <math.h>
#include "OGLGE.h"


Camera3D::Camera3D(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	m_eye = pos;
	m_target = target;
	m_up = up;

	InitCameraTrans();

	m_dirty = true;

	m_freeCamera = false;
	m_mouseCenter = false;
	m_pMouseMoveFunc = nullptr;
	m_angleH =0.f;;
	m_angleV =0.f;;
}

Camera3D::Camera3D()
{
	m_dirty = true;

	m_freeCamera = false;
	m_mouseCenter = false;
	m_pMouseMoveFunc = nullptr;
	m_angleH =0.f;;
	m_angleV =0.f;;

}

Camera3D::~Camera3D()
{
}


void Camera3D::setEyePosition(const Vector3& ps)
{
	m_eye = ps;

	m_transWorldposMat.identity();
	m_transWorldposMat.m[0][3] = -m_eye.x;
	m_transWorldposMat.m[1][3] = -m_eye.y;
	m_transWorldposMat.m[2][3] = -m_eye.z;

	m_dirty = true;
}

void Camera3D::setEyePosition(const float& x, const float& y, const float& z)
{
	m_eye.x = x;
	m_eye.y = y;
	m_eye.z = z;

	m_transWorldposMat.identity();
	m_transWorldposMat.m[0][3] = -m_eye.x;
	m_transWorldposMat.m[1][3] = -m_eye.y;
	m_transWorldposMat.m[2][3] = -m_eye.z;


	m_dirty = true;
}

void Camera3D::setTargetPosition(const Vector3& ps)
{
	m_target = ps;

	InitCameraTrans();
	m_dirty = true;
}

void Camera3D::setUpPosition(const Vector3& ps)
{
	m_up = ps;
}



void Camera3D::init()
{
	
			//Mat4::createOrthographicOffCenter(m_viewOriginalPostion.x, m_viewOriginalPostion.x+m_viewWidth, 
			//							m_viewOriginalPostion.y+m_viewHeight, m_viewOriginalPostion.y, 
			//								-1024.0, 1024.0, m_transition);

			//m_rotation.FromEuler(0.0, 0.0, 0.0);

			//setEyePosition(Vector3(m_viewOriginalPostion.x+ m_viewWidth /2.0, m_viewOriginalPostion.y+ m_viewHeight /2.0, 10));
	
	
}

void Camera3D::InitCameraTrans()
{
	m_translateMat.identity();

	Vector3 Z = m_eye-m_target;
	Z.normalize();

	Vector3 Y = m_up;
	Y.normalize();

	Vector3 X;
	Vector3::cross(Y, Z, X);

	Vector3::cross(Z, X, Y);

	m_uvn.zero();
	m_uvn.m[0][0] = X.x;
	m_uvn.m[0][1] = X.y;
	m_uvn.m[0][2] = X.z;
	m_uvn.m[1][0] = Y.x;
	m_uvn.m[1][1] = Y.y;
	m_uvn.m[1][2] = Y.z;
	m_uvn.m[2][0] = Z.x;
	m_uvn.m[2][1] = Z.y;
	m_uvn.m[2][2] = Z.z;

	m_translateMat.m[0][0] = X.x; m_translateMat.m[0][1] = X.y; m_translateMat.m[0][2] = X.z;
	m_translateMat.m[1][0] = Y.x; m_translateMat.m[1][1] = Y.y; m_translateMat.m[1][2] = Y.z;
	m_translateMat.m[2][0] = Z.x; m_translateMat.m[2][1] = Z.y; m_translateMat.m[2][2] = Z.z;
 	//m_translateMat.m[3][0] = Vector3::dot(m_eye, X);
 	//m_translateMat.m[3][1] = Vector3::dot(m_eye, Y);
 	//m_translateMat.m[3][2] = Vector3::dot(m_eye, Z);

	m_transWorldposMat.identity();
	m_transWorldposMat.m[0][3] = -m_eye.x;
	m_transWorldposMat.m[1][3] = -m_eye.y;
	m_transWorldposMat.m[2][3] = -m_eye.z;

	m_quat = m_quat.zero();
	Quaternion::createFromRotationMatrix(m_translateMat, &m_quat);

	m_stand_uvn = m_uvn;

	m_dirty = true;
}

const Matrix4f& Camera3D::getCameraTranlation()
{
	if (m_dirty)
	{
		m_cameraMat = m_perspectiveMat * m_translateMat * m_transWorldposMat;
		m_dirty = true;
	}
	
	return m_cameraMat;
}

void Camera3D::SetPerspectivePro(float Fov, float Width, float Height, float zNear, float zFar)
{
	m_persProj.FOV = Fov;
	m_persProj.Width = Width;
	m_persProj.Height = Height;
	m_persProj.zNear = zNear;
	m_persProj.zFar = zFar;
	InitPerspectiveProj();

	m_dirty = true;
}

void Camera3D::rotate(const Quaternion& q)
{
	m_quat = q * m_quat;
}
void Camera3D::rotate(const Vector3& axis, const float angle)
{
	Quaternion::createFromAxisAngle(axis, MATH_DEG_TO_RAD(angle), &m_quat);
	//Quaternion::createFromAxisAngle(axis, angle, &q);

	//Mat4 quatMt4;
	//q.getMatrix(&quatMt4);

	//rotate(q);
}

void Camera3D::rotate(const Vector3& axis, const float angle, Quaternion& quat)
{
	Quaternion::createFromAxisAngle(axis, MATH_DEG_TO_RAD(angle), &quat);
}

/*
void Camera3D::pitch(float angle)
{
	return;
	printf("Camera3D::pitch.angle=%f\r\n", angle);
	Vector3 xAxis(m_uvn.m[0][0], m_uvn.m[0][1], m_uvn.m[0][2]);
	Vector3 yAxis(m_uvn.m[1][0], m_uvn.m[1][1], m_uvn.m[1][2]);
	Vector3 zAxis;
	rotate(yAxis, angle);

	xAxis = m_quat * xAxis;
	xAxis.normalize();

	Vector3::cross(xAxis, yAxis, zAxis);
	zAxis.normalize();

	m_uvn.m[0][0] = xAxis.x;
	m_uvn.m[0][1] = xAxis.y;
	m_uvn.m[0][2] = xAxis.z;
	m_uvn.m[1][0] = yAxis.x;
	m_uvn.m[1][1] = yAxis.y;
	m_uvn.m[1][2] = yAxis.z;
	m_uvn.m[2][0] = zAxis.x;
	m_uvn.m[2][1] = zAxis.y;
	m_uvn.m[2][2] = zAxis.z;
	UpdateCameraRotation();
}
void Camera3D::yaw(float angle)
{
	
	Vector3 xAxis(m_uvn.m[0][0], m_uvn.m[0][1], m_uvn.m[0][2]);
	Vector3 yAxis(m_uvn.m[1][0], m_uvn.m[1][1], m_uvn.m[1][2]);
	Vector3 zAxis;
	rotate(xAxis, angle);

	yAxis = m_quat * yAxis;
	yAxis.normalize();

	Vector3::cross(xAxis, yAxis, zAxis);
	zAxis.normalize();

	m_uvn.m[0][0] = xAxis.x;
	m_uvn.m[0][1] = xAxis.y;
	m_uvn.m[0][2] = xAxis.z;
	m_uvn.m[1][0] = yAxis.x;
	m_uvn.m[1][1] = yAxis.y;
	m_uvn.m[1][2] = yAxis.z;
	m_uvn.m[2][0] = zAxis.x;
	m_uvn.m[2][1] = zAxis.y;
	m_uvn.m[2][2] = zAxis.z;

	UpdateCameraRotation();
}
*/
void Camera3D::slide(Vector3 m)
{
	m_eye += m;
	m_target += m;

	InitCameraTrans();
	m_dirty = true;
}

void Camera3D::rotate(float angleH, float angleV)
{
	m_angleH += angleH;
	m_angleV += angleV;

	Vector3 xAxis(m_stand_uvn.m[0][0], m_stand_uvn.m[0][1], m_stand_uvn.m[0][2]);
	Vector3 yAxis(m_stand_uvn.m[1][0], m_stand_uvn.m[1][1], m_stand_uvn.m[1][2]);
	Vector3 zAxis(m_stand_uvn.m[2][0], m_stand_uvn.m[2][1], m_stand_uvn.m[2][2]);
	
	Quaternion HQuat, VQuat, addQuat;
	rotate(yAxis, m_angleH, HQuat);
	rotate(xAxis, m_angleV, VQuat);
	addQuat = HQuat*VQuat;
	m_quat = addQuat;

	zAxis = m_quat * zAxis;
	zAxis.normalize();

	//Z = m_eye-m_target;
	m_target = m_eye - zAxis;
	
	yAxis = m_quat * yAxis;
	yAxis.normalize();
	
	xAxis = m_quat * xAxis;
	xAxis.normalize();
	
	//InitCameraTrans();
	

	//printf("%f,%f,%f,%f,%f,%f,%f,%f,%f,\r\n", xAxis.x, xAxis.y, xAxis.z, yAxis.x, yAxis.y, yAxis.z, zAxis.x, zAxis.y, zAxis.z);
	//Vector3::cross(xAxis, zAxis, yAxis);
	
	m_uvn.m[0][0] = xAxis.x;
	m_uvn.m[0][1] = xAxis.y;
	m_uvn.m[0][2] = xAxis.z;
	m_uvn.m[1][0] = yAxis.x;
	m_uvn.m[1][1] = yAxis.y;
	m_uvn.m[1][2] = yAxis.z;
	m_uvn.m[2][0] = zAxis.x;
	m_uvn.m[2][1] = zAxis.y;
	m_uvn.m[2][2] = zAxis.z;
	
	UpdateCameraRotation();
	m_dirty = true;
}


void Camera3D::UpdateCameraRotation()
{
	m_translateMat.m[0][0] = m_uvn.m[0][0]; m_translateMat.m[0][1] = m_uvn.m[0][1]; m_translateMat.m[0][2] = m_uvn.m[0][2];
	m_translateMat.m[1][0] = m_uvn.m[1][0]; m_translateMat.m[1][1] = m_uvn.m[1][1]; m_translateMat.m[1][2] = m_uvn.m[1][2];
	m_translateMat.m[2][0] = m_uvn.m[2][0]; m_translateMat.m[2][1] = m_uvn.m[2][1]; m_translateMat.m[2][2] = m_uvn.m[2][2];
	
// m_translateMat.m[3][0] = Vector3::dot(m_eye, Vector3(m_uvn.m[0][0], m_uvn.m[0][1], m_uvn.m[0][2]));
// m_translateMat.m[3][1] = Vector3::dot(m_eye, Vector3(m_uvn.m[1][0], m_uvn.m[1][1], m_uvn.m[1][2]));
// m_translateMat.m[3][2] = Vector3::dot(m_eye, Vector3(m_uvn.m[2][0], m_uvn.m[2][1], m_uvn.m[2][2]));
}

void Camera3D::InitPerspectiveProj()
{
	const float ar = m_persProj.Width / m_persProj.Height;
	const float zNear = -m_persProj.zNear;
	const float zFar = -m_persProj.zFar;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(MATH_DEG_TO_RAD(m_persProj.FOV / 2));

	Matrix4f &mat = m_perspectiveMat;
	mat.m[0][0] = 1.0f / (tanHalfFOV * ar); mat.m[0][1] = 0.0f;            mat.m[0][2] = 0.0f;                   mat.m[0][3] = 0.0;
	mat.m[1][0] = 0.0f;                   mat.m[1][1] = 1.0f / tanHalfFOV; mat.m[1][2] = 0.0f;                   mat.m[1][3] = 0.0;
	mat.m[2][0] = 0.0f;                   mat.m[2][1] = 0.0f;            mat.m[2][2] = (-zNear - zFar) / zRange; mat.m[2][3] = 2.0f * zFar*zNear / zRange;
	mat.m[3][0] = 0.0f;                   mat.m[3][1] = 0.0f;            mat.m[3][2] = -1.0f;                   mat.m[3][3] = 0.0;

}

void Camera3D::setFreeCamera(bool b)
{
	if (b)
	{
		m_pMouseMoveFunc = static_cast<mousemoveFunc>(&Camera3D::mousemove_free);
		auto winsz = OGLGE::Instance()->getWindowsRect();
		m_mouseMovelastX = winsz.width / 2.0;
		m_mouseMovelastY = winsz.height / 2.0;
	}
	else
	{
		m_pMouseMoveFunc = nullptr;
		m_mouseCenter = false;
	}
	m_freeCamera = b;
}

void Camera3D::setMouseCenterAlways(bool b)
{
	m_mouseCenter = b;
	if (b)
	{
		m_pMouseMoveFunc = static_cast<mousemoveFunc>(&Camera3D::mousemove_free_mousecenter);
		auto winsz = OGLGE::Instance()->getWindowsRect();
		m_mouseMovelastX = winsz.width / 2.0;
		m_mouseMovelastY = winsz.height / 2.0;
		glutWarpPointer(m_mouseMovelastX, m_mouseMovelastY);
	}
	else
	{
		m_pMouseMoveFunc = static_cast<mousemoveFunc>(&Camera3D::mousemove_free);
	}
}

void Camera3D::keyInput(unsigned char param, int x, int y) 
{
	if (!m_freeCamera)
		return;
	float factor = 0.5f;
	if (param == 's')
	{
		setEyePosition(m_eye.x + m_uvn.m[2][0] * factor, m_eye.y + m_uvn.m[2][1] * factor, m_eye.z + m_uvn.m[2][2] * factor);
		auto eyePos = getEyePosition();
		auto target = getTargetPosition();
		//printf("%f,%f,%f***%f,%f,%f\r\n", eyePos.x, eyePos.y, eyePos.z, target.x, target.y, target.z);
	}
	if (param == 'w')
	{
		setEyePosition(m_eye.x - m_uvn.m[2][0] * factor, m_eye.y - m_uvn.m[2][1] * factor, m_eye.z - m_uvn.m[2][2] * factor);
		auto eyePos = getEyePosition();
		auto target = getTargetPosition();
		//printf("%f,%f,%f***%f,%f,%f\r\n", eyePos.x, eyePos.y, eyePos.z, target.x, target.y, target.z);
	}
	if (param == 'a')
	{
		setEyePosition(m_eye.x - m_uvn.m[0][0] * factor, m_eye.y - m_uvn.m[0][1] * factor, m_eye.z - m_uvn.m[0][2] * factor);
		auto eyePos = getEyePosition();
		auto target = getTargetPosition();
		//printf("%f,%f,%f***%f,%f,%f\r\n", eyePos.x, eyePos.y, eyePos.z, target.x, target.y, target.z);
	}
	if (param == 'd')
	{		
		setEyePosition(m_eye.x + m_uvn.m[0][0] * factor, m_eye.y + m_uvn.m[0][1] * factor, m_eye.z + m_uvn.m[0][2] * factor);
		auto eyePos = getEyePosition();
		auto target = getTargetPosition();
		//printf("%f,%f,%f***%f,%f,%f\r\n", eyePos.x, eyePos.y, eyePos.z, target.x, target.y, target.z);
	}
};
void Camera3D::mouseInput(int button, int state, int x, int y)
{
	if (nullptr != m_pMouseMoveFunc)
		(this->*m_pMouseMoveFunc)(button, state, x, y);
}
void Camera3D::mousemove_free(int button, int state, int x, int y)
{
	float rx = 0.0, ry = 0.0;
	bool bchange = false;
	float sp = 20.0;
	if (abs(y - m_mouseMovelastY) > 2)
	{
		if (m_mouseCenter) glutWarpPointer(m_mouseMovelastX, m_mouseMovelastY);
		ry = (m_mouseMovelastY - y) / sp;
		bchange = true;
	}
	if (abs(x - m_mouseMovelastX) > 2)
	{
		if (m_mouseCenter) glutWarpPointer(m_mouseMovelastX, m_mouseMovelastY);
		rx = (m_mouseMovelastX - x) / sp;
		bchange = true;
	};
	if (bchange)
	{
		rotate(rx, ry);
		m_mouseMovelastX = x;
		m_mouseMovelastY = y;
	}
}

void Camera3D::mousemove_free_mousecenter(int button, int state, int x, int y)
{
	float rx = 0.0, ry = 0.0;
	bool bchange = false;
	float sp = 20.0;
	if (abs(y - m_mouseMovelastY) > 2)
	{
		if (m_mouseCenter) glutWarpPointer(m_mouseMovelastX, m_mouseMovelastY);
		ry = (m_mouseMovelastY - y) / sp;
		bchange = true;
	}
	if (abs(x - m_mouseMovelastX) > 2)
	{
		if (m_mouseCenter) glutWarpPointer(m_mouseMovelastX, m_mouseMovelastY);
		rx = (m_mouseMovelastX - x) / sp;
		bchange = true;
	};
	if (bchange)
	{
		rotate(rx, ry);
		auto winsz = OGLGE::Instance()->getWindowsRect();
		if (m_mouseCenter) glutWarpPointer(m_mouseMovelastX, m_mouseMovelastY);
	}
}

 

/*
//相机自旋转即绕u,v,n轴旋转，uvn坐标系*旋转矩阵
void Camera3D::pitch(float angle) 
{
	float cs = cos(MATH_DEG_TO_RAD(angle));
	float sn = sin(MATH_DEG_TO_RAD(angle));
	
 	
 	//Vector3d t(n);  
    //Vector3d s(u);  
    //n.x() = cs*t.x()-sn*s.x();  
    //n.y() = cs*t.y()-sn*s.y();  
    //n.z() = cs*t.z()-sn*s.z();  
  
    //u.x() = sn*t.x()+cs*s.x();  
    //u.y() = sn*t.y()+cs*s.y();  
    //u.z() = sn*t.z()+cs*s.z();  
	float nx = cs*m_uvn.m[2][0] + sn*m_uvn.m[0][0];
	float ny = cs*m_uvn.m[2][1] + sn*m_uvn.m[0][1];
	float nz = cs*m_uvn.m[2][2] + sn*m_uvn.m[0][2];

	float ux = sn*m_uvn.m[2][0] + cs*m_uvn.m[0][0];
	float uy = sn*m_uvn.m[2][1] + cs*m_uvn.m[0][1];
	float uz = sn*m_uvn.m[2][2] + cs*m_uvn.m[0][2];
	
	m_uvn.m[0][0] = ux;
	m_uvn.m[0][1] = uy;
	m_uvn.m[0][2] = uz;
	m_uvn.m[2][0] = nx;
	m_uvn.m[2][1] = ny;
	m_uvn.m[2][2] = nz;
	
	UpdateCameraRotation();
	
}

void Camera3D::yaw(float angle)
{
	float cs = cos(MATH_DEG_TO_RAD(angle));
	float sn = sin(MATH_DEG_TO_RAD(angle));

	//Vector3d t(v);  
    //Vector3d s(n);  
  
    //v.x() = cs*t.x()-sn*s.x();  
    //v.y() = cs*t.y()-sn*s.y();  
    //v.z() = cs*t.z()-sn*s.z();  
  
    //n.x() = sn*t.x()+cs*s.x();  
    //n.y() = sn*t.y()+cs*s.y();  
    //n.z() = sn*t.z()+cs*s.z();   
	

    float vx = cs*m_uvn.m[1][0] + sn*m_uvn.m[2][0];
	float vy = cs*m_uvn.m[1][1] + sn*m_uvn.m[2][1];
	float vz = cs*m_uvn.m[1][2] + sn*m_uvn.m[2][2];

	float nx = sn*m_uvn.m[1][0] + cs*m_uvn.m[2][0];
	float ny = sn*m_uvn.m[1][1] + cs*m_uvn.m[2][1];
	float nz = sn*m_uvn.m[1][2] + cs*m_uvn.m[2][2];
	
	m_uvn.m[1][0] = vx;
	m_uvn.m[1][1] = vy;
	m_uvn.m[1][2] = vz;
	m_uvn.m[2][0] = nx;
	m_uvn.m[2][1] = ny;
	m_uvn.m[2][2] = nz;

	UpdateCameraRotation();
}

*/

