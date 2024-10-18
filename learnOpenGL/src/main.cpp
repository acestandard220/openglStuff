#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <headers/Shader.h>
#include <headers/cubeRender.h>
//#include <headers/skybox.h>
#include <headers/platform.h>
#include <stb_image.h>
#include <headers/model.h>
#include <headers/paths.h>
#include <vector>
//#include "headers/transfoformations.h"
#include <string>
//#include "headers/pointRenderer.h"
int SCRN_HEIGHT = 800;
int SCRN_WIDTH = 800;

/// <TODO>
/// 
/// <AS SOON AS POSSIBLE>
/// Allow draw calls without mandatory call of transMatrix function
/// <//>
///  Add Textures for the Ground plane. [DONE]
///  Update cube and platform renderers to accept model matrix from main.cpp [DONE]
///  Do MVP matrix multiplication operation on CPU side
///  Switch all shader inputs to u_mvp
///  Add the ability to select which gameObject you are working on.
///  Addition of the remaining transforms(rotations,rotationSlow)
///  Addition of along a specific axis transformation support 
/// Addition of a struct for matrixes that can be passed to the form renderers
/// </TODO>


bool paneFlag = false;

float sensitivity = 0.0f;

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
void mouse_button_callback(GLFWwindow*, int, int, int);

std::vector<std::string> faces = {
	"P:/Projects/VS/learnOpenGL/learnOpenGL/Textures/skybox/skybox/right.jpg",
	"P:/Projects/VS/learnOpenGL/learnOpenGL/Textures/skybox/skybox/left.jpg",
	"P:/Projects/VS/learnOpenGL/learnOpenGL/Textures/skybox/skybox/top.jpg",
	"P:/Projects/VS/learnOpenGL/learnOpenGL/Textures/skybox/skybox/bottom.jpg",
	"P:/Projects/VS/learnOpenGL/learnOpenGL/Textures/skybox/skybox/front.jpg",
	"P:/Projects/VS/learnOpenGL/learnOpenGL/Textures/skybox/skybox/back.jpg"
};

unsigned int loadCubemap(std::vector<std::string>faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
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
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
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


int main()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GL_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(SCRN_WIDTH, SCRN_HEIGHT, "amish", NULL, NULL);
	glfwMakeContextCurrent(window);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewInit();

	Shader cubeShader(CUBE_VERTEX_SHADER, CUBE_FRAGMENT_SHADER);
	//Shader skyboxShader(SKYBOX_VERTEX_SHADER, SKYBOX_FRAGMENT_SHADER);
	Shader platformShader(PLATFORM_VERTEX_SHADER, PLATFORM_FRAGMENT_SHADER);
	//Shader scaledShader(SCALED_CUBE_VERTEX_SHADER, SCALED_CUBE_FRAGMENT_SHADER);
	Shader modelShader(MODEL_VERTEX_SHADER, MODEL_FRAGMENT_SHADER);
	Shader quadShader(QUAD_VERTEX_SHADER, QUAD_FRAGMENT_SHADER);
	Shader depthShader(DEPTH_VERTEX_SHADER, DEPTH_FRAGMENT_SHADER);
	//Shader pointShader(POINT_VERTEX_SHADER,POINT_FRAGMENT_SHADER,POINT_GEOMETRY_SHADER);

	//Skybox skybox(loadCubemap(faces));


	
	Model Cux("C:/Users/User/Downloads/halloween_pumpkin_tim_burton_style/hallowen_pum.obj");
	Model Sponza("C:/Users/User/Desktop/Sponza-master/Sponza-master/sponza.obj");
	Cube  Light;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	processInput(window);

	

	

	
	quadShader.use();
	quadShader.setInt("screenTexture", 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glm::vec3 lightPos(-2.0f, 2.0f, -1.0f);
	glm::vec3 lightPos = glm::vec3(50, 200, 20);
	

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 modelMatrix(1.0f);
		glm::mat4 viewMatrix(1.0f);
		glm::mat4 projMatrix(1.0f);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -2.0f, -30.0f));
		//modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0));
		//modelMatrix = glm::rotate(modelMatrix, glm::radians((float)glfwGetTime()*8), glm::vec3(1.0, 1.0, 1.0));
		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projMatrix = glm::perspective(glm::radians(45.0f), (float)SCRN_WIDTH / SCRN_HEIGHT + 0.0f, 0.1f ,5000.0f);
		glm::mat4 u_mvp = projMatrix * viewMatrix ;

		glViewport(0, 0, SCRN_WIDTH, SCRN_HEIGHT);


		cubeShader.use();
		modelMatrix = glm::translate(modelMatrix, lightPos);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(3.0));
		cubeShader.setUniformMatrix("model", modelMatrix);
		cubeShader.setUniformMatrix("u_mvp", u_mvp);
		cubeShader.setVec3(*"viewPos", cameraPos);
		Light.SetMVP(cubeShader,modelMatrix, cameraPos, cameraFront, cameraUp);

		Light.Draw(cubeShader);

		modelShader.use();
		modelMatrix = glm::mat4(1.0);
		modelShader.setUniformMatrix("model", modelMatrix);
		modelShader.setUniformMatrix("u_mvp", u_mvp);
		modelShader.setVec3(*"viewPos", cameraPos);
		modelShader.setVec3(*"camPos", cameraPos);
		Sponza.Draw(modelShader);

		modelShader.use();
		
		modelMatrix = glm::translate(modelMatrix, glm::vec3(20,300,20));
		modelShader.setVec3(*"lightPos", lightPos);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians((float)glfwGetTime()*8), glm::vec3(1.0, 1.0, 1.0));
		modelShader.setUniformMatrix("model", modelMatrix);
		modelShader.setUniformMatrix("u_mvp", u_mvp);
		modelShader.setVec3(*"camPos", cameraPos);
		modelShader.setVec3(*"viewPos", cameraPos);
		Cux.Draw(modelShader);
		
		






		/*
		quadShader.use(); 
		//this is the correct way of using textures
		//the code below binds the sampler2D variable inn the shader to the texture unit 0
		quadShader.setInt("screenTexture", 0);
		
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);	
		
		//*/

		glfwSwapBuffers(window);
		//WriteToPPM(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	const float cameraSpeed = 0.5f;
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
	SCRN_HEIGHT = height;
	SCRN_WIDTH = width;

	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int stated = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	if (state == GLFW_PRESS)
	{
		sensitivity = 0.1f;
	}
	if(stated) { sensitivity = 0.0f; }
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
