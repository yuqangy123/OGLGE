#include "stdafx.h"
#include "GBuffer.h"
#include "OGLGE.h"

GBuffer::GBuffer()
{
	init();
}


GBuffer::~GBuffer()
{
	delete m_mesh;
}

void GBuffer::init()
{
	m_winRt = OGLGE::InstanceEx().getWindowsRect();

	m_mesh = new ModelMesh();
	m_mesh->loadMesh("box.obj", "content"); 
	m_mesh->position = Vector3f(0, 0, -10);
	m_mesh->scale = Vector3f(1.5, 1.5, 1.5);


	m_geometryTech.init();
	m_mesh->setAttriPositionLoc( m_geometryTech.positionLoc);
	m_mesh->setAttriTexCoordLoc( m_geometryTech.texCoordLoc);
	m_mesh->setAttriNormalLoc( m_geometryTech.normalLoc);

	m_genmetryPipe.WorldPos(m_mesh->position);
	m_genmetryPipe.Scale(m_mesh->scale);
	m_genmetryPipe.setCamera(DefaultCamera);



	m_fbo = 0;
	for (int n = 0; n < G_TEXTURE_TYPE_NUM; ++n)
		m_gbufferTex[n] = 0;
	m_depthTex = 0;

	DefaultCamera->setFreeCamera(true);
	DefaultCamera->setMouseCenterAlways(true);

	m_diffuselightTech.init();
	/////////////////////////////////////////////////////////////////

	initFBO();
	initDiffuseLightInfo();
}

void GBuffer::initFBO()
{
	glGenFramebuffers(1, &m_fbo);
	glGenTextures(G_TEXTURE_TYPE_NUM, m_gbufferTex);
	glGenTextures(1, &m_depthTex);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	auto winRt = OGLGE::Instance()->getWindowsRect();
	for (int n = 0; n < G_TEXTURE_TYPE_NUM; ++n)
	{
		glBindTexture(GL_TEXTURE_2D, m_gbufferTex[n]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, winRt.width, winRt.height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + n, GL_TEXTURE_2D, m_gbufferTex[n], 0);
	}

	glBindTexture(GL_TEXTURE_2D, m_depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, winRt.width, winRt.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTex, 0);

	GLenum drawBuff[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(G_TEXTURE_TYPE_NUM, drawBuff);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != status)
	{
		printf("init framebuffer error:0x%x\r\n", status);
		assert(0);
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GBuffer::initDiffuseLightInfo()
{
	{
		m_deffuseLightInfo.ambientColor.set(0.0f, 1.0f, .0f);
		m_deffuseLightInfo.ambientIntensity = 0.5f;
		m_deffuseLightInfo.diffuseIntensity = 0.5f;
		m_deffuseLightInfo.direction.set(1.0f, 0.f, 0.f);
		m_deffuseLightInfo.direction.normalize();



		glGenBuffers(1, &m_deffuseLightInfo.VB);
		glBindBuffer(GL_ARRAY_BUFFER, m_deffuseLightInfo.VB);
		Vector3f Vertices[] = {
			Vector3f(-1, -1, 0),
			Vector3f(1, -1, 0),
			Vector3f(1, 1, 0),
			Vector3f(-1, 1, 0)
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		unsigned int Indices[] = { 0, 1, 3, 1, 2, 3 };
		glGenBuffers(1, &m_deffuseLightInfo.IB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_deffuseLightInfo.IB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), &Indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_deffuseLightInfo.numIndices = sizeof(Indices) / sizeof(Indices[0]);
	}
	
	{
		m_deffuseLightInfo2.ambientColor.set(.0f, .0f, 1.0f);
		m_deffuseLightInfo2.ambientIntensity = 0.5f;
		m_deffuseLightInfo2.diffuseIntensity = 0.5f;
		m_deffuseLightInfo2.direction.set(-1.0f, 0.f, 0.f);
		m_deffuseLightInfo2.direction.normalize();



		glGenBuffers(1, &m_deffuseLightInfo2.VB);
		glBindBuffer(GL_ARRAY_BUFFER, m_deffuseLightInfo2.VB);
		Vector3f Vertices[] = {
			Vector3f(-1, -1, 0),
			Vector3f(1, -1, 0),
			Vector3f(1, 1, 0),
			Vector3f(-1, 1, 0)
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		unsigned int Indices[] = { 0, 1, 3, 1, 2, 3 };
		glGenBuffers(1, &m_deffuseLightInfo2.IB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_deffuseLightInfo2.IB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), &Indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_deffuseLightInfo2.numIndices = sizeof(Indices) / sizeof(Indices[0]);
	}
}

void GBuffer::geometryPass()
{
	m_geometryTech.enable();

	bool depthTest = glIsEnabled(GL_DEPTH_TEST);
	bool blend = glIsEnabled(GL_BLEND);

	glDepthMask(GL_TRUE);
	
	glUniformMatrix4fv(m_geometryTech.getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_genmetryPipe.GetTrans()->m);
	glUniformMatrix4fv(m_geometryTech.getUniformLocation("WorldMatrix"), 1, GL_TRUE, (const float*)m_genmetryPipe.GetWorldTrans()->m);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	m_mesh->draw();

	glDepthMask(GL_FALSE);
}

void GBuffer::drawGBufferTex()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, m_winRt.width, m_winRt.height,
		0, 0, m_winRt.width / 2.0, m_winRt.height / 2.0, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(0, 0, m_winRt.width, m_winRt.height,
		m_winRt.width / 2.0, 0, m_winRt.width, m_winRt.height / 2.0, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glReadBuffer(GL_COLOR_ATTACHMENT2);
	glBlitFramebuffer(0, 0, m_winRt.width, m_winRt.height,
		0, m_winRt.height / 2.0, m_winRt.width / 2.0, m_winRt.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glReadBuffer(GL_COLOR_ATTACHMENT3);
	glBlitFramebuffer(0, 0, m_winRt.width, m_winRt.height,
		m_winRt.width / 2.0, m_winRt.height / 2.0, m_winRt.width, m_winRt.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);


	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}
void GBuffer::drawNormalPointLights()
{
	glDepthMask(GL_FALSE);

	glEnable(GL_BLEND);	
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	//glDepthMask(GL_TRUE);

	m_diffuselightTech.enable();

	m_mesh->setAttriPositionLoc(m_diffuselightTech.positionLoc);
	m_mesh->setAttriTexCoordLoc(m_diffuselightTech.texCoordLoc);
	m_mesh->setAttriNormalLoc(m_diffuselightTech.normalLoc);

	glUniformMatrix4fv(m_diffuselightTech.getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_genmetryPipe.GetTrans()->m);
	glUniformMatrix4fv(m_diffuselightTech.getUniformLocation("WorldMatrix"), 1, GL_TRUE, (const float*)m_genmetryPipe.GetWorldTrans()->m);

	glUniform3f(m_diffuselightTech.getUniformLocation("gDiffuseLight.color"), m_deffuseLightInfo.ambientColor.x, m_deffuseLightInfo.ambientColor.y, m_deffuseLightInfo.ambientColor.z);
	glUniform1f(m_diffuselightTech.getUniformLocation("gDiffuseLight.ambientIntensity"), m_deffuseLightInfo.ambientIntensity);
	glUniform1f(m_diffuselightTech.getUniformLocation("gDiffuseLight.diffuseIntensity"), m_deffuseLightInfo.diffuseIntensity);
	glUniform3f(m_diffuselightTech.getUniformLocation("gDiffuseLight.direction"), m_deffuseLightInfo.direction.x, m_deffuseLightInfo.direction.y, m_deffuseLightInfo.direction.z);

	m_mesh->draw();

	//draw elements again
	glUniform3f(m_diffuselightTech.getUniformLocation("gDiffuseLight.color"), m_deffuseLightInfo2.ambientColor.x, m_deffuseLightInfo2.ambientColor.y, m_deffuseLightInfo2.ambientColor.z);
	glUniform1f(m_diffuselightTech.getUniformLocation("gDiffuseLight.ambientIntensity"), m_deffuseLightInfo2.ambientIntensity);
	glUniform1f(m_diffuselightTech.getUniformLocation("gDiffuseLight.diffuseIntensity"), m_deffuseLightInfo2.diffuseIntensity);
	glUniform3f(m_diffuselightTech.getUniformLocation("gDiffuseLight.direction"), m_deffuseLightInfo2.direction.x, m_deffuseLightInfo2.direction.y, m_deffuseLightInfo2.direction.z);

	m_mesh->draw();

	glDisable(GL_BLEND);

	
}
void GBuffer::drawGBufferPointLights()
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	m_diffuselightTech.enable();

	glEnableVertexAttribArray(m_diffuselightTech.positionLoc);
	Matrix4f mvp;
	mvp.identity();
	glUniformMatrix4fv(m_diffuselightTech.getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)mvp.m);
	glUniform3f(m_diffuselightTech.getUniformLocation("gDiffuseLight.color"), m_deffuseLightInfo.ambientColor.x, m_deffuseLightInfo.ambientColor.y, m_deffuseLightInfo.ambientColor.z);
	glUniform1f(m_diffuselightTech.getUniformLocation("gDiffuseLight.ambientIntensity"), m_deffuseLightInfo.ambientIntensity);
	glUniform1f(m_diffuselightTech.getUniformLocation("gDiffuseLight.diffuseIntensity"), m_deffuseLightInfo.diffuseIntensity);
	glUniform3f(m_diffuselightTech.getUniformLocation("gDiffuseLight.direction"), m_deffuseLightInfo.direction.x, m_deffuseLightInfo.direction.y, m_deffuseLightInfo.direction.z);
	

	//glReadBuffer(GL_COLOR_ATTACHMENT0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gbufferTex[G_TEXTURE_TYPE_POSITION]);
	glUniform1i(m_diffuselightTech.getUniformLocation("s_positionMap"), 0);

	//glReadBuffer(GL_COLOR_ATTACHMENT1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gbufferTex[G_TEXTURE_TYPE_DIFFUSE]);
	glUniform1i(m_diffuselightTech.getUniformLocation("s_diffuseMap"), 1);

	//glReadBuffer(GL_COLOR_ATTACHMENT2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gbufferTex[G_TEXTURE_TYPE_NORMAL]);
	glUniform1i(m_diffuselightTech.getUniformLocation("s_normalMap"), 2);

	//glReadBuffer(GL_COLOR_ATTACHMENT3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_gbufferTex[G_TEXTURE_TYPE_TEXCOORD]);
	glUniform1i(m_diffuselightTech.getUniformLocation("s_texcoordMap"), 3);
	glUniform2f(m_diffuselightTech.getUniformLocation("screenSize"), m_winRt.width, m_winRt.height);

	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, m_deffuseLightInfo.VB);
	glVertexAttribPointer(m_diffuselightTech.positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (GLvoid*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_deffuseLightInfo.IB);
	glDrawElements(GL_TRIANGLES, m_deffuseLightInfo.numIndices, GL_UNSIGNED_INT, 0);

	
	//draw elements again
	glUniform3f(m_diffuselightTech.getUniformLocation("gDiffuseLight.color"), m_deffuseLightInfo2.ambientColor.x, m_deffuseLightInfo2.ambientColor.y, m_deffuseLightInfo2.ambientColor.z);
	glUniform1f(m_diffuselightTech.getUniformLocation("gDiffuseLight.ambientIntensity"), m_deffuseLightInfo2.ambientIntensity);
	glUniform1f(m_diffuselightTech.getUniformLocation("gDiffuseLight.diffuseIntensity"), m_deffuseLightInfo2.diffuseIntensity);
	glUniform3f(m_diffuselightTech.getUniformLocation("gDiffuseLight.direction"), m_deffuseLightInfo2.direction.x, m_deffuseLightInfo2.direction.y, m_deffuseLightInfo2.direction.z);

	glBindBuffer(GL_ARRAY_BUFFER, m_deffuseLightInfo2.VB);
	glVertexAttribPointer(m_diffuselightTech.positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (GLvoid*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_deffuseLightInfo2.IB);
	glDrawElements(GL_TRIANGLES, m_deffuseLightInfo2.numIndices, GL_UNSIGNED_INT, 0);
	

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisable(GL_BLEND);
}

void GBuffer::draw()
{
	geometryPass();

	drawGBufferTex();

	drawGBufferPointLights();
	
	//drawNormalPointLights();
}


void GBuffer::keyInput(unsigned char param, int x, int y)
{
	float factor = 0.5f;
	if (param == 'p')
	{
		//auto epos = m_tech->getLightPosition();
		//m_tech->setLightPosition(epos.x, epos.y, epos.z - 5);
	}
	if (param == 'o')
	{
		//auto epos = m_tech->getLightPosition();
		//m_tech->setLightPosition(epos.x, epos.y, epos.z + 5);
	}
}