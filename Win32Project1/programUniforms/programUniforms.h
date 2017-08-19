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

//获取所有uniform常量的信息
void getProgramUniforms(GLuint programObject, std::vector<uniformInfo>& res);