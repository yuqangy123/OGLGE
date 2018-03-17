

#include "assert.h"
#include "Test.h"
#include "FileUnits.h"
#include <stddef.h> //used offsetof
#include <OGLGE.h>
#include "Vector3.h"
#include "OGLGE.h"



static float modelZ = 1.0f;
static float rotateY = 0.0f;
static float worldPosZ = -5.0f;
static Vector3 cameraWorldPos;
static Vector3 cameraLookAt(0, 0, -1);

Test::Test()
{
	init();
}


Test::~Test()
{
	if (m_tech) delete m_tech;
}


void Test::CreateVertexBuffer()
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
void Test::CreateIndexBuffer()
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



void Test::init()
{
	
	//m_tech = new testTechnique();
	//m_tech->init();
	
	CreateVertexBuffer();
	CreateIndexBuffer();
}

void Test::update(float ft) {
	
}

void Test::keyInput(unsigned char param, int x, int y)
{
	
}

void Test::mouseInput(int button, int state, int x, int y)
{
	
}

void Test::RenderSceneCB()
{
	//glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_MVPMt4.m);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (GLvoid*)12);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);



	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glutSwapBuffers();
}

void draw_e_effect()
{
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 0);
	glVertex2f(-1, 0);
	glVertex2f(1, 0);
	glEnd();

	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 0);

	const GLfloat xA = 6.0f;
	for (GLfloat x = 0; x < xA; x += 0.005)
	{
		GLfloat fx = ((x / xA)*2.0 - 1.0);
		GLfloat fy = exp(-x)*cosf(2 * PI*x) / 1.0;

		glVertex3f(fx, fy, 0);
	}

	glEnd();
	glPointSize(1);
}
void Test::draw() {
	
	draw_e_effect();

	

	return;
	/*
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(m_tech->positionLoc);

	glVertexAttribPointer(m_tech->positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//第三个参数是 GL_TRUE 是因为我们以行优先的方式提供矩阵的。我们也可以将第三个参数为 GL_FALSE，但是这样的话我们需要转置矩阵的值，因为 C/C++ 中内存的排列仍然是行优先，但是 OpenGL 将认为我们提供的前四个值实际上是一个矩阵的列，并做相应处理
	glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_FALSE, (const GLfloat*)&m_worldMt4);
	

	//glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices),
		Indices, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);



	glDisableVertexAttribArray(m_tech->positionLoc);
	*/
}