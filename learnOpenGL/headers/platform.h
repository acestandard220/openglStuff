#pragma once
#include <GL/glew.h>
#include <headers/Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

class Platform
{
	public:
		
		Platform()
		{		
			unsigned int vBuffer;
			unsigned int EBO;
			
			glGenBuffers(1, &vBuffer);
			glGenBuffers(1, &EBO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBindBuffer(GL_ARRAY_BUFFER, vBuffer);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
			
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
			glEnableVertexAttribArray(3);
		}
		void draw(Shader& shader, glm::vec3 camPos, glm::vec3 camFront, glm::vec3 camUp)
		{
			shader.use();
			transMatrix(shader,camPos, camFront, camUp);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		} 
	private:
		float vertex[12] = {
				 0.5f,  0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				-0.5f, -0.5f, 0.0f,
				-0.5f,  0.5f, 0.0f
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

			modelMatrix = glm::rotate(modelMatrix,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -1.0f, 1.0f));
			viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
			projMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

			shader.setUniformMatrix(*"model", modelMatrix);
			shader.setUniformMatrix(*"view", viewMatrix);
			shader.setUniformMatrix(*"proj", projMatrix);


		}
};
