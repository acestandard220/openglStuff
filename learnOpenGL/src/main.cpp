#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <headers/Shader.h>
#include <headers/cubeRender.h>
//#include <headers/skybox.h>
#include <headers/platform.h>
#include <stb_image.h>
#include <headers/model.h>
#include <headers/paths.h>
#include <headers/framebufferobject.h>
#include <vector>
//#include "headers/transfoformations.h"
#include <imGUI/imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <string>
//#include "headers/pointRenderer.h"
int SCRN_HEIGHT = 720;
int SCRN_WIDTH = 1280;

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

float lastX = SCRN_WIDTH / 2.0f;
float lastY = SCRN_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float cameraSpeed = 0.5f;

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow*, int, int, int);
void joystick_callback(int jid, int _event);

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

const float* axis;
glm::vec3 sunPos = glm::vec3(50.0f, 400.0f, 20.0f);
glm::vec3 lightPos = glm::vec3(20.0f, 200.0f, 20.0f);
glm::vec3 sunColor = glm::vec3(0.5f, 0.5, 0.2f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);


glm::vec3 LIGHTPOSITIONS[3] = {
	glm::vec3(0.0f, 250.0f, 0.0f),
	glm::vec3(500.0f,250.0f,0.0f),
	glm::vec3(-500.0f,250.0f,0.0f)
};

glm::vec3 LIGHTCOLOR[3] = {
glm::vec3(1.0f, 0.0f, 0.0f),
glm::vec3(0.0f,1.0f,0.0f),
glm::vec3(0.0f,0.0f,1.0f)
};
glm::vec3 scalek = glm::vec3(1.0, 1.0, 1.0);

bool mod_window = true;
bool isOn = false;
float on;

struct ATTENTUATION_PROPS {
	float constant;
	float linear ;
	float quadratic;
};
void Render(Shader modelShader, Model& model, ATTENTUATION_PROPS props);

void RenderBulb(Shader& modelShader, Model& model, ATTENTUATION_PROPS props);


int main()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GL_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(SCRN_WIDTH, SCRN_HEIGHT, "LEARNOPENGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewInit();





	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	int count;
	

	ImGui_ImplGlfwGL3_Init(window, true);

	Shader cubeShader(CUBE_VERTEX_SHADER, CUBE_FRAGMENT_SHADER);
	//Shader skyboxShader(SKYBOX_VERTEX_SHADER, SKYBOX_FRAGMENT_SHADER);
	Shader platformShader(PLATFORM_VERTEX_SHADER, PLATFORM_FRAGMENT_SHADER);
	//Shader scaledShader(SCALED_CUBE_VERTEX_SHADER, SCALED_CUBE_FRAGMENT_SHADER);
	Shader modelShader(MODEL_VERTEX_SHADER, MODEL_FRAGMENT_SHADER);
	Shader quadShader(QUAD_VERTEX_SHADER, QUAD_FRAGMENT_SHADER);
	Shader depthShader(DEPTH_VERTEX_SHADER, DEPTH_FRAGMENT_SHADER);
	//Shader pointShader(POINT_VERTEX_SHADER,POINT_FRAGMENT_SHADER,POINT_GEOMETRY_SHADER);

	//Skybox skybox(loadCubemap(faces));
	Framebuffer FBO(SCRN_WIDTH,SCRN_HEIGHT);
	FBO.ColorAttach(GL_RGB, GL_RGB, GL_UNSIGNED_INT_24_8);
	FBO.DepthAttach();
	FBO.Check();
	FBO.Bind();
	
	
	Model Sponza("C:/Users/User/Desktop/Sponza-master/Sponza-master/sponza.obj");
	Cube  Light;
	Model Bulb("C:/Users/User/Downloads/Bulb.obj");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_CLAMP); 

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetJoystickCallback(joystick_callback);
	processInput(window);

	
	bool show_demo_window = true;
	bool show_another_window = true;
	
	unsigned int framebufer;
	
	
	
	quadShader.use();
	quadShader.setInt("screenTexture", 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glm::vec3 lightPos(-2.0f, 2.0f, -1.0f);


	ATTENTUATION_PROPS attenprops;
	attenprops.constant = 1.0f;
	attenprops.linear = 0.0001f;
	attenprops.quadratic = 0.00001f;

	int jid = 0;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		ImGui_ImplGlfwGL3_NewFrame();
	
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glViewport(0, 0, SCRN_WIDTH, SCRN_HEIGHT);
		
		if (show_demo_window)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		{
			static float fl = 0.0f;
			static int counter = 0;
			ImGui::Text("Editor");
			ImGui::InputFloat3("Directional Light Colour", &sunColor[0]);
			ImGui::InputFloat3("Directional light Position",&sunPos[0]);
			ImGui::InputFloat3("Bulb Scale", &scalek[0]);
			ImGui::Checkbox("Demo Window", &show_demo_window);

			if (ImGui::Button("Button"))
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
	
		{
			ImGui::Begin("FLUX CUX DeCartes T-WORM",&mod_window);
			ImGui::Text("This is a modification window");
			ImGui::Text("These are some stats: %d.f", ImGui::GetFrameCount());
			ImGui::Text(" This is the clipboard text: %s",ImGui::GetClipboardText());
			ImGui::SliderFloat("Camera Speed ", &cameraSpeed, 0.5f, 10.0f);
			
			ImGui::Checkbox("Directional Light", &isOn);
			ImGui::End();
		}

		Render(modelShader, Sponza,attenprops);
		RenderBulb(modelShader, Bulb, attenprops);

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

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		//WriteToPPM(window);
		glfwPollEvents();
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

//Renders
void Render(Shader modelShader,Model& model,ATTENTUATION_PROPS props)
{
	glm::mat4 modelMatrix(1.0f);
	glm::mat4 viewMatrix(1.0f);
	glm::mat4 projMatrix(1.0f);
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -2.0f, -30.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
	//modelMatrix = glm::rotate(modelMatrix, glm::radians((float)glfwGetTime()*8), glm::vec3(1.0, 1.0, 1.0));
	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	projMatrix = glm::perspective(glm::radians(45.0f), (float)SCRN_WIDTH / SCRN_HEIGHT + 0.0f, 0.1f, 3000.0f);
	glm::mat4 u_mvp = projMatrix * viewMatrix;

	modelShader.use();
	modelMatrix = glm::mat4(1.0);
	modelShader.setUniformMatrix("model", modelMatrix);
	modelShader.setUniformMatrix("u_mvp", u_mvp);
	on = isOn ? 1.0f : 0.0f;
	modelShader.setInt("on", on);
	modelShader.setVec3(*"viewPos", cameraPos);

	float cutoff = glm::cos(glm::radians(12.5f));
	float outer_cutoff = glm::cos(glm::radians(17.5f));

	int viewPos_loc1 = modelShader.getUniformLocation("viewPos");
	int spotDir_loc1 = modelShader.getUniformLocation("spotDir");
	int cutoff_loc1 = modelShader.getUniformLocation("cutoff");
	int o_cutoff_loc1 = modelShader.getUniformLocation("outer_cutoff");
	int lightPos_loc1 = modelShader.getUniformLocation("lightPos");
	int lightDir_loc1 = modelShader.getUniformLocation("d_light.lightDirection");
	int lightColor_loc1 = modelShader.getUniformLocation("d_light.lightColor");
	int lightConst_loc1 = modelShader.getUniformLocation("p_light[1].constant");
	int lightLinear_loc1 = modelShader.getUniformLocation("p_light[1].linear");
	int lightQuadratic_loc1 = modelShader.getUniformLocation("p_light[1].quadratic");
	int p_lightColor_loc1 = modelShader.getUniformLocation("p_light[1].lightColor");
	int p_lightPosition_loc1 = modelShader.getUniformLocation("p_light[1].lightPosition");
	int lightConst_loc1_2 = modelShader.getUniformLocation("p_light[2].constant");
	int lightLinear_loc1_2 = modelShader.getUniformLocation("p_light[2].linear");
	int lightQuadratic_loc1_2 = modelShader.getUniformLocation("p_light[2].quadratic");
	int p_lightColor_loc1_2 = modelShader.getUniformLocation("p_light[2].lightColor");
	int p_lightPosition_loc1_2 = modelShader.getUniformLocation("p_light[2].lightPosition");
	int lightConst_loc1_0 = modelShader.getUniformLocation("p_light[0].constant");
	int lightLinear_loc1_0 = modelShader.getUniformLocation("p_light[0].linear");
	int lightQuadratic_loc1_0 = modelShader.getUniformLocation("p_light[0].quadratic");
	int p_lightColor_loc1_0 = modelShader.getUniformLocation("p_light[0].lightColor");
	int p_lightPosition_loc1_0 = modelShader.getUniformLocation("p_light[0].lightPosition");

	glUniform1f(lightConst_loc1, props.constant);
	glUniform1f(lightLinear_loc1, props.linear);
	glUniform1f(lightQuadratic_loc1, props.quadratic);
	glUniform3fv(p_lightPosition_loc1, 1, &LIGHTPOSITIONS[1][0]);
	glUniform3fv(p_lightColor_loc1, 1, &LIGHTCOLOR[1][0]);
	glUniform1f(lightConst_loc1_2, props.constant);
	glUniform1f(lightLinear_loc1_2, props.linear);
	glUniform1f(lightQuadratic_loc1_2, props.quadratic);
	glUniform3fv(p_lightPosition_loc1_2, 1, &LIGHTPOSITIONS[2][0]);
	glUniform3fv(p_lightColor_loc1_2, 1, &LIGHTCOLOR[2][0]);
	glUniform3fv(viewPos_loc1, 1, &cameraPos[0]);
	glUniform3fv(spotDir_loc1, 1, &cameraFront[0]);
	glUniform1f(cutoff_loc1, cutoff);
	glUniform1f(o_cutoff_loc1, outer_cutoff);
	glUniform3fv(lightPos_loc1, 1, &lightPos[0]);
	glUniform3fv(lightDir_loc1, 1, &sunPos[0]);
	glUniform3fv(lightColor_loc1, 1, &sunColor[0]);
	glUniform1f(lightConst_loc1_0, props.constant);
	glUniform1f(lightLinear_loc1_0, props.linear);
	glUniform1f(lightQuadratic_loc1_0, props.quadratic);
	glUniform3fv(p_lightPosition_loc1_0, 1, &LIGHTPOSITIONS[0][0]);
	glUniform3fv(p_lightColor_loc1_0, 1, &LIGHTCOLOR[0][0]);

	model.Draw(modelShader);

}

void RenderBulb(Shader& modelShader,Model& model,ATTENTUATION_PROPS props)
{
	glm::mat4 modelMatrix(1.0f);
	glm::mat4 viewMatrix(1.0f);
	glm::mat4 projMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix, sunPos);
	modelMatrix = glm::scale(modelMatrix, scalek);
	//modelMatrix = glm::rotate(modelMatrix, glm::radians((float)glfwGetTime()*8), glm::vec3(1.0, 1.0, 1.0));
	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	projMatrix = glm::perspective(glm::radians(45.0f), (float)SCRN_WIDTH / SCRN_HEIGHT + 0.0f, 0.1f, 3000.0f);
	glm::mat4 u_mvp = projMatrix * viewMatrix;

	modelShader.use();
	modelShader.setUniformMatrix("model", modelMatrix);
	modelShader.setUniformMatrix("u_mvp", u_mvp);
	on = isOn ? 1.0f : 0.0f;
	modelShader.setInt("on", on);
	modelShader.setVec3(*"viewPos", cameraPos);

	float cutoff = glm::cos(glm::radians(12.5f));
	float outer_cutoff = glm::cos(glm::radians(17.5f));

	int viewPos_loc1 = modelShader.getUniformLocation("viewPos");
	int spotDir_loc1 = modelShader.getUniformLocation("spotDir");
	int cutoff_loc1 = modelShader.getUniformLocation("cutoff");
	int o_cutoff_loc1 = modelShader.getUniformLocation("outer_cutoff");
	int lightPos_loc1 = modelShader.getUniformLocation("lightPos");
	int lightDir_loc1 = modelShader.getUniformLocation("d_light.lightDirection");
	int lightColor_loc1 = modelShader.getUniformLocation("d_light.lightolor");
	int lightConst_loc1 = modelShader.getUniformLocation("p_light[1].constant");
	int lightLinear_loc1 = modelShader.getUniformLocation("p_light[1].linear");
	int lightQuadratic_loc1 = modelShader.getUniformLocation("p_light[1].quadratic");
	int p_lightColor_loc1 = modelShader.getUniformLocation("p_light[1].lightColor");
	int p_lightPosition_loc1 = modelShader.getUniformLocation("p_light[1].lightPosition");
	int lightConst_loc1_2 = modelShader.getUniformLocation("p_light[2].constant");
	int lightLinear_loc1_2 = modelShader.getUniformLocation("p_light[2].linear");
	int lightQuadratic_loc1_2 = modelShader.getUniformLocation("p_light[2].quadratic");
	int p_lightColor_loc1_2 = modelShader.getUniformLocation("p_light[2].lightColor");
	int p_lightPosition_loc1_2 = modelShader.getUniformLocation("p_light[2].lightPosition");
	int lightConst_loc1_0 = modelShader.getUniformLocation("p_light[0].constant");
	int lightLinear_loc1_0 = modelShader.getUniformLocation("p_light[0].linear");
	int lightQuadratic_loc1_0 = modelShader.getUniformLocation("p_light[0].quadratic");
	int p_lightColor_loc1_0 = modelShader.getUniformLocation("p_light[0].lightColor");
	int p_lightPosition_loc1_0 = modelShader.getUniformLocation("p_light[0].lightPosition");

	glUniform1f(lightConst_loc1, props.constant);
	glUniform1f(lightLinear_loc1, props.linear);
	glUniform1f(lightQuadratic_loc1, props.quadratic);
	glUniform3fv(p_lightPosition_loc1, 1, &LIGHTPOSITIONS[1][0]);
	glUniform3fv(p_lightColor_loc1, 1, &LIGHTCOLOR[1][0]);
	glUniform1f(lightConst_loc1_2, props.constant);
	glUniform1f(lightLinear_loc1_2, props.linear);
	glUniform1f(lightQuadratic_loc1_2, props.quadratic);
	glUniform3fv(p_lightPosition_loc1_2, 1, &LIGHTPOSITIONS[2][0]);
	glUniform3fv(p_lightColor_loc1_2, 1, &LIGHTCOLOR[2][0]);
	glUniform3fv(viewPos_loc1, 1, &cameraPos[0]);
	glUniform3fv(spotDir_loc1, 1, &cameraFront[0]);
	glUniform1f(cutoff_loc1, cutoff);
	glUniform1f(o_cutoff_loc1, outer_cutoff);
	glUniform3fv(lightPos_loc1, 1, &lightPos[0]);
	glUniform3fv(lightDir_loc1, 1, &sunPos[0]);
	glUniform3fv(lightColor_loc1, 1, &sunColor[0]);
	glUniform1f(lightConst_loc1_0, props.constant);
	glUniform1f(lightLinear_loc1_0, props.linear);
	glUniform1f(lightQuadratic_loc1_0, props.quadratic);
	glUniform3fv(p_lightPosition_loc1_0, 1, &LIGHTPOSITIONS[0][0]);
	glUniform3fv(p_lightColor_loc1_0, 1, &LIGHTCOLOR[0][0]);

	model.Draw(modelShader);
}
//Callbacks
void processInput(GLFWwindow* window)
{	
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
	if(!ImGui::GetIO().WantCaptureMouse || !ImGui::GetIO().WantCaptureKeyboard)
	{

		if (state == GLFW_PRESS)
		{
			sensitivity = 0.1f;
		}
		if (stated) { sensitivity = 0.0f; }
	}
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

void joystick_callback(int jid, int _event)
{
	if (_event == GLFW_CONNECTED)
		std::cout << "The joystick has been connected\n";
	if (_event == GLFW_DISCONNECTED)
		std::cout << "The joystick has been disconnected\n";

	int buttonCount;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
}