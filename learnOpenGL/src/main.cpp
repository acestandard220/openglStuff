#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <headers/Shader.h>
#include <headers/cubeRender.h>
#include <headers/paths.h>
#include <stb_image.h>
#include <headers/model.h>



int main()
{
	if (!glfwInit())
		return -1;

	GLFWwindow* window = glfwCreateWindow(800, 600, "amish", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();

	Shader cubeShader(CUBE_VERTEX_SHADER,CUBE_FRAGMENT_SHADER);

	float cube_vertex[] = {
			-0.5f, -0.5f, -0.5f,    
			 0.5f, -0.5f, -0.5f,    
			 0.5f,  0.5f, -0.5f,    
			 0.5f,  0.5f, -0.5f,    
			-0.5f,  0.5f, -0.5f,    
			-0.5f, -0.5f, -0.5f,    

			-0.5f, -0.5f,  0.5f,    
			 0.5f, -0.5f,  0.5f,    
			 0.5f,  0.5f,  0.5f,    
			 0.5f,  0.5f,  0.5f,    
			-0.5f,  0.5f,  0.5f,    
			-0.5f, -0.5f,  0.5f,    

			-0.5f,  0.5f,  0.5f,   
			-0.5f,  0.5f, -0.5f,   
			-0.5f, -0.5f, -0.5f,   
			-0.5f, -0.5f, -0.5f,   
			-0.5f, -0.5f,  0.5f,   
			-0.5f,  0.5f,  0.5f,   

			 0.5f,  0.5f,  0.5f,    
			 0.5f,  0.5f, -0.5f,    
			 0.5f, -0.5f, -0.5f,    
			 0.5f, -0.5f, -0.5f,    
			 0.5f, -0.5f,  0.5f,    
			 0.5f,  0.5f,  0.5f,    

			-0.5f, -0.5f, -0.5f,    
			 0.5f, -0.5f, -0.5f,    
			 0.5f, -0.5f,  0.5f,    
			 0.5f, -0.5f,  0.5f,    
			-0.5f, -0.5f,  0.5f,    
			-0.5f, -0.5f, -0.5f,    

			-0.5f,  0.5f, -0.5f,    
			 0.5f,  0.5f, -0.5f,    
			 0.5f,  0.5f,  0.5f,    
			 0.5f,  0.5f,  0.5f,    
			-0.5f,  0.5f,  0.5f,    
			-0.5f,  0.5f, -0.5f   
	};

	Cube cube(cube_vertex);

	Model model("P:\\Archive\\3DModels\\OBJModels\\lamborghiniurus2019.obj");


	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2, 0.5, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		cubeShader.use();

		glm::mat4 modelMatrix = 1.0f;
		glm::mat4 viewMatrix = 1.0f;
		glm::mat4 projMatrix = 1.0f;

		modelMatrix = glm::rotate(modelMatrix, glm::radians(35.0f * (float)glfwGetTime() ), glm::vec3(1.0f, 1.0f, 1.0f));
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 0, -3.0f));
		projMatrix = glm::perspective(glm::radians(45.0f), 500 / 300.0f, 0.1f, 1000.0f);

		cubeShader.setUniformMatrix(*"model", modelMatrix);
		cubeShader.setUniformMatrix(*"view", viewMatrix);
		cubeShader.setUniformMatrix(*"proj", projMatrix);
			
		//model.Draw(cubeShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}



