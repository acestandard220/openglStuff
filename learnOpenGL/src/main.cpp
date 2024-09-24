#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <headers/Shader.h>
#include <headers/cubeRender.h>
#include <headers/skybox.h>
#include <headers/platform.h>
#include <stb_image.h>
#include <headers/model.h>
#include <headers/paths.h>
#include <headers/ripOFF.h>
#include <imGUI/imgui.h>
#include <imGUI/imgui_impl_glfw_gl3.h>
#include <vector>
#include "headers/transfoformations.h"
#include <string>
#include "headers/pointRenderer.h"

#define SCRN_HEIGHT  800
#define SCRN_WIDTH  800

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

	GLFWwindow* window = glfwCreateWindow(SCRN_WIDTH, SCRN_HEIGHT, "amish", NULL, NULL);
	glfwMakeContextCurrent(window);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewInit();

	Shader cubeShader(CUBE_VERTEX_SHADER, CUBE_FRAGMENT_SHADER);
	Shader skyboxShader(SKYBOX_VERTEX_SHADER, SKYBOX_FRAGMENT_SHADER);
	Shader platformShader(PLATFORM_VERTEX_SHADER, PLATFORM_FRAGMENT_SHADER);
	Shader scaledShader(SCALED_CUBE_VERTEX_SHADER, SCALED_CUBE_FRAGMENT_SHADER);
	Shader modelShader(MODEL_VERTEX_SHADER, MODEL_FRAGMENT_SHADER);
	Shader pointShader(POINT_VERTEX_SHADER,POINT_FRAGMENT_SHADER,POINT_GEOMETRY_SHADER);

	Skybox skybox(loadCubemap(faces));
	Cube cube(loadCubemap(faces));
	Cube c(WORN_PLANKS);
	Cube outline;

	Point p;
	
	
	Platform g(WORN_PLANKS);

	glEnable(GL_DEPTH_TEST);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	processInput(window);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	bool rot = false;
	glm::vec3 translation(1.2f, 1.20f, 1.20f);

	glEnable(GL_STENCIL_TEST);
	//glStencilMask(0xFF);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	

	while (!glfwWindowShouldClose(window))
	{


		ImGui_ImplGlfwGL3_NewFrame();

		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		processInput(window);



		{
			static float fl = 0.0f;
			static int counter = 0;
			ImGui::Text("Editor");
			ImGui::SliderFloat("float", &fl, 0.0f, 1.0f);
			ImGui::ColorEdit3("clear color", (float*)&clear_color);

			ImGui::Checkbox("Demo Window", &show_demo_window);
			ImGui::Checkbox("Another Window", &show_another_window);
			ImGui::Checkbox("Rotation", &rot);
			ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 5.0f);

			


			if (ImGui::Button("Button"))
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}


		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			ImGui::Checkbox("Checket", &show_another_window);
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
		if (show_demo_window)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		

		glm::mat4 modelMatrix(1.0f);
		glm::mat4 viewMatrix(1.0f);
		glm::mat4 projMatrix(1.0f);

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, translation);
	    viewMatrix = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront,cameraUp)));
		projMatrix = glm::perspective(glm::radians(45.0f), SCRN_WIDTH /SCRN_HEIGHT + 0.0f, 0.1f, 1000.0f);

		
		/*
		skybox.transMatrix(skyboxShader, cameraPos, cameraFront, cameraUp);
		glDepthMask(GL_FALSE);
		//skybox.Draw(skyboxShader);

		glDepthMask(GL_TRUE);
		cube.SetMVP(cubeShader,modelMatrix, cameraPos, cameraFront, cameraUp);
		//cube.Draw(cubeShader);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(10.0, 1.0, 0.0f));
		//c.UseDefaultMVP(scaledShader, cameraPos, cameraFront, cameraUp);
		c.SetMVP(scaledShader, model, cameraPos, cameraFront, cameraUp);
		//c.Draw(scaledShader);
		*/

		p.Draw(pointShader);
	
		

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
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