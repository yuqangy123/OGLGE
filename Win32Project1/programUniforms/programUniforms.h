#pragma once

#include <string>
#include <vector>
#include "GL/glew.h"

typedef struct
{
	GLint	location;
	GLint	type;
	std::string	name;
}uniformInfo;

//��ȡ����uniform��������Ϣ
void getProgramUniforms(GLuint programObject, std::vector<uniformInfo>& res);