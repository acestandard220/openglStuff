
#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


enum MatrixFlags {
	DEFAULT,
	ORHTOGRAPHIC
};
class U_MVP
{
public:
	U_MVP()
	{
		ModelMatrix = glm::mat4(1.0);
		ViewMatrix = glm::mat4(1.0);
		ProjectionMatrix = glm::mat4(1.0);
	}
	U_MVP(MatrixFlags flag,glm::vec3 cameraPosition,glm::vec3 cameraFront, glm::vec3 cameraUp)
	{
		ModelMatrix = glm::mat4(1.0);
		ViewMatrix = glm::mat4(1.0);
		ProjectionMatrix = glm::mat4(1.0);

		switch (flag)
		{
		case DEFAULT:
			ViewMatrix = glm::lookAt(cameraPosition, cameraFront, cameraUp);
			ProjectionMatrix = glm::perspective(glm::radians(45.0f), 800 / 600.0f, 0.1f, 1000.0f);
			break;
		case ORHTOGRAPHIC:
			ViewMatrix = glm::lookAt(cameraPosition, cameraFront, cameraUp);
			std::cout << "Ortho hasn't been set up yet!\n";
			break;
		default:
			break;
		}

		
	}
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	glm::mat4 u_mvp;

	glm::mat4 GetModelMatrix()
	{
		return ModelMatrix;
	}
	glm::mat4 GetViewMatrix()
	{
		return ModelMatrix;
	}
	glm::mat4 GetProjectionMatrix()
	{
		return ModelMatrix;
	}

	glm::mat4 GetU_MVP()
	{
		u_mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
		return u_mvp;
	}


	


};



#endif // !TRANSFORM_H


