#include "stdafx.h"
#include "Pipeline.h"
#include <math.h>
#include "common.h"
#include "OGLGE.h"

#define M_PI 3.14159265
#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

Pipeline::Pipeline()
{
	m_scale = Vector3(1.0f, 1.0f, 1.0f);
	m_rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_worldPos = Vector3(0.0f, 0.0f, 0.0f);

	m_camera=nullptr;
	m_dirty = false;
}


Pipeline::~Pipeline()
{
}

void Pipeline::InitScaleTrans(Matrix4f& mat)
{
	mat.m[0][0] = m_scale.x;		mat.m[0][1] = 0.0f;							mat.m[0][2] = 0.0f;					mat.m[0][3] = 0.0f;
	mat.m[1][0] = 0.0f;					mat.m[1][1] = m_scale.y;				mat.m[1][2] = 0.0f;					mat.m[1][3] = 0.0f;
	mat.m[2][0] = 0.0f;					mat.m[2][1] = 0.0f;							mat.m[2][2] = m_scale.z;		mat.m[2][3] = 0.0f;
	mat.m[3][0] = 0.0f;					mat.m[3][1] = 0.0f;							mat.m[3][2] = 0.0f;					mat.m[3][3] = 1.0f;
}
void Pipeline::InitRotateTrans(Matrix4f& mat)
{
	Matrix4f rx, ry, rz;

	const float x = ToRadian(m_rotation.x);
	const float y = ToRadian(m_rotation.y);
	const float z = ToRadian(m_rotation.z);

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

	mat = rz * ry * rx;
}

void Pipeline::InitTranslationTrans(Matrix4f& mat)
{
	mat.m[0][0] = 1.0f; mat.m[0][1] = 0.0f; mat.m[0][2] = 0.0f; mat.m[0][3] = m_worldPos.x;
	mat.m[1][0] = 0.0f; mat.m[1][1] = 1.0f; mat.m[1][2] = 0.0f; mat.m[1][3] = m_worldPos.y;
	mat.m[2][0] = 0.0f; mat.m[2][1] = 0.0f; mat.m[2][2] = 1.0f; mat.m[2][3] = m_worldPos.z;
	mat.m[3][0] = 0.0f; mat.m[3][1] = 0.0f; mat.m[3][2] = 0.0f; mat.m[3][3] = 1.0f;
}




const Matrix4f* Pipeline::GetTrans()
{
	if (!m_dirty && !m_camera->dirty())
		return &m_transformation;

	m_dirty = false;
	Matrix4f ScaleTrans, RotateTrans, TranslationTrans, cameraTrans;
	InitScaleTrans(ScaleTrans);
	InitRotateTrans(RotateTrans);
	InitTranslationTrans(TranslationTrans);
	
	cameraTrans = m_camera->getCameraTranlation();
	m_worldtrans = TranslationTrans* RotateTrans* ScaleTrans;
	m_transformation = cameraTrans*m_worldtrans;
	return &m_transformation;
}

const Matrix4f* Pipeline::GetWorldTrans()
{
	return &m_worldtrans;
}

const Matrix4f* Pipeline::getVPTrans()
{
	return &m_camera->getCameraTranlation();
}