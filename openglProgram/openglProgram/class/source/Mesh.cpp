#include "Mesh.h"
#include <sstream>
#include "Shader.h"
Mesh::Mesh(std::vector<Vertex> vertex, std::vector<GLuint> indices, std::vector<Texture> texture)
{
	m_vertices = vertex;
	m_indices = indices;
	m_texture = texture;
	setupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
	
	//vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	//normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	//texcoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glBindVertexArray(0);
}

void Mesh::draw(Shader* shader)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (int i = 0; i < m_texture.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::stringstream ss;
		std::string number;
		std::string name = m_texture[i].type;
		if (name == "texture_diffuse")
			ss << diffuseNr++;
		else if(name == "texture_specular")
			ss << specularNr++;

		number = ss.str();
		GLuint location = glGetUniformLocation(shader->program, ("material_" + name + number).c_str());
		//std::cout << "location:" << name << "  " << ("material_" + name + number).c_str() << "  " << location << std::endl;
		glUniform1f(location, i);
		glBindTexture(GL_TEXTURE_2D, m_texture[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}