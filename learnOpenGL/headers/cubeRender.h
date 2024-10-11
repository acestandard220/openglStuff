#pragma once
#include <iostream>
#include <GL/glew.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <cassert>
//***********************************************             ATTENTION                *******************************************
// call one of the transMAtrix functions before using draw.


enum DEFAULTS {
	U_MVP
};

bool flag = false;

class Cube
{
public:

	bool rotate = false;
	float rotValue = 0;

	unsigned int texture;
	unsigned int vArray;
	Cube(const char* texture_Path)
	{
		unsigned int vBuffer;
		
		glGenVertexArrays(1, &vArray);
		glBindVertexArray(vArray);

		glGenBuffers(1, &vBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		//CHANGED SOMETHING HERE N****
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(texture_Path, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

		glBindTexture(GL_TEXTURE_2D, 0);
		

	}
	Cube(unsigned int textureID) : texture(textureID)
	{
		unsigned int vBuffer;

		glGenVertexArrays(1, &vArray);
		glBindVertexArray(vArray);

		glGenBuffers(1, &vBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		//CHANGED SOMETHING HERE N****
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);				

	}
	Cube()
	{
		unsigned int vBuffer;

		glGenVertexArrays(1, &vArray);
		glBindVertexArray(vArray);

		glGenBuffers(1, &vBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

	}


	void UseDefaultMVP(Shader& shader, glm::vec3& camPos, glm::vec3& camFront, glm::vec3& camUp)
	{
		glm::mat4 modelMatrix = 1.0f;
		glm::mat4 projMatrix = 1.0f;
		glm::mat4 viewMatrix(1.0f);

		//modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f * (float)glfwGetTime() * rotValue), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -20.0f));
		viewMatrix = glm::lookAt(camPos,camPos+camFront,camUp);
		projMatrix = glm::perspective(glm::radians(45.0f), 500 / 300.0f, 0.1f, 1000.0f);

		shader.use();
		glm::mat4 u_mvp = projMatrix * viewMatrix * modelMatrix;

		shader.setUniformMatrix("u_mvp", u_mvp);
	}

	void SetMVP(Shader& shader, glm::mat4 modelMat, glm::vec3& camPos, glm::vec3& camFront, glm::vec3& camUp)
	{
		glm::mat4 modelMatrix = modelMat;
		glm::mat4 projMatrix = 1.0f;
		glm::mat4 viewMatrix(1.0f);


		viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
		projMatrix = glm::perspective(glm::radians(45.0f), 500 / 300.0f, 0.1f, 1000.0f);

		glm::mat4 u_mvp = projMatrix * viewMatrix * modelMatrix;

		shader.use();
		shader.setUniformMatrix("u_mvp", u_mvp);
		shader.setUniformMatrix("model", modelMatrix);
		shader.setVec3(*"cameraPos", camPos);	
	}
	// call one of the transMAtrix functions before using draw.

	void Draw(Shader& shader)
	{		
		shader.use();
		shader.setInt("text", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vArray);
		stbi_set_flip_vertically_on_load(true);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

private:


	float vertex[72 + 108+36] = {
	
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
};