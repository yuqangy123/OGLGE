#pragma once
#include "shaders.h"
#include <map>
#include "Mat4.h"
#include "Quaternion.h"


class technique
{
public:
	technique();
	~technique();

	virtual void init();
	bool addShader(const char* vshFile, const char* fshFile);
	void enable();

	const uniformUnit* getUniforms(const char* name);
	GLint getUniformLocation(const char* name);

protected:
	bool linkShader();
	void initUniforms();

protected:
	GLuint m_shaderProg = 0;
	GLuint m_vshShader = 0;
	GLuint m_fshShader = 0;
	std::map<std::string, uniformUnit> m_userUniforms;

};

