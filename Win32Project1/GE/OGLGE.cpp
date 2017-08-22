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

void OGLGE::update(float ft)
{
	OGLGE::Instance()->m_director->update(ft);
}

void OGLGE::draw()
{
	OGLGE::update(0.0f);
	OGLGE::Instance()->m_director->draw();
}

void OGLGE::keyInput(unsigned char param, int x, int y)
{
	OGLGE::Instance()->m_director->keyInput(param,x,y);
}

void OGLGE::mouseInput(int button, int state, int x, int y)
{
	OGLGE::Instance()->m_director->mouseInput(button, state, x, y);
}

void OGLGE::mouseMove(int x, int y)
{
	OGLGE::mouseInput(-1, -1, x, y);
}

void OGLGE::initGLContext(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(m_winRt.width, m_winRt.height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OGLGE");

	glutDisplayFunc(OGLGE::draw);
	glutIdleFunc(OGLGE::draw);
	glutKeyboardFunc(OGLGE::keyInput);
	glutMouseFunc(OGLGE::mouseInput);
	glutPassiveMotionFunc(OGLGE::mouseMove);


	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return ;
	}

	//set viewport
	glViewport(0, 0, m_winRt.width, m_winRt.height);

	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);

	
	//优化方法，减少运算量，正常的应该是使用深度测试
	glFrontFace(GL_CCW);//指定三角形正面方向，决定是否被剔除，画UI的时候小心
	glCullFace(GL_BACK);//决定正面or反面被剔除
	glEnable(GL_CULL_FACE);//是否执行剔除
	//glDisable(GL_CULL_FACE);

	//开启深度测试，需要初始化一个深度缓冲区
	glEnable(GL_DEPTH_TEST);
}
void OGLGE::init(int argc, char** argv)
{
	m_winRt.width = 720.0;
	m_winRt.height = 640.0;

	initGLContext(argc, argv);
	
	m_director = new CDirector();
	m_director->init();
}

void OGLGE::test()
{
	auto scene = getRunScene();

	Texture2D* texture2 = new Texture2D("res/sao.jpg"); scene->addNode(texture2);
	texture2->setPosition(m_winRt.width/2.0, 50);

	Texture2D* texture1 = new Texture2D("res/guo.jpg"); scene->addNode(texture1);
	texture1->setScale(0.5);

	JikeTest * jk = new JikeTest(); scene->addNode(jk);
	
}

void OGLGE::start()
{
	glutMainLoop();
}