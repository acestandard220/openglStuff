#pragma once
#include <headers/platform.h>
#include <headers/Shader.h>
#include <headers/paths.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class World 
{
public:
	World(Shader shader, glm::vec3 camPos, glm::vec3 camFront,glm::vec3 camUp)
	{
		skybox = CreateSkybox();
		skybox->transMatrix(shader, camPos, camFront, camUp);
		glDepthMask(GL_FALSE);
		skybox->Draw(shader);
	}
private:
	Skybox* skybox = (Skybox*)malloc(sizeof(Skybox));

	Skybox* CreateSkybox() const
	{
		return &Skybox(loadCubemap(faces));
	}



		std::vector<std::string> faces = {
	"P:/Projects/VS/learnOpenGL/learnOpenGL/Textures/skybox/skybox/right.jpg",
	"P:/Projects/VS/learnOpenGL/learnOpenGL/Textures/skybox/skybox/left.jpg",
	"P:/Projects/VS/learnOpenGL/learnOpenGL/Textures/skybox/skybox/top.jpg",
	"P:/Projects/VS/learnOpenGL/learnOpenGL/Textures/skybox/skybox/bottom.jpg",
	"P:/Projects/VS/learnOpenGL/learnOpenGL/Textures/skybox/skybox/front.jpg",
	"P:/Projects/VS/learnOpenGL/learnOpenGL/Textures/skybox/skybox/back.jpg"
		};
		//NOTE this as you made this mistake and almost reconsidered your programming journey :)
		// const member functions cannot call non const member functions or modify any members of the class this is the reason why loadCubemap is marked as const so that Create Skybox which for some reason is const can call it
		//also note that a const object cannot call a non const member function 
		// So simply put const fucks with only const people or else the compiler will fuck you up. :)
		unsigned int loadCubemap(std::vector<std::string>Faces) const
		{
			unsigned int textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

			int width, height, nrChannels;
			for (unsigned int i = 0; i < Faces.size(); i++)
			{
				unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
				if (data)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
					);
					stbi_image_free(data);
				}
				else
				{
					std::cout << "Cubemap texture failed to load at path: " << Faces[i] << std::endl;
					stbi_image_free(data);
				}
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			return textureID;
		}


};