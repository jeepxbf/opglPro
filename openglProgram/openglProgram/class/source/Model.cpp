#include "Model.h"


#include "Mesh.h"
#include "TextureManager.h"
Model::Model(GLchar* path)
{
	loadFile(path);
}

Model::~Model()
{

}

void Model::loadFile(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "load error!" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}
	
void Model::draw(Shader* shader)
{
	for (auto it : meshes)
	{
		it->draw(shader);
	}
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
	
		std::cout << node->mMeshes[i] << std::endl;
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		//把解出来的mesh转换成我们需要的mesh结构 包含顶点，索引，纹理等数据
		meshes.push_back(processMesh(mesh, scene));
	}
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		//set position
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		//set normal
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.f, 0.f);
		}
		vertices.push_back(vertex);
	}

	//indices
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			//一个面包含索引，这些索引定义我们需要绘制的顶点以在那样的顺序提供给每个图元，
			//所以如果我们遍历所有面，把所有面的索引储存到indices向量
			indices.push_back(face.mIndices[j]);
		}
	}

	//set materials
	if (mesh->mMaterialIndex > 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse_maps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
		std::vector<Texture> specular_maps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
	}

	return new Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat,aiTextureType type,std::string typeName)
{
	std::vector<Texture> textures;	
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean skip = false;
		for (auto it : textureLoaded)
		{
			if (it.path == str)
			{
				textures.push_back(it);
				skip = true;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.type = typeName;
			texture.path = str;
			texture.id = textureFromFile(str.C_Str(), this->directory);
			textures.push_back(texture);
			textureLoaded.push_back(texture);
		}	
	}
	std::cout << "1==========================================" << std::endl;
	for (auto it : textures)
	{
		std::cout << it.id << std::endl;
		std::cout << it.path.C_Str() << std::endl;
		std::cout << it.type << std::endl;	
	}
	std::cout << "2==========================================" << std::endl;
	return textures;
}
int getIndex()
{
	static int i = 1;
	return i++;
}
GLuint Model::textureFromFile(const char* path, std::string director)
{
	std::string fileName = std::string(path);
	fileName = director + '/' + fileName;
	GLuint texId = TextureManager::Inst()->LoadTextureFromFile(fileName.c_str(), getIndex(),GL_BGRA,GL_RGBA);
	return texId;
}