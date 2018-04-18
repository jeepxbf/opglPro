#pragma once
#include <fstream>
#include <GL\glew.h>
#include "Mesh.h"
#include <vector>
#include "common.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
class Shader;
class Model
{
public:
	Model(GLchar* path);

	~Model();
	void loadFile(std::string path);
	void draw(Shader* shader);
private:
	std::vector<Texture> textureLoaded;
	std::vector<Mesh*> meshes;
	std::string directory;
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	GLuint textureFromFile(const char* path, std::string director);
};

