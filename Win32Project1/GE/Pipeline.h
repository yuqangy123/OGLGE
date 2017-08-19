#pragma once
#include "Vector3.h"
#include "Mat4.h"
#include "Camera.h"

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	void setCamera(Camera* p) { m_camera = p; }

	void Scale(float ScaleX, float ScaleY, float ScaleZ)
	{
		m_scale.x = ScaleX;
		m_scale.y = ScaleY;
		m_scale.z = ScaleZ;
	}

	void WorldPos(float x, float y, float z)
	{
		m_worldPos.x = x;
		m_worldPos.y = y;
		m_worldPos.z = z;
	}

	void Rotate(float RotateX, float RotateY, float RotateZ)
	{
		m_rotation.x = RotateX;
		m_rotation.y = RotateY;
		m_rotation.z = RotateZ;
	}



	const Matrix4f* GetTrans();

private:
	void InitScaleTrans(Matrix4f& mat);
	void InitRotateTrans(Matrix4f& mat);
	void InitTranslationTrans(Matrix4f& mat);
	


	Vector3 m_scale;
	Vector3 m_rotation;
	Vector3 m_worldPos;

	Camera* m_camera=nullptr;
	bool		m_dirty = true;
	Matrix4f m_transformation;
};