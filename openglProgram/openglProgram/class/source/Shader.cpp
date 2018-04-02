#include "Shader.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

Shader::~Shader()
{
}
Shader::Shader(const char*vertexPath,const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	fShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try
	{
		vShaderFile.open(vertexPath);	
		std::stringstream vShaderStream;		
		vShaderStream << vShaderFile.rdbuf();	
		vertexCode = vShaderStream.str();
		vShaderFile.close();		
	}
	catch(std::ifstream::failure e)
	{
		std::cout << "vertex file ERROR: " << vertexPath << std::endl;
	}

	try
	{
		fShaderFile.open(fragmentPath);
		std::stringstream fShaderStream;
		fShaderStream << fShaderFile.rdbuf();
		fragmentCode = fShaderStream.str();
		fShaderFile.close();
	}
	catch(std::ifstream::failure e)
	{
		std::cout << "frag file ERROR:" << fragmentPath;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	this->vertexPath = vertexPath;
	this->fragmentPath = fragmentPath;
	compile(vShaderCode, fShaderCode);
}

bool Shader::compile(const char* vCode, const char* fCode)
{
	std::cout << "开始编译.." << std::endl;
	std::cout << "file:" << this->vertexPath << std::endl;
	
	GLuint vertex, fragment;
	GLint vSuccess,fSuccess;
	char infoLog[512];
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &vSuccess);
	if (!vSuccess)
	{
		std::cout << "编译报错，报错文件: " << this->vertexPath << std::endl;
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "vertex compile ERROR:" << std::endl;
		std::cout << infoLog << std::endl;
		return vSuccess;
	}
	else
	{
		std::cout << "vertex compile success .." << std::endl;
	}

	std::cout << "开始编译.." << std::endl;
	std::cout << "file:" << this->fragmentPath << std::endl;
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &fSuccess);
	if (!fSuccess)
	{
		std::cout << "链接报错，报错文件： " << this->fragmentPath << std::endl;
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "fragment compile ERROR:" << std::endl;
		std::cout << infoLog;
		std::cout << std::endl;
		return fSuccess;
	}
	else
		std::cout << "fragment compile success ..." << std::endl;

	std::cout << "开始链接.." << std::endl;
	this->program = glCreateProgram();
	glAttachShader(this->program, vertex);
	glAttachShader(this->program, fragment);
	glLinkProgram(this->program);
	GLint success;
	glGetProgramiv(this->program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->program, 512, NULL, infoLog);
		std::cout << "link ERROR:" << std::endl;
		std::cout << infoLog << std::endl;
	}
	else
		std::cout << "link success .." << std::endl;
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return vSuccess&&fSuccess&&success;
}

void Shader::use()
{
	if (program != 0)
	{
		glUseProgram(program);
	}
}