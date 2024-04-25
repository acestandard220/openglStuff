#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <headers/Shader.h>
#include <headers/cubeRender.h>


int main()
{
	if (!glfwInit())
		return -1;

	GLFWwindow* window = glfwCreateWindow(800, 600, "amish", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();

	Shader cubeShader("P:\\Projects\\VS\\learnOpenGL\\learnOpenGL\\Shaders\\cube_VertexShader.shader", "P:\\Projects\\VS\\learnOpenGL\\learnOpenGL\\Shaders\\cube_FragmentShader.shader");

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

	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2, 0.5, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);


		cube.Enable(ROTATE);
		cube.Draw(cubeShader);
			
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}



