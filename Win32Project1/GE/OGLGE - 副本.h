#pragma once
#include "Singleton.h"
#include "Rect.h"
#include "esUtil.h"
#include "Director.h"
#include "Scene.h"

typedef struct
{
	GLuint programObject;
}UserData;




class OGLGE:public CSingleton<OGLGE>
{
public:
	OGLGE();
	~OGLGE();

	void init();
	void test();
	void start();

	//get windows size
	Rect getWindowsRect() { return m_winRt; }
	CScene* getRunScene() { return m_director->getRunScene(); }
	CDirector* getDirector() { return m_director; }

	static void update(ESContext* es, float ft);
	static void draw(ESContext* es);
	static void keyInput(ESContext* es, unsigned char param, int x, int y);
	
	ESContext* getEsContext() { return m_esContext; }
protected:
	void initGLContext(const char* name, float win_w, float win_h);

protected:
	ESContext* m_esContext = nullptr;
	CDirector* m_director;
	Rect m_winRt;
};

