#include "stdafx.h"
#include "assert.h"
#include "triangleVertebralLight.h"
#include "FileUnits.h"
#include <stddef.h> //used offsetof
#include <algorithm>
#include <OGLGE.h>
#include "Vector3.h"
#include "OGLGE.h"
#include "TextureManager.h"





static float modelZ = 1.0f;
static float rotateY = 0.0f;
static float worldPosZ = -5.0f;
static Vector3 cameraWorldPos;
static Vector3 cameraLookAt(0, 0, -1);

triangleVertebralLight::triangleVertebralLight()
{
	init();
}


triangleVertebralLight::~triangleVertebralLight()
{
	if (m_tech) delete m_tech;
}


void triangleVertebralLight::CreateVertexBuffer()
{
	Vector3 Vertices[8];
	Vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
	Vertices[1] = Vector3(0.0f, 0.0f, 0.0f);

	Vertices[2] = Vector3(0.0f, -1.0f, modelZ);
	Vertices[3] = Vector3(0.5f, 0.0f, 0.0f);

	Vertices[4] = Vector3(1.0f, -1.0f, 0.0f);
	Vertices[5] = Vector3(1.0f, 0.0f, 1.0f);

	Vertices[6] = Vector3(0.0f, 1.0f, 0.0f);
	Vertices[7] = Vector3(0.5f, 1.0f, 1.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

void triangleVertebralLight::CreateIndexBuffer()
{
	unsigned int Indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}



void triangleVertebralLight::init()
{
	m_lightType = lightType::ambientLight;
	m_tech = new lightTechnique();
	m_tech->init(m_lightType);

	m_ambientLightColor.set(0.0, 0.5, 0.5);
	m_ambientLightIntensity = 1.0;

	auto winsz = OGLGE::Instance()->getWindowsRect();
	m_lastMouseX = winsz.width / 2.0;
	m_lastMouseY = winsz.height / 2.0;
	glutWarpPointer(m_lastMouseX, m_lastMouseY);



	m_worldMt4.identity();

	m_pipe.setCamera(DefaultCamera);

	//m_pipe.Scale(0.25, 0.25, 0.25);
	m_pipe.WorldPos(0, 0, -5.0);

	m_exturesID = TextureMgr->loadTextureJpeg("res/gezi.jpg", GL_RGBA, GL_RGBA, 0, 0);

	// Set the filtering mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
	CreateVertexBuffer();
	CreateIndexBuffer();
}

void triangleVertebralLight::update(float ft) {

	m_worldMt4 = *m_pipe.GetTrans();
}

void triangleVertebralLight::keyInput(unsigned char param, int x, int y)
{
	if (param == 'a')
	{
		rotateY += 5.f;
		m_pipe.Rotate(0, rotateY, 0);
	}
	else if (param == 's')
	{
		rotateY -= 5.f;
		m_pipe.Rotate(0, rotateY, 0);
	}
	else if (param == 'w')
	{
		worldPosZ += 0.05;
		m_pipe.WorldPos(0, 0, worldPosZ);
	}
	else if (param == 'q')
	{
		worldPosZ -= 0.05;
		m_pipe.WorldPos(0, 0, worldPosZ);
	}
	else if (param == 'z')
	{
		modelZ -= 0.1f;
		CreateVertexBuffer();
		printf("modelZ=%f\r\n", modelZ);
	}
	else if (param == 'x')
	{
		modelZ += 0.1f;
		CreateVertexBuffer();
		printf("modelZ=%f\r\n", modelZ);
	}
	else if (param == 'r')
	{
		DefaultCamera->slide(Vector3(0.0, 0.0, 0.1));
	}
	else if (param == 'f')
	{
		DefaultCamera->slide(Vector3(0.0, 0.0, -0.1));
	}
	else if (param == 'd')
	{
		DefaultCamera->slide(Vector3(-0.1, 0.0, 0.0));
	}
	else if (param == 'g')
	{
		DefaultCamera->slide(Vector3(0.1, 0.0, 0.0));
	}
	else if (param == 'e')
	{
		DefaultCamera->slide(Vector3(0.0, -0.1, 0.0));
	}
	else if (param == 't')
	{
		DefaultCamera->slide(Vector3(0.0, 0.1, 0.0));
	}
	else if (param == 'o')
	{
		m_ambientLightColor.set(std::max(0.0f, m_ambientLightColor.x - 0.05f), std::max(0.0f, m_ambientLightColor.y - 0.05f), std::max(0.0f, m_ambientLightColor.z - 0.05f));
	}
	else if (param == 'p')
	{
		m_ambientLightColor.set(std::min(1.5f, m_ambientLightColor.x + 0.05f), std::min(1.5f, m_ambientLightColor.y + 0.05f), std::min(1.5f, m_ambientLightColor.z + 0.05f));
	}
	else if (param == 'k')
	{
		m_ambientLightIntensity = std::max(0.0f, m_ambientLightIntensity - 0.05f);
	}
	else if (param == 'l')
	{
		m_ambientLightIntensity = std::max(1.5f, m_ambientLightIntensity + 0.05f);
	}


}

void triangleVertebralLight::mouseInput(int button, int state, int x, int y)
{
	float rx = 0.0, ry = 0.0;

	float sp = 20.0;
	if (abs(y - m_lastMouseY) > 2)
	{
		glutWarpPointer(m_lastMouseX, m_lastMouseY);
		ry = (m_lastMouseY - y) / sp;
		//printf("y:%d,%d\r\n", y, m_lastMouseY);
		//DefaultCamera->yaw((m_lastMouseY - y)/10.0);
	}
	if (abs(x - m_lastMouseX) > 2)
	{
		glutWarpPointer(m_lastMouseX, m_lastMouseY);
		rx = (m_lastMouseX - x) / sp;
		//printf("x:%d,%d\r\n", x, m_lastMouseX);
		//DefaultCamera->pitch((m_lastMouseX - x) / 10.0);
	};
	DefaultCamera->rotate(rx, ry);
}

void triangleVertebralLight::updateLightUniform()
{
	switch (m_lightType)
	{
		case lightType::ambientLight:
		{
			glUniform3f(m_tech->getUniformLocation("gDirectionLight.color"), m_ambientLightColor.x, m_ambientLightColor.y, m_ambientLightColor.z);
			glUniform1f(m_tech->getUniformLocation("gDirectionLight.ambientIntensity"), m_ambientLightIntensity);
		}break;
	}
}

void triangleVertebralLight::RenderSceneCB()
{


	//第三个参数是 GL_TRUE 是因为我们以行优先的方式提供矩阵的。我们也可以将第三个参数为 GL_FALSE，但是这样的话我们需要转置矩阵的值，因为 C/C++ 中内存的排列仍然是行优先，但是 OpenGL 将认为我们提供的前四个值实际上是一个矩阵的列，并做相应处理
	glUniformMatrix4fv(m_tech->getUniformLocation("gWorld"), 1, GL_TRUE, (const float*)m_worldMt4.m);

	glEnableVertexAttribArray(m_tech->positionLoc);
	glEnableVertexAttribArray(m_tech->texCoordLoc);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (GLvoid*)12);

	//bind texture
	glActiveTexture(GL_TEXTURE0);
	TextureMgr->bindTexture(m_exturesID);
	glEnable(GL_TEXTURE_2D);
	glUniform1i(m_tech->getUniformLocation("s_texture"), 0);// Set the sampler texture unit to 0

	updateLightUniform();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(m_tech->positionLoc);
	glDisableVertexAttribArray(m_tech->texCoordLoc);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glutSwapBuffers();
}

void triangleVertebralLight::draw() {

	m_tech->enable();

	RenderSceneCB();
	return;
	/*
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(m_tech->positionLoc);

	glVertexAttribPointer(m_tech->positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glUniformMatrix4fv(m_tech->getUniformLocation("gWorld"), 1, GL_FALSE, (const GLfloat*)&m_worldMt4);


	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices),
	Indices, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);



	glDisableVertexAttribArray(m_tech->positionLoc);
	*/
}