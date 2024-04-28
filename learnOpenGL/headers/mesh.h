#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <vector>
#include <headers/Shader.h>
#define MAX_BONE_INFLUENCE 4

struct Vertex {

	glm::vec3 Position;
	glm::vec3 Normals;
	glm::vec2 TextureCoords;

	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
	unsigned int ID;
	std::string type;
	std::string path;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setup();
	}

	void Draw(Shader& shader) {
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0+i);

			std::string number;
			std::string name = textures[i].type;

			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular")
			{
				number = std::to_string(specularNr++);
			}
			else if (name == "texture_normal")
			{
				number = std::to_string(normalNr++);
			}
			else if (name == "texture_height")
			{
				number = std::to_string(heightNr++);
			}

			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()),i);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
			
		}

		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

private:
	unsigned int vBuffer;
	unsigned int EBO;
	void setup()
	{
		glGenBuffers(1, &vBuffer);
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Normals));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		

	}
};
