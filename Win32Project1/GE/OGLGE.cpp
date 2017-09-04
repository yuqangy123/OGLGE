#include "stdafx.h"
#include "OGLGE.h"

#include "Texture2D.h"
#include "triangleVertebralTexture.h"
#include "triangleVertebralLight.h"
#include "meshObject.h"
#include "meshShadowMap.h"

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
	OGLGE::Instance()->m_director->keyInput(param, x, y);
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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
		return;
	}

	//set viewport
	glViewport(0, 0, m_winRt.width, m_winRt.height);



	//三角形方向（正向，反向），通过指定定点的顺序来规定（顺，逆时针），
	//如果三角新法线方向跟视点方向是同向，则是背面，反之则是正面
	//优化方法，减少运算量，正常的应该是使用深度测试
	glFrontFace(GL_CCW);//指定三角形正面方向，决定是否被剔除，画UI的时候小心，GL_CCW为逆时针
	glCullFace(GL_BACK);//决定正面or反面被剔除
	glEnable(GL_CULL_FACE);//是否执行剔除
						   //glDisable(GL_CULL_FACE);

						   //设置深度测试对比类型，本实例
	glClearDepth(-1.0f);
	glDepthFunc(GL_GREATER);

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

	//Texture2D* texture2 = new Texture2D("res/sao.jpg"); scene->addNode(texture2); texture2->setPosition(m_winRt.width/2.0, 50);

	//Texture2D* texture1 = new Texture2D("res/guo.jpg"); scene->addNode(texture1); texture1->setScale(0.5);

	//JikeTest * jk = new JikeTest(); scene->addNode(jk);

	//triangleVertebralTexture * jk = new triangleVertebralTexture(); scene->addNode(jk);

	//triangleVertebralLight * jk2 = new triangleVertebralLight(); scene->addNode(jk2);
	

	meshShadowMap* shadowMapA = new meshShadowMap();
	shadowMapA->loadMesh("content/jeep.obj", Vector3(0, 0, -30), Vector3(0.05, 0.05, 0.05));
	//shadowMapA->loadMesh("content/hheli.obj", Vector3(0, 0, -70), Vector3(0.2, 0.2, 0.2));

	shadowMapA->setLightPosition(-50, 40, -50);
	shadowMapA->setTargetPosition(0, 0, -50);
	shadowMapA->setPosition(-30, 50, -60);
	shadowMapA->setScale(10);
	scene->addNode(shadowMapA);




	meshObject* m = new meshObject(); m->loadMesh("content/jeep.obj"); m->setPosition(0, 0, -50); m->setScale(0.05); scene->addNode(m);
	//meshObject* m2 = new meshObject(); m2->loadMesh("content/hheli.obj"); m2->setPosition(0, 0, -90); m2->setScale(0.2); scene->addNode(m2);
	//meshObject* m3 = new meshObject(); m3->loadMesh("content/hheli.obj"); m3->setPosition(0, 0, 0); m3->setScale(0.2); scene->addNode(m3);
	
	
	DefaultCamera->setFreeCamera(true);
	DefaultCamera->setEyePosition(-50, 50, -50);
	DefaultCamera->setTargetPosition(Vector3(0, 0, -50));
	
}

void OGLGE::start()
{
	glutMainLoop();
}