#include "stdafx.h"
#include "meshObject.h"
#include "anim.h"
#include "Importer.hpp"
#include "postprocess.h"
#include "TextureManager.h"
#include "OGLGE.h"
#include "InPutControl.h"
#include "common.h"

meshObject::meshObject()
{
	init();
}


meshObject::~meshObject()
{
	InPutControlIns.removeListenNode(this);

}

void meshObject::init()
{
	m_draw_framebox = false;

	m_tech = new lightTechnique();
	m_tech->init(lightType::diffuseLight);
	m_tech->enable();

	m_MVPMt4.identity();

	m_pipe.setCamera(DefaultCamera);
	DefaultCamera->setFreeCamera(true);
	DefaultCamera->setMouseCenterAlways(true);

	InPutControlIns.addListenNode(this);

	m_ambientLightColor.set(0.7f, 0.7f, 0.7f);
	m_ambientLightIntensity = 0.5f;
	m_diffuseDirection.set(1.0f, 1.0f, -1.9f);
	m_diffuseDirection.normalize();
	m_diffuseIntensity = 1.0f;

	m_mesh = new ModelMesh();
	m_mesh->setAttriPositionLoc(m_tech->positionLoc);
	m_mesh->setAttriTexCoordLoc(m_tech->texCoordLoc);
	m_mesh->setAttriNormalLoc(m_tech->normalLoc);
}

void meshObject::draw()
{
	m_tech->enable();


	glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_MVPMt4.m);
	const Matrix4f* worldMt4 = m_pipe.GetWorldTrans();
	glUniformMatrix4fv(m_tech->getUniformLocation("WorldMatrix"), 1, GL_TRUE, (const float*)worldMt4->m);
	glUniform3f(m_tech->getUniformLocation("gDiffuseLight.color"), m_ambientLightColor.x, m_ambientLightColor.y, m_ambientLightColor.z);
	glUniform1f(m_tech->getUniformLocation("gDiffuseLight.ambientIntensity"), m_ambientLightIntensity);
	glUniform3f(m_tech->getUniformLocation("gDiffuseLight.direction"), m_diffuseDirection.x, m_diffuseDirection.y, m_diffuseDirection.z);
	glUniform1f(m_tech->getUniformLocation("gDiffuseLight.diffuseIntensity"), m_diffuseIntensity);

	m_mesh->draw();

	return;

	m_tech->disable();
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(m_world_point.x, m_world_point.y, 0);
	glEnd();
	glPointSize(1);
	
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 0);
	glVertex2f(m_line_a_pos.x, m_line_a_pos.y);
	glVertex2f(m_line_b_pos.x, m_line_b_pos.y);
	glEnd();

}

void meshObject::update(float ft)
{
	m_MVPMt4 = *m_pipe.GetTrans();
}

void meshObject::setPosition(float x, float y, float z)
{
	m_pipe.WorldPos(x, y, z);
}

void meshObject::setScale(float s)
{
	m_pipe.Scale(s, s, s);
}

bool meshObject::loadMesh(const char* filename, const char* filepath)
{
	bool res = m_mesh->loadMesh(filename, filepath);

	return res;
}

void meshObject::setDrawBoundbox(bool visible)
{
	if (m_mesh)
		m_mesh->setDrawBoundbox(visible);
}


void meshObject::keyInput(unsigned char param, int x, int y)
{
	float factor = 0.5f;
	if (param == 'p')
	{
	}
	if (param == 'o')
	{
	}
}

void meshObject::mouseInput(int button, int state, int x, int y)
{
	return;
	if (button == 0 && state == 0)
	{
		const Rect& winsz = OGLGE::Instance()->getWindowsRect();
		float znear = 0.1f; 
		float zfar = 100.f;
		float a = (zfar + znear) / (zfar - znear);
		float b = (2 * zfar*znear) / ((zfar - znear)*-znear);

		//求ndc坐标（z值是相反值）
		float ndc_x = ((float)x / winsz.width)*2.0f - 1.0f;
		float ndc_y = 1.0f - (2.0f*(float)y)/winsz.height;
		float ndc_z = -1.f;
		Vector4 ndc_screen_point(ndc_x, ndc_y, ndc_z, 1.0f);
		//printf("ndc:%f,%f,%f\r\n", ndc_x, ndc_y, ndc_z);

		GLfloat pixz = 0.f;
		glReadPixels(x,y,1, 1, GL_DEPTH_COMPONENT,GL_FLOAT,&pixz);
		printf("pixz:%f\r\n", pixz);
		

		Matrix4f mvpMt4 = *m_pipe.GetTrans();
		mvpMt4.transpose();

		Vector4 clip_point;
		Vector4 moudel_point = Vector4(100, 0, 0, 1.0);
		clip_point = mvpMt4*moudel_point;
		clip_point.normalizieW();
		
		//m_world_point = vpMt4*clip_point;
		printf("clip_point:%f,%f,%f,%f\r\n", clip_point.x, clip_point.y, clip_point.z, clip_point.w);

		//转换到裁剪坐标系
		clip_point = mvpMt4*moudel_point;

		mvpMt4.Inverse();
		Vector4 inverseWorldPoint = mvpMt4*clip_point;
		printf("inverseWorldPoint:%f,%f,%f,%f\r\n", inverseWorldPoint.x, inverseWorldPoint.y, inverseWorldPoint.z, inverseWorldPoint.w);


		Matrix4f cameraTransMt4 = DefaultCamera->getCameraTranlation();
		Matrix4f cameraPerspectiveMt4 = DefaultCamera->getPerspectiveMt4();
		cameraTransMt4.transpose();
		cameraTransMt4.Inverse();

		//到ndc空间
		clip_point.normalizieW();
		clip_point = ndc_screen_point;//这里改为测试屏幕点击位置的ndc坐标
		Vector4 clip_2_world_pos = cameraTransMt4*clip_point;
		clip_2_world_pos.x /= clip_2_world_pos.w;
		clip_2_world_pos.y /= clip_2_world_pos.w;
		clip_2_world_pos.z /= clip_2_world_pos.w;
		clip_2_world_pos.w = 1.0f;

		

		//clip_2_world_pos = pipeModelTransMt4*clip_2_world_pos;
		printf("clip_2_world_pos:%f,%f,%f,%f\r\n", clip_2_world_pos.x, clip_2_world_pos.y, clip_2_world_pos.z, clip_2_world_pos.w);

		cameraTransMt4 = DefaultCamera->getCameraTranlation();
		cameraTransMt4.transpose();
		clip_point = cameraTransMt4*clip_2_world_pos;
		clip_point.normalizieW();

		//模型世界转换矩阵
		Matrix4f worldMt4 = *m_pipe.GetWorldTrans();
		worldMt4.transpose();

		AABBBox meshBox;
		m_mesh->getAABBBox(meshBox);
		Vector4f maxPos;
		Vector4f minPos;

		VTC3_TO_VTC4(meshBox.max, maxPos);
		VTC3_TO_VTC4(meshBox.min, minPos);

		maxPos = worldMt4*maxPos;
		maxPos.normalizieW();
		minPos = worldMt4*minPos;
		minPos.normalizieW();

		VTC4_TO_VTC3(maxPos, meshBox.max);
		VTC4_TO_VTC3(minPos, meshBox.min);

		Vector4f dir = clip_2_world_pos;
		dir = dir - VTC4_REF_VTC3(DefaultCamera->getEyePosition());
		Vector3 eyePos = DefaultCamera->getEyePosition();
		Ray ray(eyePos, VTC3_REF_VTC4(dir));
		Vector3f hitPosition;

		ray.original = Vector3f(0, 10, 0);
		ray.dir = Vector3f(0, 0, -1);
		hitPosition = Vector3f(0, 10, 0);
		bool res = ray.intersectAABBBox(meshBox, hitPosition);

		cameraTransMt4 = DefaultCamera->getCameraTranlation();
		cameraTransMt4.transpose();
		Vector4f hitPositionv4;
		VTC3_TO_VTC4(hitPosition, hitPositionv4);
		hitPositionv4 = cameraTransMt4*hitPositionv4;
		hitPositionv4.normalizieW();

		m_world_point.x = hitPositionv4.x;
		m_world_point.y = hitPositionv4.y;

		
		VTC3_TO_VTC4(eyePos, m_line_a_pos);
		m_line_b_pos = clip_2_world_pos;
		cameraTransMt4 = DefaultCamera->getCameraTranlation();
		cameraTransMt4.transpose();
		m_line_a_pos = cameraTransMt4*m_line_a_pos;
		m_line_a_pos.normalizieW();
		m_line_b_pos = cameraTransMt4*m_line_b_pos;
		m_line_b_pos.normalizieW();
		
		int n = 0;
		printf("\r\n\r\n");
	}
}


