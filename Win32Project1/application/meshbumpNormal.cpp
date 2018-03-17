

#include "meshbumpNormal.h"
#include "anim.h"
#include "Importer.hpp"
#include "postprocess.h"
#include "TextureManager.h"
#include "OGLGE.h"


meshbumpNormal::meshbumpNormal()
{
	init();
}


meshbumpNormal::~meshbumpNormal()
{

}

void meshbumpNormal::init()
{
	switchNormal = 0.0f;

	//init shader tech
	m_tech = new bumpNormalLightTechnique();
	m_tech->init();
	m_tech->enable();

	m_pipe.setCamera(DefaultCamera);

	//init light config
	m_ambientLightColor.set(1.0f, 1.0f, 1.0f);
	m_ambientLightIntensity = 0.5f;
	m_diffuseIntensity = 0.5f;

	m_lightPosition.set(-1.0f, 1.5f, -0.5f);
	m_weakContant = 1.0f;
	m_weakLine = 0.0f;	

	
	//init mesh
	m_mesh = new bumpNormalMeshNode();
	m_mesh->setAttriPositionLoc(m_tech->positionLoc);
	m_mesh->setAttriTexCoordLoc(m_tech->texCoordLoc);
	m_mesh->setAttriNormalLoc(m_tech->normalLoc);
	m_mesh->setAttriTangentLoc(m_tech->tangentLoc);
	m_mesh->setUniformTextureLoc(m_tech->getUniformLocation("s_texture"));
	m_mesh->setUniformNormalTextureLoc(m_tech->getUniformLocation("s_normalTexture"));
	

	//init vbo
	Vector3 Vertices[12];
	Vertices[0] = Vector3(-1.0f, 1.0f, 0.0f);
	Vertices[1] = Vector3(0.0f, 1.0f, 0.0f);
	Vertices[2] = Vector3(0.0f, 0.0f, 0.0f);

	Vertices[3] = Vector3(1.0f, 1.0f, 0.0f);
	Vertices[4] = Vector3(1.0f, 1.0f, 0.0f);
	Vertices[5] = Vector3(0.0f, 0.0f, 0.0f);

	Vertices[6] = Vector3(1.0f, -1.0f, 0.0f);
	Vertices[7] = Vector3(1.0f, 0.0f, 0.0f);
	Vertices[8] = Vector3(0.0f, 0.0f, 0.0f);

	Vertices[9] = Vector3(-1.0f, -1.0f, 0.0f);
	Vertices[10] = Vector3(0.0f, 0.0f, 0.0f);
	Vertices[11] = Vector3(0.0f, 0.0f, 0.0f);


	unsigned int Indices[] = {
		0, 2, 1,
		3, 2, 0,
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);


	//init ibo	
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	m_tex = TextureMgr->loadTextureJpeg("content/bricks.jpg", GL_RGBA, GL_RGBA, 0, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_normalTex = TextureMgr->loadTextureJpeg("content/normal_map.jpg", GL_RGBA, GL_RGBA, 0, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

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
		normal.z = -normal.z;

		Vertices[i0 + 2] += normal;
		Vertices[i1 + 2] += normal;
		Vertices[i2 + 2] += normal;
	}

	for (int n = 0; n < 12; n += 3)
	{
		Vertices[n + 2].normalize();
	}
}

void meshbumpNormal::draw()
{
	m_tech->enable();

	glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_pipe.GetTrans()->m);
	glUniformMatrix4fv(m_tech->getUniformLocation("WorldMatrix"), 1, GL_TRUE, (const float*)m_pipe.GetWorldTrans()->m);
	glUniform3f(m_tech->getUniformLocation("gDiffuseLight.color"), m_ambientLightColor.x, m_ambientLightColor.y, m_ambientLightColor.z);
	glUniform1f(m_tech->getUniformLocation("gDiffuseLight.ambientIntensity"), m_ambientLightIntensity);
	glUniform1f(m_tech->getUniformLocation("gDiffuseLight.diffuseIntensity"), m_diffuseIntensity);
	glUniform3f(m_tech->getUniformLocation("gDiffuseLight.lightPos"), m_lightPosition.x, m_lightPosition.y, m_lightPosition.z);
	glUniform1f(m_tech->getUniformLocation("gDiffuseLight.weakContant"), m_weakContant);
	glUniform1f(m_tech->getUniformLocation("gDiffuseLight.weakLine"), m_weakLine);
	glUniform1f(m_tech->getUniformLocation("switchNormal"), switchNormal);
	
	m_mesh->draw();
	return;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glEnableVertexAttribArray(m_tech->positionLoc);
	glEnableVertexAttribArray(m_tech->texCoordLoc);

	glVertexAttribPointer(m_tech->positionLoc, 3, GL_FLOAT, GL_FALSE, 36, 0);
	glVertexAttribPointer(m_tech->texCoordLoc, 3, GL_FLOAT, GL_FALSE, 36, (GLvoid*)12);
	glVertexAttribPointer(m_tech->normalLoc, 3, GL_FLOAT, GL_FALSE, 36, (GLvoid*)24);

	glActiveTexture(GL_TEXTURE0);
	TextureMgr->bindTexture(m_tex);
	glUniform1i(m_tech->getUniformLocation("s_texture"), 0);

	glActiveTexture(GL_TEXTURE1);
	TextureMgr->bindTexture(m_normalTex);
	glUniform1i(m_tech->getUniformLocation("s_normalTexture"), 1);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(m_tech->positionLoc);
	glDisableVertexAttribArray(m_tech->texCoordLoc);
	
}

void meshbumpNormal::update(float ft)
{
	static float rty = 0.0;
	m_pipe.Rotate(0, rty, 0);
	rty += 0.09f;
	
}

void meshbumpNormal::setPosition(float x, float y, float z)
{
	m_pipe.WorldPos(x, y, z);
}

void meshbumpNormal::setScale(float s)
{
	m_pipe.Scale(s, s, s);
}

bool meshbumpNormal::loadMesh(const char* filename, const char* normalMapFile)
{
	return m_mesh->loadMesh(filename, normalMapFile);

	//return true;
}


void meshbumpNormal::keyInput(unsigned char param, int x, int y)
{
	if (param == '1')
	{
		switchNormal = 0.0f;
	}
	if (param == '2')
	{
		switchNormal = 1.0f;
	}
}

