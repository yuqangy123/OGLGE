#include "stdafx.h"
#include "assert.h"
#include "JikeTest.h"
#include "FileUnits.h"
#include <stddef.h> //used offsetof
#include <OGLGE.h>
#include "Vector3.h"

Vector3 Vertices[4];
unsigned int Indices[12];
GLuint VBO;
GLuint IBO;

#define ATTRI_A_POSITION_INDEX	0
#define ATTRI_A_POSITION_NAME		"a_position"
#define ATTRI_A_TEXCOORD_INDEX	1
#define ATTRI_A_TEXCOORD_NAME		"a_texCoord"


JikeTest::JikeTest()
{
	init();
}


JikeTest::~JikeTest()
{
}

void JikeTest::init()
{
	if (0 != m_programObj)
		return;

	char dir[512] = { 0 };
	char file[512] = { 0 };
	GetCurrentDirectoryA(1024 * 2, dir);

	const char* vshSrcFile = "res/shaderFile/jikeshader.vsh";
	const char* fshSrcFile = "res/shaderFile/jikeshader.fsh";
	std::string strVsh;
	FileUnits::Instance()->getFileData(vshSrcFile, strVsh);
	m_vshShader = loadShader(GL_VERTEX_SHADER, strVsh.c_str());

	std::string strFsh;
	FileUnits::Instance()->getFileData(fshSrcFile, strFsh);
	m_fshShader = loadShader(GL_FRAGMENT_SHADER, strFsh.c_str());

	m_programObj = glCreateProgram();
	if (0 == m_programObj)
	{
		ERROR("create Texture2D Shader program failed");
		return;
	}
	glAttachShader(m_programObj, m_vshShader);
	glAttachShader(m_programObj, m_fshShader);

	//need linkProgram front init attri and Uniforms
	glLinkProgram(m_programObj);
	GLint status = GL_TRUE;
	glGetProgramiv(m_programObj, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLen = 0;
		glGetProgramiv(m_programObj, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1)
		{
			char* infoLog = new char[sizeof(char) * infoLen];
			glGetProgramInfoLog(m_programObj, infoLen, NULL, infoLog);
			ERROR("cocos2d: ERROR: Failed to link program:\r\n%s", infoLog);
			delete infoLog;
		}
		glDeleteProgram(m_programObj);
		return;
	}
	

	positionLoc = glGetAttribLocation(m_programObj, ATTRI_A_POSITION_NAME);
	texCoordLoc = glGetAttribLocation(m_programObj, ATTRI_A_TEXCOORD_NAME);
	// Get the sampler location

	//glBindAttribLocation(m_programObj, ATTRI_A_POSITION_INDEX, ATTRI_A_POSITION_NAME);
	//glBindAttribLocation(m_programObj, ATTRI_A_TEXCOORD_INDEX, ATTRI_A_TEXCOORD_NAME);


	// Query and store uniforms from the program.
	GLint activeUniforms;
	glGetProgramiv(m_programObj, GL_ACTIVE_UNIFORMS, &activeUniforms);
	if (activeUniforms > 0)
	{
		GLint length;
		glGetProgramiv(m_programObj, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
		if (length > 0)
		{
			char* uniformName = new char[length + 1];

			for (int i = 0; i < activeUniforms; ++i)
			{
				uniformUnit u;
				// Query uniform info.
				glGetActiveUniform(m_programObj, i, length, nullptr, &u.size, &u.type, uniformName);
				uniformName[length] = '\0';
				u.name.assign(uniformName);

				int location = glGetUniformLocation(m_programObj, uniformName);
				u.location = location;
				GLenum __gl_error_code = glGetError();
				if (__gl_error_code != GL_NO_ERROR)
					ERROR("error: 0x%x  uniformName: %s", (int)__gl_error_code, uniformName);

				assert(__gl_error_code == GL_NO_ERROR);

				m_userUniforms[u.name] = u;
			}
			delete uniformName;
		}
	}
	else
	{
		char ErrorLog[1024];
		glGetProgramInfoLog(m_programObj, sizeof(ErrorLog), nullptr, ErrorLog);
		ERROR("Error linking shader program: '%s'\n", ErrorLog);
	}


	Vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
	Vertices[1] = Vector3(0.0f, -1.0f, 1.0f);
	Vertices[2] = Vector3(1.0f, -1.0f, 0.0f);
	Vertices[3] = Vector3(0.0f, 1.0f, 0.0f);

	unsigned int tmpIndices[] = { 
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2,
		 };
	memcpy(Indices, tmpIndices, sizeof(Indices));

	m_worldMt4.identity();

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	
	//m_pipe.Scale(0.25, 0.25, 1);

	auto winrt = OGLGE::Instance()->getWindowsRect();
	m_pipe.setPerspectiveProj(60, winrt.width, winrt.height, 1.0f, 100.0f);
}

void JikeTest::update(float ft) {
	m_pipe.WorldPos(0, 0, -5); 
	m_pipe.GetTrans(m_worldMt4);
}

void JikeTest::keyInput(unsigned char param, int x, int y)
{
	static float va = 0.0f;
	
	if (param == 97)
	{
		va -= 0.05f;
		m_pipe.Rotate(0, va, 0);
	}
	else if (param == 100)
	{
		va += 0.05f;
		m_pipe.Rotate(0, va, 0);
	}
}

void JikeTest::draw() {
	glUseProgram(m_programObj);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);



	glEnableVertexAttribArray(positionLoc);

	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//第三个参数是 GL_TRUE 是因为我们以行优先的方式提供矩阵的。我们也可以将第三个参数为 GL_FALSE，但是这样的话我们需要转置矩阵的值，因为 C/C++ 中内存的排列仍然是行优先，但是 OpenGL 将认为我们提供的前四个值实际上是一个矩阵的列，并做相应处理
	glUniformMatrix4fv(m_userUniforms["gWorld"].location, 1, GL_FALSE, (const GLfloat*)&m_worldMt4);
	

	//glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices),
		Indices, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);



	glDisableVertexAttribArray(0);
}