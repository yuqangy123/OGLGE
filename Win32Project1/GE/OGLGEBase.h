#pragma once

#include "Singleton.h"
#include "Rect.h"
#include "Scene.h"
#include "Director.h"
#include "Scene.h"


class OGLGEBase
{
public:
	OGLGEBase();
	~OGLGEBase();

	virtual void init(unsigned int w, unsigned int h) {};


	//get windows size
	Rect getWindowsRect() { return m_winRt; }
	Director* getDirector() { return m_director; }


	virtual void update(float ft) {};
	virtual void draw() {};
	virtual void keyInput(unsigned char param, int x, int y) {};
	virtual void mouseInput(int button, int state, int x, int y) {};
	virtual void mouseMove(int x, int y) {};
	

protected:
	Rect m_winRt;
	Director* m_director;
};

