#include "stdafx.h"
#include "Pipeline.h"
#include <math.h>
#include "common.h"

Pipeline::Pipeline()
{
	Scale(1.0, 1.0, 1.0);
}
Pipeline::~Pipeline()
{
	
}

void Pipeline::Scale(float ScaleX, float ScaleY, float ScaleZ)
{
	m_scale.x = ScaleX;
	m_scale.y = ScaleY;
	m_scale.z = ScaleZ;
}

void Pipeline::WorldPos(float x, float y, float z)
{
	m_worldPos.x = x;
	m_worldPos.y = y;
	m_worldPos.z = z;
}

void Pipeline::Rotate(float RotateX, float RotateY, float RotateZ)
{
	m_rotateInfo.x = RotateX;
	m_rotateInfo.y = RotateY;
	m_rotateInfo.z = RotateZ;
}

void Pipeline::GetTrans(Matrix4f& mt4)
{
	Matrix4f translationTrans;
	translationTrans.identity();
	translationTrans.m[0][3] = m_worldPos.x;
	translationTrans.m[1][3] = m_worldPos.y;
	translationTrans.m[2][3] = m_worldPos.z;

	Matrix4f rotateTrans;
	InitRotateTrans(rotateTrans);

	Matrix4f scaleTrans;
	scaleTrans.identity();
	scaleTrans.m[0][0] = m_scale.x;
	scaleTrans.m[1][1] = m_scale.y;
	scaleTrans.m[2][2] = m_scale.z;

	Matrix4f persProjTrans;
	initPerspectiveProj(persProjTrans);
	//persProjTrans.identity();
	mt4 = persProjTrans * translationTrans * rotateTrans * scaleTrans;
}

void Pipeline::setPerspectiveProj(float fov, float width, float height, float znear, float zfar)
{
	m_persProj.fov = fov;
	m_persProj.width = width;
	m_persProj.height = height;
	m_persProj.znear = znear;
	m_persProj.zfar = zfar;
}

void Pipeline::initPerspectiveProj(Matrix4f& mat)const
{
	const float ar = m_persProj.width / m_persProj.height;
	const float zNear = -m_persProj.znear;
	const float zFar = -m_persProj.zfar;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(MATH_DEG_TO_RAD(m_persProj.fov / 2.0));
	
	mat.m[0][0] = 1.0f / (tanHalfFOV * ar); mat.m[0][1] = 0.0f;            mat.m[0][2] = 0.0f;                   mat.m[0][3] = 0.0;
	mat.m[1][0] = 0.0f;                   mat.m[1][1] = 1.0f / tanHalfFOV; mat.m[1][2] = 0.0f;                   mat.m[1][3] = 0.0;
	mat.m[2][0] = 0.0f;                   mat.m[2][1] = 0.0f;            mat.m[2][2] = (-zNear - zFar) / zRange; mat.m[2][3] = 2.0f * zFar*zNear / zRange;
	mat.m[3][0] = 0.0f;                   mat.m[3][1] = 0.0f;            mat.m[3][2] = -1.0f;                   mat.m[3][3] = 0.0;
}

void Pipeline::InitRotateTrans(Matrix4f& m)
{
	Matrix4f rx, ry, rz;

	const float x = m_rotateInfo.x;
	const float y = m_rotateInfo.y;
	const float z = m_rotateInfo.z;

	rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f; rx.m[0][2] = 0.0f; rx.m[0][3] = 0.0f;
	rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x); rx.m[1][2] = -sinf(x); rx.m[1][3] = 0.0f;
	rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x); rx.m[2][2] = cosf(x); rx.m[2][3] = 0.0f;
	rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f; rx.m[3][2] = 0.0f; rx.m[3][3] = 1.0f;

	ry.m[0][0] = cosf(y); ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(y); ry.m[0][3] = 0.0f;
	ry.m[1][0] = 0.0f; ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f; ry.m[1][3] = 0.0f;
	ry.m[2][0] = sinf(y); ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y); ry.m[2][3] = 0.0f;
	ry.m[3][0] = 0.0f; ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f; ry.m[3][3] = 1.0f;

	rz.m[0][0] = cosf(z); rz.m[0][1] = -sinf(z); rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
	rz.m[1][0] = sinf(z); rz.m[1][1] = cosf(z); rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
	rz.m[2][0] = 0.0f; rz.m[2][1] = 0.0f; rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
	rz.m[3][0] = 0.0f; rz.m[3][1] = 0.0f; rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

	m = rz * ry * rx;
}