#include "stdafx.h"
#include "Director.h"
#include "OGLGE.h"


CDirector::CDirector()
{
}


CDirector::~CDirector()
{

}

void CDirector::init()
{
	auto winrt = OGLGE::Instance()->getWindowsRect();

	//init camera
	m_Camera3D = new Camera3D(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, -1.0), Vector3(0.0, 1.0, 0.0));	
	m_Camera3D->SetPerspectivePro(60, winrt.width, winrt.height, 2.0f, 100.0f);

	m_Camera2D = new Camera2D(Vector2(0.0, 0.0), winrt.width, winrt.height);

	
	//init default scene	
	auto scene = new CScene();
	m_scenes.push_back(scene);
}

void CDirector::update(float ft)
{
	for (auto itr = m_scenes.begin(); itr != m_scenes.end(); ++itr)
	{
		(*itr)->update(ft);
	}
}

void CDirector::draw()
{
	

	
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	//m_defCamera->draw();

	for (auto itr = m_scenes.begin(); itr != m_scenes.end(); ++itr)
	{
		(*itr)->draw();
	}

	glutSwapBuffers();
}

void CDirector::keyInput(unsigned char param, int x, int y)
{
	for (auto itr = m_scenes.begin(); itr != m_scenes.end(); ++itr)
	{
		(*itr)->keyInput(param, x, y);
	}
}

void CDirector::mouseInput(int button, int state, int x, int y)
{
	for (auto itr = m_scenes.begin(); itr != m_scenes.end(); ++itr)
	{
		(*itr)->mouseInput(button, state, x, y);
	}
}

CScene* CDirector::getRunScene()
{
	CScene* ss = nullptr;
	if (m_scenes.size() > 0)
		ss = m_scenes[0];
	return ss;
}

