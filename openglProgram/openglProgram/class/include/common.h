#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
#include <assimp\Importer.hpp>

enum enumTexture
{
	box,
	face,
	specularBox,
	model,
};

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	GLuint id;
	std::string type;
	aiString path;
};
