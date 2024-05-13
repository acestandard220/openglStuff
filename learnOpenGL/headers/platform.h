#pragma once
#include <GL/glew.h>
#include <headers/Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

/// <TODO>
/// Add texture coordinates [DONE]
/// Update Vertex Attrib... [DONE]
/// <TODO>

class Platform
{
	public:
		unsigned int texture;
		Platform(const char* texturePath)
		{		
			unsigned int vBuffer;
			unsigned int EBO;
			
			glGenBuffers(1, &vBuffer);
			glGenBuffers(1, &EBO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBindBuffer(GL_ARRAY_BUFFER, vBuffer);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
			
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
			glEnableVertexAttribArray(3);

			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
			glEnableVertexAttribArray(4);

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			int height, width, nChannel;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load(texturePath, &width, &height, &nChannel,0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}else{std::cout<<"NOPE"; }
			stbi_image_free(data);

		}
		void draw(Shader& shader, glm::vec3 camPos, glm::vec3 camFront, glm::vec3 camUp)
		{
			shader.use();
			glBindTexture(GL_TEXTURE_2D, texture);
			transMatrix(shader,camPos, camFront, camUp);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		} 
	private:
		float vertex[12+8] = {
				 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
				 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
				-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
				-0.5f,  0.5f, 0.0f,  0.0f, 1.0f
		};
		unsigned int indices[6] = {
			0,1,3,
			1,2,3
		};
		void transMatrix(Shader& shader,glm::vec3 camPos,glm::vec3 camFront,glm::vec3 camUp)
		{
			glm::mat4 modelMatrix(1.0f);
			glm::mat4 viewMatrix(1.0f);
			glm::mat4 projMatrix(1.0f);

			//modelMatrix = glm::rotate(modelMatrix,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(1.1f));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -1.0f));
			viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
			projMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

			shader.setUniformMatrix(*"model", modelMatrix);
			shader.setUniformMatrix(*"view", viewMatrix);
			shader.setUniformMatrix(*"proj", projMatrix);
			shader.setInt(*"tex", texture);


		}
};
