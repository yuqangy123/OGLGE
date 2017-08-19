#pragma once
#include "Camera.h"
#include "Mat3.h"
#include "Vector3.h"
#include "Rect.h"
#include "Quaternion.h"



class Camera3D : public Camera
{
public:
	Camera3D(const Vector3& pos, const Vector3& target, const Vector3& up);
	Camera3D();
	~Camera3D();

	//设置透视投影
	void SetPerspectivePro(float Fov, float Width, float Height, float zNear, float zFar);

	//相机平滑移动
	void slide(Vector3 m);
	void rotate(float angleH, float angleV);
	
	/*
	void pitch(float angle);
	void yaw(float angle);
	*/

	void setEyePosition(const Vector3& ps);
	void setTargetPosition(const Vector3& ps);
	void setUpPosition(const Vector3& ps);


	Matrix4f getCameraTranlation();

protected:
	void InitCameraTrans();
	void InitPerspectiveProj();
	void UpdateCameraRotation();
	void init();


	void rotate(const Quaternion& q);
	void rotate(const Vector3& axis, const float angle);
	void rotate(const Vector3& axis, const float angle, Quaternion& quat);

protected:
	Vector3 	m_eye;
	Vector3 	m_target;
	Vector3 	m_up;
	Mat3 		m_uvn;
	Quaternion	m_quat;
	float		m_angleH = 0;
	float		m_angleV = 0;
	Matrix4f	m_translateMat;
	Matrix4f	m_transWorldposMat;

	struct {
		float FOV;
		float Width;
		float Height;
		float zNear;
		float zFar;
	}m_persProj;
	Matrix4f	m_perspectiveMat;

	
	Matrix4f	m_cameraMat;

	

	
	
	//Quaternion m_rotation;
};

