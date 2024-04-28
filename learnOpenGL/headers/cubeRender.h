#pragma once
#include <iostream>
#include <GL/glew.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum keys {
	ROTATE = 0,
	COLOR =1
};
enum colors {
	RED =0,
	YELLOW = 2,
	GREEN = 3,
};

class Cube
{
public:

	glm::mat4 viewMatrix = 1.0f;
	bool rotate = false;
	float rotValue = 0;
	Cube(float vertex[], int count = 108)
	{
		float v[108];
		for (int i = 0; i < count; i++)
		{
			v[i] = vertex[i];
		}
	
		
		unsigned int vBuffer;
		glGenBuffers(1, &vBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

	}

	void Draw(Shader& shader)
	{
		shader.use();
		transMatrix(shader);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
	}

	void Enable(keys key)
	{
		if (key == ROTATE)
		{
			rotValue = 1;
		}
	}
	void Enable(keys key, colors color)
	{
		if (key == COLOR)
		{
			//do something
			
		}
		else {
			return;
		}
	}
	


private:
	
	void transMatrix(Shader& shader)
	{
		glm::mat4 modelMatrix = 1.0f;
		glm::mat4 projMatrix = 1.0f;

		modelMatrix = glm::rotate(modelMatrix, glm::radians(35.0f * (float)glfwGetTime() *rotValue), glm::vec3(1.0f, 1.0f, 1.0f));
		//viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 0, 1.0f));
		projMatrix = glm::perspective(glm::radians(45.0f), 500 / 300.0f, 0.1f, 1000.0f);
		
		shader.setUniformMatrix(*"model", modelMatrix);
		shader.setUniformMatrix(*"view", viewMatrix);
		shader.setUniformMatrix(*"proj", projMatrix);

	}
	


};