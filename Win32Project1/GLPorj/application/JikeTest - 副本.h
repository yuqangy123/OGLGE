#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Mat4.h"
#include "Quaternion.h"
#include "esUtil.h"
#include "shaders.h"
#include <string>
#include <map>
#include "Node.h"
#include "Texture2DRender.h"
#include "Pipeline.h"


class JikeTest :public Node
{
public:
	JikeTest();
	~JikeTest();
	virtual void update(float ft) ;
	virtual void draw() ;
	virtual void keyInput(unsigned char param, int x, int y);

protected:
	void init();

protected:
	GLuint m_programObj = 0;
	GLuint m_vshShader = 0;
	GLuint m_fshShader = 0;
	GLuint m_vbo[2];
	std::map<std::string, uniformUnit> m_userUniforms;
	GLuint m_lastTexturesID;
	int positionLoc = 0;
	int texCoordLoc = 0;

	Matrix4f m_worldMt4;
	Pipeline m_pipe;
};

