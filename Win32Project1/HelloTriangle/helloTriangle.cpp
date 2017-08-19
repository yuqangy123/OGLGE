#include "stdafx.h"
#include "helloTriangle.h"
#include "shaders.h"


helloTriangle::helloTriangle()
{
	init();
}


helloTriangle::~helloTriangle()
{
}

void Draw()
{
	UserData* uerdata = (UserData*)esContext->userData;

	GLfloat vVertices[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	//set viewport
	glViewport(0, 0, esContext->width, esContext->height);
	
	//clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//Use the program object
	glUseProgram(uerdata->programObject);

	//load vertex data绑定定点属性数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FLOAT, 0, vVertices);

	//使能定点属性数组，0是上面的属性索引。glDisableVertexAttribArray则使用常量定点属性
	glEnableVertexAttribArray(0);
	//glValidateProgram(uerdata->programObject);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

int helloTriangle::init()
{
	ESContext esContext;
	UserData userdata;

	memset(&esContext, 0x0, sizeof(ESContext));
	esContext.userData = &userdata;

	esCreateWindow(&esContext, "myOGLProject", 640*1.0, 480*1.0, ES_WINDOW_RGB);

	const char vshSrc[] =
		"attribute vec4 v_Position;	\n"
		"void main()	\n"
		"{	\n"
		"	gl_Position = v_Position;	\n"
		"}	\n";
	GLuint vshShader = loadShader(GL_VERTEX_SHADER, vshSrc);

	const char fshSrc[] =
		"precision mediump float;	\n"
		"void main()	\n"
		"{	\n"
		"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);	\n"
		"}	\n";
	GLuint fshShader = loadShader(GL_FRAGMENT_SHADER, fshSrc);

	GLuint programObj = glCreateProgram();
	if (0 == programObj)
		return -1;

	glAttachShader(programObj, vshShader);
	glAttachShader(programObj, fshShader);

	//绑定定点属性，glGetAttribLocation返回制定属性的索引值
	glBindAttribLocation(programObj, 0, "v_Position");

	glLinkProgram(programObj);

	GLint linked;
	glGetProgramiv(programObj, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint infoLen = 0;
		glGetProgramiv(programObj, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1)
		{
			char* infoLog = new char[sizeof(char)*infoLen];
			glGetProgramInfoLog(programObj, infoLen, NULL, infoLog);
			esLogMessage("error link program: %s\r\n", infoLog);
			delete infoLog;
		}
		glDeleteProgram(programObj);
		return -1;
	}

	userdata.programObject = programObj;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	esRegisterDrawFunc(&esContext, Draw);

	esMainLoop(&esContext);

	return 0;
}