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
	bool addShader(const char* vshFile = nullptr, const char* fshFile = nullptr, const char* gshFile = nullptr);
	void enable();
	void disable();

	const uniformUnit* getUniforms(const char* name);
	GLint getUniformLocation(const char* name);
	GLint getUniformLocationEx(const char* name);

protected:
	bool linkShader();
	void initUniforms();

protected:
	GLuint m_shaderProg;
	GLuint m_vshShader;
	GLuint m_fshShader;
	GLuint m_gshShader;
	std::map<std::string, uniformUnit> m_userUniforms;

};

