#pragma once
#include "Camera2D.h"
#include "Camera3D.h"
#include "Scene.h"
#include <vector>

class CDirector
{
public:
	CDirector();
	~CDirector();

	void init();
	void update(float ft);
	void draw();
	void keyInput(unsigned char param, int x, int y);
	void mouseInput(int button, int state, int x, int y);
	CScene* getRunScene();
	Camera3D* getDefaultCamera() { return m_Camera3D; };
	Camera2D* get2DCamera() { return m_Camera2D; };

protected:
	Camera3D* m_Camera3D = nullptr;
	Camera2D* m_Camera2D = nullptr;
	std::vector<CScene*> m_scenes;

};

