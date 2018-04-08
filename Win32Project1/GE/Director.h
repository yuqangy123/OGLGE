#pragma once
#include "Camera2D.h"
#include "Camera3D.h"
#include "Scene.h"
#include <vector>
#include "statisticalNode.h"

class Director
{
public:
	Director();
	~Director();

	void init();

	void update(float ft);

	void draw();

	Camera3D* getDefaultCamera() { return m_Camera3D; };

	Camera2D* get2DCamera() { return m_Camera2D; };

	void displayStats(bool b);

protected:
	Camera3D* m_Camera3D;
	Camera2D* m_Camera2D;
	
};

