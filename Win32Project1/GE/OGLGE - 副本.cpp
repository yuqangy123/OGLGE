#include "stdafx.h"
#include "OGLGE.h"

#include "Texture2D.h"
#include "JikeTest.h"

DECLARE_SINGLETON_MEMBER(OGLGE);
OGLGE::OGLGE()
{
}


OGLGE::~OGLGE()
{
}

void OGLGE::update(ESContext* es, float ft)
{
	OGLGE::Instance()->m_director->update(ft);
}

void OGLGE::draw(ESContext* es)
{
	OGLGE::Instance()->m_director->draw();
}

void OGLGE::keyInput(ESContext* es, unsigned char param, int x, int y)
{
	OGLGE::Instance()->m_director->keyInput(param,x,y);
}

void OGLGE::initGLContext(const char* name, float win_w, float win_h)
{
	m_esContext = (ESContext*)malloc(sizeof(ESContext));
	UserData *userdata = (UserData*)malloc(sizeof(UserData));

	memset(m_esContext, 0x0, sizeof(ESContext));
	m_esContext->userData = userdata;

	esCreateWindow(m_esContext, name, win_w , win_h,  ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	esRegisterUpdateFunc(m_esContext, OGLGE::update);
	esRegisterDrawFunc(m_esContext, OGLGE::draw);
	esRegisterKeyFunc(m_esContext, OGLGE::keyInput);

	
}
void OGLGE::init()
{
	m_winRt.width = 720.0;
	m_winRt.height = 640.0;

	initGLContext("OGLGE", m_winRt.width, m_winRt.height);
	
	m_director = new CDirector();
	m_director->init();
}

void OGLGE::test()
{
	auto scene = getRunScene();

	//Texture2D* texture = new Texture2D("res/def.jpg");
	//scene->addNode(texture);

	JikeTest * jk = new JikeTest();
	scene->addNode(jk);
}

void OGLGE::start()
{
	esMainLoop(m_esContext);
}