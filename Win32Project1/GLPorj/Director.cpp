#include "stdafx.h"
#include "Director.h"
#include "OGLGE.h"
#include "SceneManager.h"
#include "SceneNode.h"
#include "InPutControl.h"

Director::Director()
{
}


Director::~Director()
{

}

void Director::init()
{
	SceneManagerIns;
	InPutControlIns;
	statsNodeIns;

	auto winrt = OGLGE::Instance()->getWindowsRect();

	//init camera
	m_Camera3D = new Camera3D(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, -1.0), Vector3(0.0, 1.0, 0.0));	
	m_Camera3D->SetPerspectivePro(60, winrt.width, winrt.height, 0.1f, 200.0f);
	InPutControlIns.addListenNode(m_Camera3D);
	
	m_Camera2D = new Camera2D(Vector2(0.0, 0.0), winrt.width, winrt.height);

	DefaultSceneIns.addNode(m_Camera3D);
	DefaultSceneIns.addNode(m_Camera2D);

	
	displayStats(true);
}

void Director::update(float ft)
{
	statsNodeIns.update();
	SceneManagerIns.update(ft);
}

void Director::draw()
{
	// Clear the color buffer
	glClearColor(0.0f, 0.f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	SceneManagerIns.draw();
	statsNodeIns.draw();

	glutSwapBuffers();
}

void Director::displayStats(bool b)
{
	statsNodeIns.displayStats(b);
}



