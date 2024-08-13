#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <headers/Shader.h>
#include <headers/cubeRender.h>
#include <headers/platform.h>
#include <stb_image.h>
#include <headers/model.h>
#include <headers/paths.h>
#include <headers/ripOFF.h>
#include <imGUI/imgui.h>
#include <imGUI/imgui_impl_glfw_gl3.h>


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

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewInit();

	Shader cubeShader(CUBE_VERTEX_SHADER, CUBE_FRAGMENT_SHADER);

	Shader platformShader(PLATFORM_VERTEX_SHADER, PLATFORM_FRAGMENT_SHADER);
	Shader scaledShader(SCALED_CUBE_VERTEX_SHADER, SCALED_CUBE_FRAGMENT_SHADER);
	Shader modelShader(MODEL_VERTEX_SHADER, MODEL_FRAGMENT_SHADER);

	Cube cube(WOOD_PEELING);
	Platform ground(RUST_COARSE);
	

	glEnable(GL_DEPTH_TEST);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	processInput(window);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	bool rot = false;
	glm::vec3 translation(1.0f, 1.0f, 1.0f);

	stbi_set_flip_vertically_on_load(true);

	while (!glfwWindowShouldClose(window))
	{

		

		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		processInput(window);




		glm::mat4 modelMatrix(1.0f);

		//modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f * (float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, translation);
		glm::mat4 viewMatrix(1.0f);
		glm::mat4 projMatrix(1.0f);

		viewMatrix = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
		projMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);


		///
		//ground.transMatrix(platformShader, modelMatrix, cameraPos, cameraFront, cameraUp);
		ground.draw(platformShader);

		cube.transMatrix(cubeShader, modelMatrix, cameraPos, cameraFront, cameraUp);
		cube.Draw(cubeShader);
		
		
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