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

	Shader cubeShader(CUBE_VERTEX_SHADER,CUBE_FRAGMENT_SHADER);

	Shader platformShader(PLATFORM_VERTEX_SHADER,PLATFORM_FRAGMENT_SHADER);
	Shader scaledShader(SCALED_CUBE_VERTEX_SHADER,SCALED_CUBE_FRAGMENT_SHADER);

	Cube cube(WORN_PLANKS);
	Platform ground(RUST_COARSE);

	glEnable(GL_DEPTH_TEST);


	Platform ground(COAST_SAND);
	
	Cube cube(WORN_PLANKS);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	processInput(window);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	 
	bool rot = false;
	glm::vec3 translation(1.0f,1.0f,1.0f);

	while (!glfwWindowShouldClose(window))
	{

		ImGui_ImplGlfwGL3_NewFrame();

		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		processInput(window);
/*
		

		{
			static float fl = 0.0f;
			static int counter = 0;
			ImGui::Text("Editor");                           
			ImGui::SliderFloat("float", &fl , 0.0f, 1.0f);
			ImGui::ColorEdit3("clear color", (float*)&clear_color); 
			ImGui::SliderFloat3("Scale", &translation.x,0.1f,10.0f);

			ImGui::Checkbox("Demo Window", &show_demo_window);     
			ImGui::Checkbox("Another Window", &show_another_window);

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

		ground.draw(platformShader, cameraPos, cameraFront, cameraUp);

		
		cube.Draw(cubeShader, cameraPos, cameraFront, cameraUp);

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