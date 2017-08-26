#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Mat4.h"
#include "Quaternion.h"
#include "shaders.h"
#include <string>
#include <map>
#include "Node.h"
#include "Texture2DRender.h"
#include "Pipeline.h"
#include "tech/lightTechnique.h"


class triangleVertebralLight :public Node
{
public:
	triangleVertebralLight();
	~triangleVertebralLight();
	virtual void update(float ft);
	virtual void draw();
	virtual void keyInput(unsigned char param, int x, int y);
	virtual void mouseInput(int button, int state, int x, int y);
	void RenderSceneCB();

protected:
	void init();

	void createVBOIBO_specularLight();
	void createVBOIBO_diffuseLight();
	void createVBOIBO_ambientLight();

	void updateLightUniform();

protected:
	lightTechnique* m_tech = nullptr;
	lightType m_lightType;

	GLuint VBO;
	GLuint IBO;

	Vector3 *m_Vertices = nullptr;
	unsigned int *m_Indices = nullptr;

	GLuint m_exturesID;



	Matrix4f m_MVPMt4;
	Pipeline m_pipe;

	int m_lastMouseX;
	int m_lastMouseY;

	Vector3 m_ambientLightColor;
	float m_ambientLightIntensity;
	Vector3 m_diffuseDirection;
	float m_diffuseIntensity;
	Vector3 m_specularEye;
	float m_specularFactor;
	float m_specularIntensity;
};

