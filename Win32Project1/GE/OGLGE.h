#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "ogldev_util.h"

#include "OGLGEBase.h"
#include "Rect.h"

#define DefaultCamera OGLGE::Instance()->getDirector()->getDefaultCamera()
#define DefaultCamera2D OGLGE::Instance()->getDirector()->get2DCamera()

class OGLGE: public CSingleton<OGLGE> , public OGLGEBase
{
public:
	OGLGE();
	~OGLGE();

	void init(unsigned int w, unsigned int h);
	void test();
	void start();

	

	static void update(float ft);
	static void draw();
	static void keyInput(unsigned char param, int x, int y);
	static void mouseInput(int button, int state, int x, int y);
	static void mouseMove(int x, int y);
	
	void initGLContext(int argc, char** argv);

};

