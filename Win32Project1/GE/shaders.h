#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <string>

typedef struct
{
	std::string		name;
	GLenum			type;
	GLsizei			size;
	GLint			location;
}uniformUnit;

// create shader and load shader code
GLuint loadShader(GLenum type, const char* shaderSrc);
void clearShader(GLuint s);