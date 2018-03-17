

#include "shaders.h"

GLuint loadShader(GLenum type, const char* shaderSrc)
{
	GLuint shader;
	shader = glCreateShader(type);
	if (0 == shader)
		return 0;

	//load shader sources
	const GLchar* p[1];
	p[0] = shaderSrc;
	GLint L[1];
	L[0] = strlen(shaderSrc);

	glShaderSource(shader, 1, p, L);

	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1)
		{
			char* infoLog = new char[sizeof(char)*infoLen];
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			printf("%s\r\n", infoLog);
			//esLogMessage("error compile shader: %s\r\n", infoLog);
			delete infoLog;
		}
		clearShader(shader);
		return 0;
	}
	return shader;
}

void clearShader(GLuint s)
{
	if (s && 0 != s)
	{
		glDeleteShader(s);
	}
}