#pragma once
#include "Camera.h"
#include "Mat3.h"
#include "Vector3.h"
#include "Rect.h"
#include "Quaternion.h"
#include "InPutInterface.h"


class Camera3D : public Camera , public InPutInterface
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
	void setEyePosition(const float& x, const float& y, const float& z);
	void setTargetPosition(const Vector3& ps);
	void setUpPosition(const Vector3& ps);

	const Vector3& getTargetPosition() { return m_target; }
	const Vector3& getEyePosition() { return m_eye; }

	const Matrix4f& getCameraTranlation();

	void setFreeCamera(bool b);
	void setMouseCenterAlways(bool b);

	//获得投影矩阵
	Matrix4f getPerspectiveMt4() { return m_perspectiveMat; }

protected:
	void InitCameraTrans();
	void InitPerspectiveProj();
	void UpdateCameraRotation();
	void init();


	void rotate(const Quaternion& q);
	void rotate(const Vector3& axis, const float angle);
	void rotate(const Vector3& axis, const float angle, Quaternion& quat);

	void keyInput(unsigned char param, int x, int y);
	void mouseInput(int button, int state, int x, int y);
	void mousemove_free(int button, int state, int x, int y);
	void mousemove_free_mousecenter(int button, int state, int x, int y);

protected:
	Vector3 	m_eye;
	Vector3 	m_target;
	Vector3 	m_up;
	Mat3 		m_uvn;
	Mat3 		m_stand_uvn;
	Quaternion	m_quat;
	float		m_angleH ;
	float		m_angleV ;
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

	bool		m_freeCamera;
	bool		m_mouseCenter;

	float m_mouseMovelastX;
	float m_mouseMovelastY;
	
	mousemoveFunc m_pMouseMoveFunc;
	//Quaternion m_rotation;
};

