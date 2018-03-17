

#include "programUniforms.h"

void getProgramUniforms(GLuint programObject, std::vector<uniformInfo>& res)
{
	GLint uniformNum = 0;
	GLint uniformMaxLen = 0;
	glGetProgramiv(programObject, GL_ACTIVE_UNIFORMS, &uniformNum);
	glGetProgramiv(programObject, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxLen);

	char* uniformName = new char[uniformMaxLen];
	memset(uniformName, 0x0, uniformMaxLen*sizeof(char));

	for (int index = 0; index < uniformNum; ++index)
	{
		GLint size;
		GLenum type;
		glGetActiveUniform(programObject, index, uniformMaxLen, NULL, &size, &type, uniformName);
		GLint location = glGetUniformLocation(programObject, uniformName);

		uniformInfo info;
		info.location = location;
		info.name = uniformName;
		info.type = type;

		res.push_back(info);
	}
}