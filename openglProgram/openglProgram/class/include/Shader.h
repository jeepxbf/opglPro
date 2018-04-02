#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>


class Shader
{
public:
	Shader(const char* vertexPath,const char* flagmentPath);
	~Shader();
	void use();
	GLuint program = 0;

private:
	bool compile(const char* vCode = NULL, const char* fCode = NULL);
	const char* vertexPath = "";
	const char* fragmentPath = "";
};

