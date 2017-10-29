#pragma once
#include <vector>
#include "GL\glew.h"
#include "Vector2.h"
#include "Node.h"
#include "ModelMesh.h"
#include "Camera3D.h"
#include "Pipeline.h"
#include "tech/geometryTechnique.h"
#include "tech/GBufferDiffuselightTechnique.h"


class GBuffer :public Node
{
public:
	GBuffer();
	~GBuffer();


	void draw();

	void keyInput(unsigned char param, int x, int y);

protected:
	void init();
	void initFBO();
	void initDiffuseLightInfo();

	void geometryPass();
	void drawGBufferTex();
	void drawNormalPointLights();
	void drawGBufferPointLights();

protected:
	enum G_TEXTURE_TYPE
	{
		G_TEXTURE_TYPE_POSITION=0,
		G_TEXTURE_TYPE_DIFFUSE,
		G_TEXTURE_TYPE_NORMAL,
		G_TEXTURE_TYPE_TEXCOORD,
		G_TEXTURE_TYPE_NUM
	};

	typedef struct {
		GLuint VB;
		GLuint IB;

		Vector3f ambientColor;
		float ambientIntensity;
		float diffuseIntensity;
		Vector3f direction;
		int numIndices;
	}DIFFUSE_LIGHT_DATA;

protected:
	GLuint m_fbo;
	GLuint m_gbufferTex[G_TEXTURE_TYPE_NUM];
	GLuint m_depthTex;

	ModelMesh* m_mesh;
	geometryTechnique m_geometryTech;
	Pipeline m_genmetryPipe;

	Rect m_winRt;

	DIFFUSE_LIGHT_DATA m_deffuseLightInfo;
	DIFFUSE_LIGHT_DATA m_deffuseLightInfo2;
	GBufferDiffuselightTechnique m_diffuselightTech;
};
