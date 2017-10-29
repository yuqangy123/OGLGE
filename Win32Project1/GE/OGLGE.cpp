#include "stdafx.h"
#include "OGLGE.h"
#include "InPutControl.h"
#include "SceneManager.h"


#include "Test.h"
#include "Texture2D.h"
#include "triangleVertebralTexture.h"
#include "triangleVertebralLight.h"
#include "meshObject.h"
#include "meshShadowMap.h"
#include "meshShadow.h"
#include "meshbumpNormal.h"
#include "Skybox.h"
#include "BillboardList.h"
#include "meshSkinnedAnimation.h"
#include "GBuffer.h"
#include "xFreeTypeFont.h"


DECLARE_SINGLETON_MEMBER(OGLGE);
OGLGE::OGLGE()
{
}


OGLGE::~OGLGE()
{
}



static double st_startTime = (double)GetCurrentTimeMillis();
static double st_lastTime = (double)GetCurrentTimeMillis();
void OGLGE::draw()
{
	double currTime = (double)GetCurrentTimeMillis();
	
	float RunningTime = (float)(currTime - st_startTime) / 1000.0f;
	OGLGE::update(RunningTime);
	OGLGE::Instance()->m_director->draw();
}

void OGLGE::keyInput(unsigned char param, int x, int y)
{
	InPutControlIns.keyInput(param, x, y);
}

void OGLGE::mouseInput(int button, int state, int x, int y)
{
	InPutControlIns.mouseInput(button, state, x, y);
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

	const GLubyte * name = glGetString(GL_VENDOR);
	const GLubyte * biaoshifu = glGetString(GL_RENDERER);
	const GLubyte * OpenGLVersion = glGetString(GL_VERSION);
	const GLubyte * gluVersion = gluGetString(GLU_VERSION);
	printf("OpenGL实现厂商的名字：%s\n", name);
	printf("渲染器标识符：%s\n", biaoshifu);
	printf("OOpenGL实现的版本号：%s\n", OpenGLVersion);
	printf("OGLU工具库版本：%s\n", gluVersion);

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

void testBumpNormalMesh()
{
	meshObject* m = new meshObject();
	m->loadMesh("box.obj", "content");
	m->setPosition(0, 0, -3);
	//scene->addNode(m);

	meshbumpNormal* bumpNormal = new meshbumpNormal();
	bumpNormal->setPosition(0, 0, -1);
	bumpNormal->loadMesh("content/box.obj", "content/normal_map.jpg");
	SceneIns.addNode(bumpNormal);

	DefaultCamera->setEyePosition(2.5, 1.5, 0);
	DefaultCamera->setTargetPosition(Vector3(0, -1, -4));
	DefaultCamera->setFreeCamera(true);
}
void testSkybox()
{
	SkyBox* box = new SkyBox();

	std::vector<std::string> imgs;
	imgs.push_back("content/sp3right.jpg");
	imgs.push_back("content/sp3left.jpg");
	imgs.push_back("content/sp3top.jpg");
	imgs.push_back("content/sp3bot.jpg");
	imgs.push_back("content/sp3front.jpg");
	imgs.push_back("content/sp3back.jpg");
	box->loadMesh(imgs);
	

	SceneIns.addNode(box);

	DefaultCamera->setEyePosition(0, 0, 0);
	DefaultCamera->setTargetPosition(Vector3(0, -1, 0));
	DefaultCamera->setFreeCamera(true);
}
void testTest()
{
	Test* t = new Test();
	SceneIns.addNode(t);
}
void testBillboardList()
{
	BillboardList* billboard = new BillboardList();

	SceneIns.addNode(billboard);
	DefaultCamera->setFreeCamera(true);
}
void testSkinnedAnimation()
{
	meshSkinnedAnimation* test = new meshSkinnedAnimation();
	test->loadMesh("content/boblampclean.md5mesh", Vector3(0, 0, -10), Vector3(1.0, 1.0, 1.0));
	SceneIns.addNode(test);
}

void testGBuffer()
{
	GBuffer* gbuff = new GBuffer();
	SceneIns.addNode(gbuff);
}

void testXFreeType()
{
	xFreeTypeFont* font = new xFreeTypeFont();
	font->setString(L"舍得离开房间为了开发就完了开发就fps:1234567894112345678YUJK<JKLsdfgklmwsf;90", 500, 100);
	font->setPosition(100, 100);
	SceneIns.addNode(font);
}

void OGLGE::test()
{
	

	//JikeTest * jk = new JikeTest(); scene->addNode(jk);

	//triangleVertebralTexture * jk = new triangleVertebralTexture(); scene->addNode(jk);

	//triangleVertebralLight * jk2 = new triangleVertebralLight(); scene->addNode(jk2);
	
	/*
	meshShadowMap* shadowMapA = new meshShadowMap();
	shadowMapA->loadMesh("content/jeep.obj", Vector3(0, 0, -30), Vector3(0.05, 0.05, 0.05));
	//shadowMapA->loadMesh("content/hheli.obj", Vector3(0, 0, -70), Vector3(0.2, 0.2, 0.2));
	shadowMapA->setLightPosition(-50, 40, -50);
	shadowMapA->setLightTargetPosition(0, 0, -50);
	shadowMapA->setPosition(0, 0, -70);
	shadowMapA->setScale(15);
	SceneIns.addNode(shadowMapA);
	*/

	//meshObject* m = new meshObject(); m->loadMesh("content/jeep.obj"); m->setPosition(0, 0, -50); m->setScale(0.05); SceneIns.addNode(m);
	//meshObject* m = new meshObject(); m->loadMesh("sponza.obj", "content/crytek_sponza"); m->setPosition(0, 0, -50); m->setScale(0.05); SceneIns.addNode(m);


	
	
	
	//meshObject* m2 = new meshObject(); m2->loadMesh("content/hheli.obj"); m2->setPosition(0, 0, -90); m2->setScale(0.2); SceneIns.addNode(m2);
	//meshObject* m3 = new meshObject(); m3->loadMesh("content/hheli.obj"); m3->setPosition(0, 0, 0); m3->setScale(0.2); scene->addNode(m3);
	
	/*
	meshShadow* shadowm = new meshShadow();
	shadowm->loadMesh("content/jeep.obj", Vector3(30, -30, -30), Vector3(0.05, 0.05, 0.05));
	shadowm->loadMesh("content/hheli.obj", Vector3(30, -30, -00), Vector3(0.1, 0.1, 0.1));
	shadowm->setLightPosition(-50, 40, -30);
	shadowm->setTargetPosition(30, -30, -30);
	SceneIns.addNode(shadowm);
	*/
	
	//DefaultCamera->setEyePosition(0, 0, 0);
	//DefaultCamera->setEyePosition(-50, 40, -30); shadowMapA->Rotate(-45, 90, 0);
	//DefaultCamera->setTargetPosition(Vector3(0, 0, -50));
	//DefaultCamera->setFreeCamera(true);

	//Texture2D* texture2 = new Texture2D("res/sao.jpg"); st_scene->addNode(texture2); texture2->setPosition(m_winRt.width / 2.0, 50);

	//Texture2D* texture1 = new Texture2D("res/guo.jpg"); SceneIns.addNode(texture1); texture1->setScale(0.1);

	//testBumpNormalMesh();

	//testSkybox();

	//testBillboardList();

	//testTest();

	//testSkinnedAnimation();

	//testGBuffer();

	testXFreeType();
}


void OGLGE::start()
{
	glutMainLoop();
}

void OGLGE::update(float ft)
{
	OGLGE::Instance()->m_director->update(ft);
	
}