#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum TRANSFORMATION
{
	TRANSLATE,
	ROTATION,
	SCALE
};

glm::mat4 modelMatrix()
{
	glm::mat4 modelMat(1.0f);

	return modelMat;
}

glm::mat4 modelMatrix(TRANSFORMATION transformation, glm::mat4& updateMatrix, glm::vec3 scalar = glm::vec3(1.0f))
{
	glm::mat4 modelMat(1.0f);

	if (transformation == TRANSLATE)
	{
		modelMat = glm::translate(updateMatrix,scalar);

	}
	else if (transformation == SCALE)
	{
		modelMat = glm::scale(updateMatrix,scalar);
	}

	return modelMat;
}
