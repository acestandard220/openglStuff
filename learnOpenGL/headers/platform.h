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
		
		Platform(const char* texturePath)
		{		
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			
			glGenBuffers(1, &vBuffer);
			glGenBuffers(1, &EBO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBindBuffer(GL_ARRAY_BUFFER, vBuffer);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
			
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
			glEnableVertexAttribArray(1);

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

			int height, width, nChannel;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load(texturePath, &width, &height, &nChannel,0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}else{std::cout<<"NOPE, couldn't load the texture."; }
			stbi_image_free(data);

			glBindVertexArray(0);
		}

		Platform()
		{
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			glGenBuffers(1, &vBuffer);
			glGenBuffers(1, &EBO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBindBuffer(GL_ARRAY_BUFFER, vBuffer);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glBindVertexArray(0);
		}

		void transMatrix(Shader& shader, glm::vec3 camPos, glm::vec3 camFront, glm::vec3 camUp)
		{
			glm::mat4 modelMatrix(1.0f);
			glm::mat4 viewMatrix(1.0f);
			glm::mat4 projMatrix(1.0f);

			modelMatrix = glm::scale(modelMatrix, glm::vec3(30.0f));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, -0.05, 0.0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
			viewMatrix = glm::lookAt(camPos,camFront+camPos, camUp);
			projMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

			shader.use();
			shader.setUniformMatrix("model", modelMatrix);
			shader.setUniformMatrix("view", viewMatrix);
			shader.setUniformMatrix("proj", projMatrix);

			shader.setUniformMatrix("u_mvp", projMatrix * viewMatrix * modelMatrix);
			


		}
		void transMatrix(Shader& shader, glm::mat4 modelMat, glm::vec3 camPos, glm::vec3 camFront, glm::vec3 camUp)
		{
			glm::mat4 modelMatrix = modelMat;
			glm::mat4 viewMatrix(1.0f);
			glm::mat4 projMatrix(1.0f);

			viewMatrix = glm::lookAt(camPos, camFront + camPos, camUp);
			projMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

			shader.use();
			shader.setUniformMatrix("model", modelMatrix);
			shader.setUniformMatrix("view", viewMatrix);
			shader.setUniformMatrix("proj", projMatrix);
			shader.setInt("text", 1);


		}
		void Draw(Shader& shader)
		{
			shader.use();
			glBindVertexArray(VAO);
			shader.setInt("diffuseTexture", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		} 
	private:
		unsigned int vBuffer;
		unsigned int EBO;
		unsigned int VAO;
		unsigned int texture;
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
	
};
