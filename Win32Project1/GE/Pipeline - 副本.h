#pragma once
#include "Vector3.h"
#include "Mat4.h"

typedef struct
{
	float fov;
	float width;
	float height;
	float znear;
	float zfar;
}perspective;

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	void Scale(float ScaleX, float ScaleY, float ScaleZ);
	void WorldPos(float x, float y, float z);
	void Rotate(float RotateX, float RotateY, float RotateZ);
	void setPerspectiveProj(float fov, float width, float height,float znear, float zfar);//Õ∏ ”Õ∂”∞
	void GetTrans(Matrix4f& mt4);

protected:
	void initPerspectiveProj(Matrix4f& m)const;
	void InitRotateTrans(Matrix4f& mat);

private:
	Vector3 m_scale;
	Vector3 m_worldPos;
	Vector3 m_rotateInfo;
	perspective m_persProj;

	Matrix4f m_transformation;
};

