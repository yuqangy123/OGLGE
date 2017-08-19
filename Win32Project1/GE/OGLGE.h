#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"


#include "Singleton.h"
#include "Rect.h"
#include "Director.h"
#include "Scene.h"



#define DefaultCamera OGLGE::Instance()->getDirector()->getDefaultCamera()
#define DefaultCamera2D OGLGE::Instance()->getDirector()->get2DCamera()

class OGLGE:public CSingleton<OGLGE>
{
public:
	OGLGE();
	~OGLGE();

	void init(int argc, char** argv);
	void test();
	void start();

	//get windows size
	Rect getWindowsRect() { return m_winRt; }
	CScene* getRunScene() { return m_director->getRunScene(); }
	CDirector* getDirector() { return m_director; }


	static void update(float ft);
	static void draw();
	static void keyInput(unsigned char param, int x, int y);
	static void mouseInput(int button, int state, int x, int y);
	static void mouseMove(int x, int y);
	

protected:
	void initGLContext(int argc, char** argv);

protected:

	CDirector* m_director;
	Rect m_winRt;
};

