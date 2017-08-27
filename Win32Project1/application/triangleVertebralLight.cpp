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


void triangleVertebralLight::createVBOIBO_dotDiffuseLight()
{
	m_ambientLightColor.set(1.0f, 1.0f, 1.0f);
	m_ambientLightIntensity = 1.6f;

	m_diffuseIntensity = 6.0f;

	m_dotDiffuseLightPos.set(0.0f, 0.0f, -3.0f);
	m_weakContant = 1.0f;
	m_weakLine = 1.0f;

	//init vbo
	Vector3 Vertices[12];
	Vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
	Vertices[1] = Vector3(0.0f, 0.0f, 0.0f);
	Vertices[2] = Vector3(0.0f, 0.0f, 0.0f);

	Vertices[3] = Vector3(0.0f, -1.0f, modelZ);
	Vertices[4] = Vector3(0.5f, 0.0f, 0.0f);
	Vertices[5] = Vector3(0.0f, 0.0f, 0.0f);

	Vertices[6] = Vector3(1.0f, -1.0f, 0.0f);
	Vertices[7] = Vector3(1.0f, 0.0f, 1.0f);
	Vertices[8] = Vector3(0.0f, 0.0f, 0.0f);

	Vertices[9] = Vector3(0.0f, 1.0f, 0.0f);
	Vertices[10] = Vector3(0.5f, 1.0f, 1.0f);
	Vertices[11] = Vector3(0.0f, 0.0f, 0.0f);

	unsigned int Indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	//create normal
	for (int n = 0; n < sizeof(Indices) / sizeof(Indices[0]); n += 3)
	{
		unsigned int i0 = Indices[n + 0] * 3;
		unsigned int i1 = Indices[n + 1] * 3;
		unsigned int i2 = Indices[n + 2] * 3;
		Vector3& v0 = Vertices[i0];
		Vector3& v1 = Vertices[i1];
		Vector3& v2 = Vertices[i2];

		Vector3 a1 = v1 - v0;
		Vector3 a2 = v2 - v0;

		Vector3 normal;
		Vector3::cross(a2, a1, normal);
		normal.normalize();

		Vertices[i0 + 2] += normal;
		Vertices[i1 + 2] += normal;
		Vertices[i2 + 2] += normal;
	}

	for (int n = 0; n < 12; n += 3)
	{
		Vertices[n + 2].normalize();
	}


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);


	//init ibo	
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

void triangleVertebralLight::createVBOIBO_specularLight()
{
	m_ambientLightColor.set(0.5f, 0.5f, 0.5f);
	m_ambientLightIntensity = 0.5f;
	m_diffuseDirection.set(0.0f, 0.0f, -1.0f);
	m_diffuseDirection.normalize();
	m_diffuseIntensity = 1.0f;
	m_specularEye = DefaultCamera->getPosition();
	
	m_specularPower = 64.f;
	m_specularIntensity = 4.0f;

	//init vbo
	Vector3 Vertices[12];
	Vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
	Vertices[1] = Vector3(0.0f, 0.0f, 0.0f);
	Vertices[2] = Vector3(0.0f, 0.0f, 0.0f);

	Vertices[3] = Vector3(0.0f, -1.0f, modelZ);
	Vertices[4] = Vector3(0.5f, 0.0f, 0.0f);
	Vertices[5] = Vector3(0.0f, 0.0f, 0.0f);

	Vertices[6] = Vector3(1.0f, -1.0f, 0.0f);
	Vertices[7] = Vector3(1.0f, 0.0f, 1.0f);
	Vertices[8] = Vector3(0.0f, 0.0f, 0.0f);

	Vertices[9] = Vector3(0.0f, 1.0f, 0.0f);
	Vertices[10] = Vector3(0.5f, 1.0f, 1.0f);
	Vertices[11] = Vector3(0.0f, 0.0f, 0.0f);

	unsigned int Indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	//create normal
	for (int n = 0; n < sizeof(Indices) / sizeof(Indices[0]); n += 3)
	{
		unsigned int i0 = Indices[n + 0] * 3;
		unsigned int i1 = Indices[n + 1] * 3;
		unsigned int i2 = Indices[n + 2] * 3;
		Vector3& v0 = Vertices[i0];
		Vector3& v1 = Vertices[i1];
		Vector3& v2 = Vertices[i2];

		Vector3 a1 = v1 - v0;
		Vector3 a2 = v2 - v0;

		Vector3 normal;
		Vector3::cross(a1, a2, normal);
		normal.normalize();

		Vertices[i0 + 2] += normal;
		Vertices[i1 + 2] += normal;
		Vertices[i2 + 2] += normal;
	}

	for (int n = 0; n < 12; n += 3)
	{
		Vertices[n + 2].normalize();
	}


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);


	//init ibo	
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

void triangleVertebralLight::createVBOIBO_diffuseLight()
{
	m_ambientLightColor.set(0.5f, 0.5f, 0.5f);
	m_ambientLightIntensity = 0.5f;
	m_diffuseDirection.set(1.0f, 0.0f, 0.0f);
	m_diffuseDirection.normalize();
	m_diffuseIntensity = 3.0f;
	


	//init vbo
	Vector3 Vertices[12];	
	Vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
	Vertices[1] = Vector3(0.0f, 0.0f, 0.0f);
	Vertices[2] = Vector3(0.0f, 0.0f, 0.0f);

	Vertices[3] = Vector3(0.0f, -1.0f, modelZ);
	Vertices[4] = Vector3(0.5f, 0.0f, 0.0f);
	Vertices[5] = Vector3(0.0f, 0.0f, 0.0f);

	Vertices[6] = Vector3(1.0f, -1.0f, 0.0f);
	Vertices[7] = Vector3(1.0f, 0.0f, 1.0f);
	Vertices[8] = Vector3(0.0f, 0.0f, 0.0f);

	Vertices[9] = Vector3(0.0f, 1.0f, 0.0f);
	Vertices[10] = Vector3(0.5f, 1.0f, 1.0f);
	Vertices[11] = Vector3(0.0f, 0.0f, 0.0f);

	unsigned int Indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	//create normal
	for (int n = 0; n < sizeof(Indices) / sizeof(Indices[0]); n += 3)
	{
		unsigned int i0 = Indices[n + 0]*3;
		unsigned int i1 = Indices[n + 1]*3;
		unsigned int i2 = Indices[n + 2]*3;
		Vector3& v0 = Vertices[i0];
		Vector3& v1 = Vertices[i1];
		Vector3& v2 = Vertices[i2];

		Vector3 a1 = v1 - v0;
		Vector3 a2 = v2 - v0;

		Vector3 normal;
		Vector3::cross(a2, a1, normal);
		normal.normalize();

		Vertices[i0 + 2] += normal;
		Vertices[i1 + 2] += normal;
		Vertices[i2 + 2] += normal;
	}

	for (int n = 0; n < 12; n += 3)
	{
		Vertices[n + 2].normalize();
	}


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);


	//init ibo	
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

void triangleVertebralLight::createVBOIBO_ambientLight()
{
	m_ambientLightColor.set(0.5f, 0.5f, 0.5f);
	m_ambientLightIntensity = 1.0f;

	//init vbo
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


	//init ibo
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
	m_lightType = lightType::dotDiffuseLight;
	m_tech = new lightTechnique();
	m_tech->init(m_lightType);

	auto winsz = OGLGE::Instance()->getWindowsRect();
	m_lastMouseX = winsz.width / 2.0;
	m_lastMouseY = winsz.height / 2.0;
	glutWarpPointer(m_lastMouseX, m_lastMouseY);



	m_MVPMt4.identity();

	m_pipe.setCamera(DefaultCamera);

	//m_pipe.Scale(0.25, 0.25, 0.25);
	m_pipe.WorldPos(0, 0, -5.0);

	m_exturesID = TextureMgr->loadTextureJpeg("res/gezi.jpg", GL_RGBA, GL_RGBA, 0, 0);

	// Set the filtering mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
	switch (m_lightType)
	{
		case lightType::ambientLight:
		{
			createVBOIBO_ambientLight();
		}break;

		case lightType::diffuseLight:
		{
			createVBOIBO_diffuseLight();
		}break;

		case lightType::specularLight:
		{
			createVBOIBO_specularLight();
		}break;

		case lightType::dotDiffuseLight:
		{
			createVBOIBO_dotDiffuseLight();
		}break;
	}
	
}

void triangleVertebralLight::update(float ft) {

	m_MVPMt4 = *m_pipe.GetTrans();
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
		Vector3 pos = m_pipe.getWorldPos();
		pos.z += 0.1f;
		m_pipe.WorldPos(pos.x, pos.y, pos.z);
		printf("%f,%f,%f\r\n", pos.x, pos.y, pos.z);
	}
	else if (param == 'q')
	{
		Vector3 pos = m_pipe.getWorldPos();
		pos.z -= 0.1f;
		m_pipe.WorldPos(pos.x, pos.y, pos.z);
		printf("%f,%f,%f\r\n", pos.x, pos.y, pos.z);
	}
	else if (param == 'z')
	{
		modelZ -= 0.1f;
		//CreateVertexBuffer();
		printf("modelZ=%f\r\n", modelZ);
	}
	else if (param == 'x')
	{
		modelZ += 0.1f;
		//CreateVertexBuffer();
		printf("modelZ=%f\r\n", modelZ);
	}
	else if (param == 'r')
	{
		//DefaultCamera->slide(Vector3(0.0, 0.0, 0.1));
		Vector3 pos = m_pipe.getWorldPos();
		pos.y += 0.1f;
		m_pipe.WorldPos(pos.x, pos.y, pos.z);
		printf("%f,%f,%f\r\n", pos.x, pos.y, pos.z);
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
		Vector3 pos = m_pipe.getWorldPos();
		pos.y -= 0.1f;
		m_pipe.WorldPos(pos.x, pos.y, pos.z);
		printf("%f,%f,%f\r\n", pos.x, pos.y, pos.z);
		//DefaultCamera->slide(Vector3(0.0, -0.1, 0.0));
	}
	else if (param == 't')
	{
		Vector3 pos = m_pipe.getWorldPos();
		pos.x -= 0.1f;
		m_pipe.WorldPos(pos.x, pos.y, pos.z);
		printf("%f,%f,%f\r\n", pos.x, pos.y, pos.z);
		//DefaultCamera->slide(Vector3(0.0, 0.1, 0.0));
	}
	else if (param == 'y')
	{
		Vector3 pos = m_pipe.getWorldPos();
		pos.x += 0.1f;
		m_pipe.WorldPos(pos.x, pos.y, pos.z);
		printf("%f,%f,%f\r\n", pos.x, pos.y, pos.z);
		//DefaultCamera->slide(Vector3(0.0, 0.1, 0.0));
	}

	else if (param == 'i')
	{
		m_diffuseDirection.x += 0.5;
		m_diffuseDirection.normalize();
		printf("%f,%f,%f\r\n", m_diffuseDirection.x, m_diffuseDirection.y, m_diffuseDirection.z);
	}
	else if (param == 'o')
	{
		m_diffuseDirection.y += 0.5;
		m_diffuseDirection.normalize();
		printf("%f,%f,%f\r\n", m_diffuseDirection.x, m_diffuseDirection.y, m_diffuseDirection.z);
	}
	else if (param == 'p')
	{
		m_diffuseDirection.z += 0.5;
		m_diffuseDirection.normalize();
		printf("%f,%f,%f\r\n", m_diffuseDirection.x, m_diffuseDirection.y, m_diffuseDirection.z);
	}
	else if (param == 'j')
	{
		m_diffuseDirection.x -= 0.5;
		m_diffuseDirection.normalize();
		printf("%f,%f,%f\r\n", m_diffuseDirection.x, m_diffuseDirection.y, m_diffuseDirection.z);
	}
	else if (param == 'k')
	{
		m_specularEye.z += 0.05f;
		m_specularEye.normalize();
		//m_diffuseDirection.y -= 0.5;
		//m_diffuseDirection.normalize();
		//printf("%f,%f,%f\r\n", m_diffuseDirection.x, m_diffuseDirection.y, m_diffuseDirection.z);
	}
	else if (param == 'l')
	{
		
		m_specularEye.z -= 0.05f;
		m_specularEye.normalize();
		//m_diffuseDirection.z -= 0.5;
		//m_diffuseDirection.normalize();
		//printf("%f,%f,%f\r\n", m_diffuseDirection.x, m_diffuseDirection.y, m_diffuseDirection.z);
	}


}

void triangleVertebralLight::mouseInput(int button, int state, int x, int y)
{
	float rx = 0.0, ry = 0.0;

	float sp = 20.0;
	if (abs(y - m_lastMouseY) > 2)
	{
		glutWarpPointer(m_lastMouseX, m_lastMouseY);
		//m_specularEye.y += (m_lastMouseY - y) / sp;
		//m_specularEye.normalize();
		ry = (m_lastMouseY - y) / sp;
		//printf("y:%d,%d\r\n", y, m_lastMouseY);
		//DefaultCamera->yaw((m_lastMouseY - y)/10.0);
	}
	if (abs(x - m_lastMouseX) > 2)
	{
		glutWarpPointer(m_lastMouseX, m_lastMouseY);
		//m_specularEye.x += (m_lastMouseX - x) / sp;
		//m_specularEye.normalize();
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
			glEnableVertexAttribArray(m_tech->positionLoc);
			glEnableVertexAttribArray(m_tech->texCoordLoc);

			glVertexAttribPointer(m_tech->positionLoc, 3, GL_FLOAT, GL_FALSE, 24, 0);
			glVertexAttribPointer(m_tech->texCoordLoc, 3, GL_FLOAT, GL_FALSE, 24, (GLvoid*)12);
			
			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

			glDisableVertexAttribArray(m_tech->positionLoc);
			glDisableVertexAttribArray(m_tech->texCoordLoc);

			glUniform3f(m_tech->getUniformLocation("gDirectionLight.color"), m_ambientLightColor.x, m_ambientLightColor.y, m_ambientLightColor.z);
			glUniform1f(m_tech->getUniformLocation("gDirectionLight.ambientIntensity"), m_ambientLightIntensity);

		}break;

		case lightType::diffuseLight:
		{
			glEnableVertexAttribArray(m_tech->positionLoc);
			glEnableVertexAttribArray(m_tech->texCoordLoc);
			glEnableVertexAttribArray(m_tech->normalLoc);

			glVertexAttribPointer(m_tech->positionLoc, 3, GL_FLOAT, GL_FALSE, 12 * 3, (GLvoid*)(12 * 0));
			glVertexAttribPointer(m_tech->texCoordLoc, 3, GL_FLOAT, GL_FALSE, 12 * 3, (GLvoid*)(12 * 1));
			glVertexAttribPointer(m_tech->normalLoc, 3, GL_FLOAT, GL_FALSE, 12 * 3, (GLvoid*)(12 * 2));

			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

			glDisableVertexAttribArray(m_tech->positionLoc);
			glDisableVertexAttribArray(m_tech->texCoordLoc);
			glDisableVertexAttribArray(m_tech->normalLoc);

			const Matrix4f* worldMt4 = m_pipe.GetWorldTrans();
			glUniformMatrix4fv(m_tech->getUniformLocation("WorldMatrix"), 1, GL_TRUE, (const float*)worldMt4->m);
			glUniform3f(m_tech->getUniformLocation("gDiffuseLight.color"), m_ambientLightColor.x, m_ambientLightColor.y, m_ambientLightColor.z);
			glUniform1f(m_tech->getUniformLocation("gDiffuseLight.ambientIntensity"), m_ambientLightIntensity);
			glUniform3f(m_tech->getUniformLocation("gDiffuseLight.direction"), m_diffuseDirection.x, m_diffuseDirection.y, m_diffuseDirection.z);
			glUniform1f(m_tech->getUniformLocation("gDiffuseLight.diffuseIntensity"), m_diffuseIntensity);

		}break;

		case lightType::specularLight:
		{
			glEnableVertexAttribArray(m_tech->positionLoc);
			glEnableVertexAttribArray(m_tech->texCoordLoc);
			glEnableVertexAttribArray(m_tech->normalLoc);

			glVertexAttribPointer(m_tech->positionLoc, 3, GL_FLOAT, GL_FALSE, 12 * 3, (GLvoid*)(12 * 0));
			glVertexAttribPointer(m_tech->texCoordLoc, 3, GL_FLOAT, GL_FALSE, 12 * 3, (GLvoid*)(12 * 1));
			glVertexAttribPointer(m_tech->normalLoc, 3, GL_FLOAT, GL_FALSE, 12 * 3, (GLvoid*)(12 * 2));

			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

			glDisableVertexAttribArray(m_tech->positionLoc);
			glDisableVertexAttribArray(m_tech->texCoordLoc);
			glDisableVertexAttribArray(m_tech->normalLoc);

			const Matrix4f* worldMt4 = m_pipe.GetWorldTrans();
			glUniformMatrix4fv(m_tech->getUniformLocation("WorldMatrix"), 1, GL_TRUE, (const float*)worldMt4->m);
			glUniform3f(m_tech->getUniformLocation("gDiffuseLight.color"), m_ambientLightColor.x, m_ambientLightColor.y, m_ambientLightColor.z);
			glUniform1f(m_tech->getUniformLocation("gDiffuseLight.ambientIntensity"), m_ambientLightIntensity);
			glUniform3f(m_tech->getUniformLocation("gDiffuseLight.direction"), m_diffuseDirection.x, m_diffuseDirection.y, m_diffuseDirection.z);
			glUniform1f(m_tech->getUniformLocation("gDiffuseLight.diffuseIntensity"), m_diffuseIntensity);
			glUniform3f(m_tech->getUniformLocation("gDiffuseLight.specularEye"), m_specularEye.x, m_specularEye.y, m_specularEye.z);
			glUniform1f(m_tech->getUniformLocation("gDiffuseLight.specularPower"), m_specularPower);
			glUniform1f(m_tech->getUniformLocation("gDiffuseLight.specularIntensity"), m_specularIntensity);

		}break;

		case lightType::dotDiffuseLight:
		{
			glEnableVertexAttribArray(m_tech->positionLoc);
			glEnableVertexAttribArray(m_tech->texCoordLoc);
			glEnableVertexAttribArray(m_tech->normalLoc);

			glVertexAttribPointer(m_tech->positionLoc, 3, GL_FLOAT, GL_FALSE, 12 * 3, (GLvoid*)(12 * 0));
			glVertexAttribPointer(m_tech->texCoordLoc, 3, GL_FLOAT, GL_FALSE, 12 * 3, (GLvoid*)(12 * 1));
			glVertexAttribPointer(m_tech->normalLoc, 3, GL_FLOAT, GL_FALSE, 12 * 3, (GLvoid*)(12 * 2));

			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

			glDisableVertexAttribArray(m_tech->positionLoc);
			glDisableVertexAttribArray(m_tech->texCoordLoc);
			glDisableVertexAttribArray(m_tech->normalLoc);

			const Matrix4f* worldMt4 = m_pipe.GetWorldTrans();
			glUniformMatrix4fv(m_tech->getUniformLocation("WorldMatrix"), 1, GL_TRUE, (const float*)worldMt4->m);
			glUniform3f(m_tech->getUniformLocation("gDotDiffuseLight.color"), m_ambientLightColor.x, m_ambientLightColor.y, m_ambientLightColor.z);
			glUniform1f(m_tech->getUniformLocation("gDotDiffuseLight.ambientIntensity"), m_ambientLightIntensity);
			glUniform1f(m_tech->getUniformLocation("gDotDiffuseLight.diffuseIntensity"), m_diffuseIntensity);
			glUniform3f(m_tech->getUniformLocation("gDotDiffuseLight.lightPos"), m_dotDiffuseLightPos.x, m_dotDiffuseLightPos.y, m_dotDiffuseLightPos.z);
			glUniform1f(m_tech->getUniformLocation("gDotDiffuseLight.weakContant"), m_weakContant);
			glUniform1f(m_tech->getUniformLocation("gDotDiffuseLight.weakLine"), m_weakLine);
		}break;
	}
}

void triangleVertebralLight::RenderSceneCB()
{

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	

	//第三个参数是 GL_TRUE 是因为我们以行优先的方式提供矩阵的。我们也可以将第三个参数为 GL_FALSE，但是这样的话我们需要转置矩阵的值，因为 C/C++ 中内存的排列仍然是行优先，但是 OpenGL 将认为我们提供的前四个值实际上是一个矩阵的列，并做相应处理
	glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_MVPMt4.m);

	
	//bind texture
	glActiveTexture(GL_TEXTURE0);
	TextureMgr->bindTexture(m_exturesID);
	glEnable(GL_TEXTURE_2D);
	glUniform1i(m_tech->getUniformLocation("s_texture"), 0);// Set the sampler texture unit to 0

	updateLightUniform();
	

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


	glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_FALSE, (const GLfloat*)&m_worldMt4);


	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices),
	Indices, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);



	glDisableVertexAttribArray(m_tech->positionLoc);
	*/
}