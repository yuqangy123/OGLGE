#include "stdafx.h"
#include "assert.h"
#include "Texture2DRender.h"
#include <stddef.h> //used offsetof

const char* vshSrcFile = "shaderFile/texture2d.vsh";
const char* fshSrcFile = "shaderFile/texture2d.fsh";

#define ATTRI_A_POSITION_INDEX	0
#define ATTRI_A_POSITION_NAME		"a_position"
#define ATTRI_A_TEXCOORD_INDEX	1
#define ATTRI_A_TEXCOORD_NAME		"a_position"

Texture2DRender::Texture2DRender()
{
	m_vshShader = 0;
	m_fshShader = 0;
	m_programObj = 0;
	m_vbo[0] = 0;
	m_vbo[1] = 0;

	init();
}


Texture2DRender::~Texture2DRender()
{
	clearShader(m_vshShader);
	clearShader(m_fshShader);

	if (m_programObj != 0)
	{
		glDeleteProgram(m_programObj);
	}
}

void Texture2DRender::init()
{
	if (0 != m_programObj)
		return;

	TCHAR szFilePath[512] = { 0 };
	::GetModuleFileName(NULL, szFilePath + 1, 512);

	m_vshShader = loadShader(GL_VERTEX_SHADER, vshSrcFile);
	m_fshShader = loadShader(GL_FRAGMENT_SHADER, fshSrcFile);
	
	m_programObj = glCreateProgram();
	if (0 == m_programObj)
	{
		ERROR("create Texture2D Shader program failed");
		return;
	}

	glAttachShader(m_programObj, m_vshShader);
	glAttachShader(m_programObj, m_fshShader);

	glBindAttribLocation(m_programObj, ATTRI_A_POSITION_INDEX, ATTRI_A_POSITION_NAME);
	glBindAttribLocation(m_programObj, ATTRI_A_TEXCOORD_INDEX, ATTRI_A_TEXCOORD_NAME);
	
	
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
				u.name.copy(uniformName, length);

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

	//general vbo buffer
	glGenBuffers(2, m_vbo);
}

void Texture2DRender::begin()
{
	m_lastTexturesID = 0;
}
void Texture2DRender::end()
{
	m_lastTexturesID = 0;
}

void Texture2DRender::render(GLuint texturesID, V3F_T2F* vts, unsigned int size)
{
#define kQuadSize sizeof(V3F_T2F)

	/**************  Copy vertices/indices to GL objects *************/
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(V3F_T2F) * size, vts, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(ATTRI_A_POSITION_INDEX);
	glEnableVertexAttribArray(ATTRI_A_TEXCOORD_INDEX);

	glVertexAttribPointer(ATTRI_A_POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*)offsetof(V3F_T2F, vertices));
	glVertexAttribPointer(ATTRI_A_TEXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*)offsetof(V3F_T2F, texCoords));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	GLushort _indices[6] = {0,1,2,3,2,1};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(_indices[0]) * 6, _indices, GL_STATIC_DRAW);

	/**************  Draw *************/
	if (m_lastTexturesID != texturesID)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturesID);
		m_lastTexturesID = texturesID;

		// Set the sampler texture unit to 0
		glUniform1i(m_userUniforms["CC_Texture0"].location, 0);
	}

	/************** 3: Draw *************/
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)(0*sizeof(_indices[0])));

	//clear
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}