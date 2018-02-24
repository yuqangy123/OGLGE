#include "stdafx.h"
#include "technique.h"
#include "FileUnits.h"

technique::technique()
{
	m_shaderProg = 0;
	m_vshShader = 0;
	m_fshShader = 0;
	m_gshShader = 0;
}


technique::~technique()
{
	clearShader(m_vshShader);
	clearShader(m_fshShader);
	clearShader(m_gshShader);

	glDeleteProgram(m_shaderProg);
}

void technique::init()
{
	m_shaderProg = glCreateProgram();
	if (0 == m_shaderProg)
	{
		ERROR("create Texture2D Shader program failed");
		return;
	}
}

bool technique::addShader(const char* vshFile, const char* fshFile, const char* gshFile)
{
	if (0 == m_shaderProg)
		return false;

	if (nullptr != vshFile)
	{
		std::string str;
		FileUnits::Instance()->getFileData(vshFile, str);
		m_vshShader = loadShader(GL_VERTEX_SHADER, str.c_str());
		glAttachShader(m_shaderProg, m_vshShader);
	}

	if (nullptr != fshFile)
	{
		std::string str;
		FileUnits::Instance()->getFileData(fshFile, str);
		m_fshShader = loadShader(GL_FRAGMENT_SHADER, str.c_str());
		glAttachShader(m_shaderProg, m_fshShader);
	}
	
	if (nullptr != gshFile)
	{
		std::string str;
		FileUnits::Instance()->getFileData(gshFile, str);
		m_gshShader = loadShader(GL_GEOMETRY_SHADER, str.c_str());
		glAttachShader(m_shaderProg, m_gshShader);
	}
	
	

	return linkShader();
}

bool technique::linkShader()
{
	//need linkProgram front init attri and Uniforms
	glLinkProgram(m_shaderProg);
	GLint status = GL_TRUE;
	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLen = 0;
		glGetProgramiv(m_shaderProg, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1)
		{
			char* infoLog = new char[sizeof(char) * infoLen];
			glGetProgramInfoLog(m_shaderProg, infoLen, NULL, infoLog);
			printf("ERROR: Failed to link program:\r\n%s", infoLog);
			assert(0);
			delete infoLog;
		}
		glDeleteProgram(m_shaderProg);
		return false;
	}
	initUniforms();

	return true;
}

std::string technique::getLastError()
{
	std::string err;
	//need linkProgram front init attri and Uniforms
	glLinkProgram(m_shaderProg);
	GLint status = GL_TRUE;
	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLen = 0;
		glGetProgramiv(m_shaderProg, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1)
		{
			char* infoLog = new char[sizeof(char)* infoLen];
			glGetProgramInfoLog(m_shaderProg, infoLen, NULL, infoLog);
			printf("ERROR: Failed to link program:\r\n%s", infoLog);
			assert(0);
			delete infoLog;
		}
		glDeleteProgram(m_shaderProg);
		return false;
	}
	initUniforms();

	return "";
}

void technique::initUniforms()
{
	// Query and store uniforms from the program.
	GLint activeUniforms;
	glGetProgramiv(m_shaderProg, GL_ACTIVE_UNIFORMS, &activeUniforms);
	if (activeUniforms > 0)
	{
		GLint length;
		glGetProgramiv(m_shaderProg, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
		if (length > 0)
		{
			char* uniformName = new char[length + 1];

			for (int i = 0; i < activeUniforms; ++i)
			{
				uniformUnit u;
				// Query uniform info.
				glGetActiveUniform(m_shaderProg, i, length, nullptr, &u.size, &u.type, uniformName);
				uniformName[length] = '\0';
				u.name.assign(uniformName);

				int location = glGetUniformLocation(m_shaderProg, uniformName);
				u.location = location;
				GLenum __gl_error_code = glGetError();
				if (__gl_error_code != GL_NO_ERROR)
					printf("error: 0x%x  uniformName: %s", (int)__gl_error_code, uniformName);

				//assert(__gl_error_code == GL_NO_ERROR);

				m_userUniforms[u.name] = u;
			}
			delete uniformName;
		}
	}
	else
	{
		char ErrorLog[1024];
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), nullptr, ErrorLog);
		printf("Error linking shader program: '%s'\n", ErrorLog);
	}
}

void technique::enable()
{
	if (0 != m_shaderProg)
		glUseProgram(m_shaderProg);
}
void technique::disable()
{
	glUseProgram(0);
}

const uniformUnit* technique::getUniforms(const char* name)
{
	if (m_userUniforms.end() == m_userUniforms.find(name))
	{
		return nullptr;
	}
	return &m_userUniforms[name];
}

GLint technique::getUniformLocation(const char* name)
{
	auto unif = getUniforms(name);
	if (nullptr != unif)
		return unif->location;
	return -1;
}

GLint technique::getUniformLocationEx(const char* name)
{
	auto unifLocal = glGetUniformLocation(m_shaderProg, name);

	GLenum __gl_error_code = glGetError();
	if (__gl_error_code != GL_NO_ERROR)
		printf("error: 0x%x  uniformName: %s", (int)__gl_error_code, name);

	return unifLocal;
}

