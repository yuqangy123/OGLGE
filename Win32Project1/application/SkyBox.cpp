

#include "SkyBox.h"
#include "anim.h"
#include "Importer.hpp"
#include "postprocess.h"
#include "TextureManager.h"
#include "OGLGE.h"

SkyBox::SkyBox()
{
	init();
}


SkyBox::~SkyBox()
{

}

void SkyBox::init()
{
	
	m_tech = new skyBoxTechnique();
	m_tech->init();
	
	m_mesh = new ModelMesh();
	m_mesh->loadMesh("sphere.obj", "content");
	m_mesh->setAttriPositionLoc(m_tech->positionLoc);

	m_pipe.setCamera(DefaultCamera);

	//setScale(40);
	
}

void SkyBox::update(float ft)
{

}

void SkyBox::draw()
{
	m_tech->enable();

	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	glCullFace(GL_FRONT);
	//glDepthFunc(GL_LEQUAL);

	
	auto eyePos = DefaultCamera->getEyePosition();
	setPosition(eyePos.x, eyePos.y, eyePos.z);

	glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_pipe.GetTrans()->m);
	
	glActiveTexture(GL_TEXTURE0);
	TextureMgr->bindTexture(m_texture, GL_TEXTURE_CUBE_MAP);
	glUniform1i(m_tech->getUniformLocation("s_texture"), 0);

	m_mesh->draw();

	glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);
}


void SkyBox::setPosition(float x, float y, float z)
{
	m_pipe.WorldPos(x, y, z);
}

void SkyBox::setScale(float s)
{
	m_pipe.Scale(s, s, s);
}

bool SkyBox::loadMesh(std::vector<std::string>& imgs)
{
	m_texture = TextureMgr->loadTextureCube(imgs, GL_RGBA, GL_RGBA, 0, 0);
	if (0 == m_texture)
	{
		printf("skybox: load texture failed\r\n");
		return false;
	}
	return true;
}

