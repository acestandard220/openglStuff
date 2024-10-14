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

int pixels[50 * 50];
std::vector<float> pix(SCRN_HEIGHT* SCRN_WIDTH*4 );

void WriteToPPM(GLFWwindow* window)
{
	glReadBuffer(GL_FRONT);
	glfwGetFramebufferSize(window, &SCRN_WIDTH, &SCRN_HEIGHT);
	pix.resize(SCRN_HEIGHT * SCRN_WIDTH);
	glReadPixels(0, 0, SCRN_WIDTH, SCRN_HEIGHT, GL_RGB, GL_FLOAT, pix.data());
		
	


	std::fstream v;
	v.open("out.ppm");
	v << "P3\n";
	v << 800 <<" "<< 800 << "\n";
	v << 1;
	v << "\n";

	for (int i = 0; i < 800 * 800 ; i++)
	{
		v << pix[i]<<std::endl;

	}
	
	
	//v << stream.str();
}

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

	TextureDetails details;
	details.diffuse_path = "C:/Users/User/Downloads/castle_brick_02_red_2k/textures/castle_brick_02_red_diff_2k.jpg";
	details.normal_path = "C:/Users/User/Downloads/castle_brick_02_red_2k/textures/castle_brick_02_red_nor_gl_2k.jpg";

	Cube cube(details);
	Platform platform(COAST_SAND);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	processInput(window);

	
	float quad[] = { 
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap , 0);
	glDrawBuffer(GL_NONE);  glReadBuffer(GL_NONE);

	unsigned int renderBuferObject;
	glGenRenderbuffers(1, &renderBuferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 1024);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuferObject);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		return -11;
	}
	else {
		std::cout << "INFO::FRAMEBUFFER:: Framebuffer is complete" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	unsigned int VBO;
	unsigned int VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	quadShader.use();
	quadShader.setInt("screenTexture", 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glm::vec3 lightPos(-2.0f, 2.0f, -1.0f);
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
	

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
	
		
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		glViewport(0, 0, 1024, 1024);

		depthShader.use();
		depthShader.setUniformMatrix("lightSpaceMatrx", lightSpaceMatrix);
		cube.Draw(depthShader);
		glm::mat4 model(1.0);
		model = glm::scale(model, glm::vec3(30.0f));
		model = glm::translate(model, glm::vec3(0.0,-0.05,0.0));
		model = glm::rotate(model, glm::radians(90.0f),glm::vec3(1.0,0.0,0.0));
		depthShader.setUniformMatrix("lightSpaceMatrx", lightProjection * lightView *model);
		platform.Draw(depthShader);

		

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		
		glm::mat4 modelMatrix(1.0f);
		glm::mat4 viewMatrix(1.0f);
		glm::mat4 projMatrix(1.0f);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -2.0f, -30.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0));
		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projMatrix = glm::perspective(glm::radians(45.0f), SCRN_WIDTH / SCRN_HEIGHT + 0.0f, 0.1f, 1000.0f);
		glm::mat4 u_mvp = projMatrix * viewMatrix * modelMatrix;

		glViewport(0, 0, 1024, 1024);
		cubeShader.use();
		cubeShader.setInt("shadowMap", 2);
		cubeShader.setUniformMatrix("model", modelMatrix);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		platform.transMatrix(platformShader, cameraPos, cameraFront, cameraUp);
		platform.Draw(platformShader);

		cubeShader.use();
		cubeShader.setUniformMatrix("u_mvp", u_mvp);
		cubeShader.setVec3(*"lightPos", lightPos);
		cubeShader.setUniformMatrix("model", modelMatrix);
		cubeShader.setVec3(*"viewPos", cameraPos);
		cubeShader.setInt("shadowMap", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glBindVertexArray(VAO);
		cube.Draw(cubeShader);
		glBindVertexArray(0);

		






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
