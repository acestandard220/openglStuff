#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <headers/Shader.h>
#include <headers/cubeRender.h>
#include <headers/platform.h>
#include <stb_image.h>
#include <headers/model.h>
#include <headers/paths.h>
#include <headers/ripOFF.h>


/// <TODO>
///  Add Textures for the Ground plane. [DONE]
///  Update cube and platform renderers to accept model matrix from main.cpp 
/// </TODO>

float lastX = 1000 / 2.0f;
float lastY = 800 / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	
float lastFrame = 0.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;



void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main()
{
	if (!glfwInit())
		return -1;

	GLFWwindow* window = glfwCreateWindow(1000, 800, "amish", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewInit();

	//stbi_set_flip_vertically_on_load(true);

	Shader cubeShader("P:\\Projects\\VS\\learnOpenGL\\learnOpenGL\\Shaders\\cube_VertexShader.shader", "P:\\Projects\\VS\\learnOpenGL\\learnOpenGL\\Shaders\\cube_FragmentShader.shader");
	Shader platformShader("P:\\Projects\\VS\\learnOpenGL\\learnOpenGL\\Shaders\\platform_VertexShader.shader", "P:\\Projects\\VS\\learnOpenGL\\learnOpenGL\\Shaders\\platform_FragmentShader.shader");
	Shader scaledShader("P:\\Projects\\VS\\learnOpenGL\\learnOpenGL\\Shaders\\outline_Vertex.shader", "P:\\Projects\\VS\\learnOpenGL\\learnOpenGL\\Shaders\\outline_fragment.shader");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST); 
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); 
	glStencilMask(0xFF);

//	Cube cube(WORN_PLANKS);
	//CubeR scaledCube(glm::vec3(1.0));
	Platform quad(WORN_PLANKS);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	processInput(window);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
		
		processInput(window);

		/*
		glm::mat4 modelMatrix(1.0f);
		glm::mat4 viewMatrix(1.0f);
		glm::mat4 projMatrix(1.0);

		cmodelMatrix = glm::rotate(modelMatrix,glm::radians(45.0f), glm::vec3(1.0, 1.0f, 1.0f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0,0.0,0.0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0, 1.0, 1.0f));
		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projMatrix = glm::perspective(glm::radians(45.0f), (float)1000.0f / (float)800, 0.1f, 10000.0f);

		cubeShader.setUniformMatrix(*"model", modelMatrix);
		cubeShader.setUniformMatrix(*"view", viewMatrix);
		cubeShader.setUniformMatrix(*"proj", projMatrix);
		*/

		quad.draw(platformShader, cameraPos, cameraFront, cameraUp);


			
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	const float cameraSpeed = 0.005f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

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