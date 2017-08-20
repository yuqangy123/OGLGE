#pragma once
#include "pipeLine.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Mat4.h"
#include "Quaternion.h"
#include <string>
#include "Singleton.h"
#include "tech/texture2dTechnique.h"



/**The max number of vertices in a vertex buffer object.*/
static const int VBO_SIZE = 65536;
/**The max number of indices in a index buffer.*/
static const int INDEX_VBO_SIZE = VBO_SIZE * 6 / 4;

typedef struct _V3F_T2F
{
	Vector3     vertices;            // 12 bytes
	Vector2    texCoords;           // 8 bytes
}V3F_T2F;

class Texture2DRender :public CSingleton<Texture2DRender>
{
public:
	Texture2DRender();
	~Texture2DRender();

	void begin();
	void render(GLuint texturesID, V3F_T2F* vts, unsigned int size, const Matrix4f& viewTrans);
	void render1(GLuint texturesID, V3F_T2F* vts, unsigned int size, const Matrix4f& viewTrans);
	void render2(GLuint texturesID, V3F_T2F* vts, unsigned int size, Mat4& viewTransform);
	void render3(GLuint texturesID, V3F_T2F* vts, unsigned int size, Mat4& viewTransform);
	void render4(GLuint texturesID, V3F_T2F* vts, unsigned int size, const Matrix4f& viewTrans);
	void end();

	Pipeline& getPipeline() { m_pipe; };

protected:
	void init();

protected:
	GLuint m_vbo[2];
	Pipeline	m_pipe;
	GLuint m_lastTexturesID;
	texture2dTechnique* m_tech = nullptr;
};

