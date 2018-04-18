#pragma once

#include <iostream>
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <vector>
#include "common.h"

class Shader;
class Mesh
{
public:	


	Mesh(std::vector<Vertex>, std::vector<GLuint>, std::vector<Texture>);
	~Mesh();
	void setupMesh();
	void draw(Shader* shader);
private:

	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<Texture> m_texture;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};
