#include "stdafx.h"
#include "assert.h"
#include "Texture2DRender.h"
#include "FileUnits.h"
#include <stddef.h> //used offsetof
#include <OGLGE.h>
#include "TextureManager.h"


DECLARE_SINGLETON_MEMBER(Texture2DRender)
Texture2DRender::Texture2DRender()
{
	m_vbo[0] = 0;
	m_vbo[1] = 0;

	init();
}


Texture2DRender::~Texture2DRender()
{
}

void Texture2DRender::init()
{
	m_tech = new texture2dTechnique();
	m_tech->init();

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

void Texture2DRender::render2(GLuint texturesID, V3F_T2F* vts, unsigned int size, Mat4& projection)
{

	GLfloat vVertices[] = { -0.5f,  0.5f, 0.0f, 1.5f,  // Position 0
		0.0f,  0.0f,              // TexCoord 0 
		-0.5f, -0.5f, 0.0f, 0.75f, // Position 1
		0.0f,  1.0f,              // TexCoord 1
		0.5f, -0.5f, 0.0f, 0.75f, // Position 2
		1.0f,  1.0f,              // TexCoord 2
		0.5f,  0.5f, 0.0f, 1.5f,  // Position 3
		1.0f,  0.0f               // TexCoord 3
	};
	
	GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, vVertices, GL_DYNAMIC_DRAW);

	// Load the vertex position
	glVertexAttribPointer(m_tech->positionLoc, 4, GL_FLOAT,
		GL_FALSE, 6 * sizeof(GLfloat), (const void*)0);
	// Load the texture coordinate
	glVertexAttribPointer(m_tech->texCoordLoc, 2, GL_FLOAT,
		GL_FALSE, 6 * sizeof(GLfloat), (const void*)(4 * sizeof(GLfloat)));

	glEnableVertexAttribArray(m_tech->positionLoc);
	glEnableVertexAttribArray(m_tech->texCoordLoc);

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturesID);

	// Set the sampler texture unit to 0
	glUniform1i(m_tech->texCoordLoc, 0);

	// Draw quad with nearest sampling
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indices, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

void Texture2DRender::render4(GLuint texturesID, V3F_T2F* vts, unsigned int size, const Matrix4f& projection)
{
	m_tech->enable();
	GLfloat vVertices[] = { -0.5f,  0.5f, 0.0f, 1.0f,  // Position 0
		-0.5f, -0.5f, 0.0f, 1.0f, // Position 1
		0.5f, -0.5f, 0.0f, 1.0f, // Position 2
		0.5f,  0.5f, 0.0f, 1.0f,  // Position 3
	};
	GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, vVertices, GL_STATIC_DRAW);
	// Load the vertex position
	glVertexAttribPointer(m_tech->positionLoc, 4, GL_FLOAT,
		GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(m_tech->positionLoc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indices, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

}

void Texture2DRender::render3(GLuint texturesID, V3F_T2F* vts, unsigned int size, Mat4& projection)
{

	GLfloat vVertices[] = { -0.5f,  0.5f, 0.0f, 1.5f,  // Position 0
		0.0f,  0.0f,              // TexCoord 0 
		-0.5f, -0.5f, 0.0f, 0.75f, // Position 1
		0.0f,  1.0f,              // TexCoord 1
		0.5f, -0.5f, 0.0f, 0.75f, // Position 2
		1.0f,  1.0f,              // TexCoord 2
		0.5f,  0.5f, 0.0f, 1.5f,  // Position 3
		1.0f,  0.0f               // TexCoord 3
	};

	GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, vVertices, GL_STATIC_DRAW);

	// Load the vertex position
	glVertexAttribPointer(m_tech->positionLoc, 4, GL_FLOAT,
		GL_FALSE, 6 * sizeof(GLfloat), (const void*)0);
	// Load the texture coordinate
	glVertexAttribPointer(m_tech->texCoordLoc, 2, GL_FLOAT,
		GL_FALSE, 6 * sizeof(GLfloat), (const void*)(4 * sizeof(GLfloat)));

	glEnableVertexAttribArray(m_tech->positionLoc);
	glEnableVertexAttribArray(m_tech->texCoordLoc);

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturesID);

	// Set the sampler texture unit to 0
	glUniform1i(m_tech->getUniformLocation("viewMatrix"), 0);

	// Draw quad with nearest sampling
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indices, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

void Texture2DRender::render(GLuint texturesID, V3F_T2F* vts, unsigned int size, const Matrix4f& viewTrans)
{
	m_tech->enable();

	const size_t kQuadSize = sizeof(V3F_T2F);

	
	//Copy vertices/indices to GL objects
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, kQuadSize * size, vts, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(m_tech->positionLoc, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*)offsetof(V3F_T2F, vertices));
	glVertexAttribPointer(m_tech->texCoordLoc, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*)offsetof(V3F_T2F, texCoords));

	glEnableVertexAttribArray(m_tech->positionLoc);
	glEnableVertexAttribArray(m_tech->texCoordLoc);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	//GLushort _indices[6] = { 0,1,2,3,2,1 };
	GLushort _indices[6] = { 0, 1, 2, 0, 2, 3 };
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * 6, _indices, GL_STATIC_DRAW);

	if (m_lastTexturesID != texturesID)
	{
		glActiveTexture(GL_TEXTURE0);
		TextureMgr->bindTexture(texturesID);
		glEnable(GL_TEXTURE_2D);

		m_lastTexturesID = texturesID;

		// Set the sampler texture unit to 0
		glUniform1i(m_tech->getUniformLocation("s_texture"), 0);
	}
	glUniformMatrix4fv(m_tech->getUniformLocation("viewMatrix"), 1, GL_TRUE, (const float*)viewTrans.m);


	//Draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(m_tech->positionLoc);
	glDisableVertexAttribArray(m_tech->texCoordLoc);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Texture2DRender::render1(GLuint texturesID, V3F_T2F* vts, unsigned int size, const Matrix4f& viewTrans)
{
	m_tech->enable();

	const size_t kQuadSize = sizeof(V3F_T2F);


	/**************  Copy vertices/indices to GL objects *************/
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, kQuadSize * size, vts, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(m_tech->positionLoc, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*)offsetof(V3F_T2F, vertices));
	//glVertexAttribPointer(m_tech->texCoordLoc, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*)offsetof(V3F_T2F, texCoords));

	glEnableVertexAttribArray(m_tech->positionLoc);
	//glEnableVertexAttribArray(m_tech->texCoordLoc);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	//GLushort _indices[6] = { 0,1,2,3,2,1 };
	GLushort _indices[6] = { 0, 1, 2, 0, 2, 3 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * 6, _indices, GL_STATIC_DRAW);

	/*
	if (m_lastTexturesID != texturesID)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturesID);
		glEnable(GL_TEXTURE_2D);

		m_lastTexturesID = texturesID;

		// Set the sampler texture unit to 0
		glUniform1i(m_tech->getUniformLocation("s_texture"), 0);
	}

	Matrix4f mat;
	mat.identity();
	glUniformMatrix4fv(m_tech->getUniformLocation("viewMatrix"), 1, GL_TRUE, (const float*)mat.m);
	//glUniformMatrix4fv(m_tech->getUniformLocation("viewMatrix"), 1, GL_TRUE, (const float*)viewTrans.m);

	*/

	/************** 3: Draw *************/
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	//glDisableVertexAttribArray(m_tech->positionLoc);
	//glDisableVertexAttribArray(m_tech->texCoordLoc);
}