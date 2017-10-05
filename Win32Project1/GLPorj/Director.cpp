#include "stdafx.h"
#include "Director.h"
#include "OGLGE.h"
#include "SceneManager.h"
#include "SceneNode.h"
#include "InPutControl.h"

CDirector::CDirector()
{
}


CDirector::~CDirector()
{

}

void CDirector::init()
{
	SceneManagerIns;
	InPutControlIns;

	auto winrt = OGLGE::Instance()->getWindowsRect();

	//init camera
	m_Camera3D = new Camera3D(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, -1.0), Vector3(0.0, 1.0, 0.0));	
	m_Camera3D->SetPerspectivePro(60, winrt.width, winrt.height, 0.1f, 100.0f);
	InPutControlIns.addListenNode(m_Camera3D);
	
	m_Camera2D = new Camera2D(Vector2(0.0, 0.0), winrt.width, winrt.height);

	SceneIns.addNode(m_Camera3D);
	SceneIns.addNode(m_Camera2D);
}

void CDirector::update(float ft)
{
	SceneManagerIns.update(ft);
}

void CDirector::draw()
{
	// Clear the color buffer
	glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	SceneManagerIns.draw();

	glutSwapBuffers();
}



