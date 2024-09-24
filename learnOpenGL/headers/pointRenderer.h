#pragma once
#include <GL/glew.h>
#include "headers/Shader.h"

class Point {

public:
	unsigned int vArray;
	Point()
	{
		unsigned int vBuffer;
		
		glGenVertexArrays(1, &vArray);

		glGenBuffers(1, &vBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

		glBindVertexArray(vArray);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

	}

	void Draw(Shader shader)
	{
		shader.use();
		glBindVertexArray(vArray);
		glDrawArrays(GL_POINTS, 0, 4);

	}

private:
	float vertex[20] = {
		 -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
	     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
	     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
	    -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
	};
};

