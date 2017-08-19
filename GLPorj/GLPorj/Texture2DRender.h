#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Mat4.h"
#include "Quaternion.h"
#include "esUtil.h"
#include "shaders.h"
#include <string>
#include <map>
#include "Singleton.h"


typedef struct
{
	std::string		name;
	GLenum			type;
	GLsizei			size;
	GLint			location;
}uniformUnit;

/**The max number of vertices in a vertex buffer object.*/
static const int VBO_SIZE = 65536;
/**The max number of indices in a index buffer.*/
static const int INDEX_VBO_SIZE = VBO_SIZE * 6 / 4;

typedef struct 
{
	Vector3     vertices[4];            // 12 bytes
	Vector2    texCoords;           // 8 bytes
}V3F_T2F;

class Texture2DRender :public CSingleton<Texture2DRender>
{
public:
	Texture2DRender();
	~Texture2DRender();

	void begin();
	void render(GLuint texturesID, V3F_T2F* vts, unsigned int size);
	void end();

protected:
	void init();

protected:
	GLuint m_programObj = 0;
	GLuint m_vshShader = 0;
	GLuint m_fshShader = 0;
	GLuint m_vbo[2];
	std::map<std::string, uniformUnit> m_userUniforms;
	GLuint m_lastTexturesID;
};

