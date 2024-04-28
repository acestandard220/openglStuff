#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <headers/Shader.h>
#include <headers/cubeRender.h>
#include <headers/paths.h>
#include <stb_image.h>
#include <headers/model.h>


float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;


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


	Model model("P:/Projects/VS/learnOpenGL/learnOpenGL/backpack/backpack.obj");


	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2, 0.5, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);


		cubeShader.use();

		glm::mat4 modelMatrix = 1.0f;
		glm::mat4 viewMatrix = 1.0f;
		glm::mat4 projMatrix = 1.0f;

		modelMatrix = glm::rotate(modelMatrix, glm::radians(35.0f * (float)glfwGetTime() ), glm::vec3(1.0f, 1.0f, 1.0f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0,1.0f, 1.0f));
		viewMatrix = glm::translate(viewMatrix,glm::vec3(0.0,0.0f,-3.0f));
		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projMatrix = glm::perspective(glm::radians(45.0f), 500 / 300.0f, 0.1f, 1000.0f);

		cubeShader.setUniformMatrix(*"model", modelMatrix);
		cubeShader.setUniformMatrix(*"view", viewMatrix);
		cubeShader.setUniformMatrix(*"proj", projMatrix);
			
		
		model.Draw(cubeShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow* window)
{
	const float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

}

