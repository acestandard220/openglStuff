#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	Shader(){}
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		std::ifstream vShade;
		std::ifstream fShade;

		vShade.open(vertexPath);
		fShade.open(fragmentPath);

		std::stringstream vStream, fStream;
		vStream << vShade.rdbuf();
		fStream << fShade.rdbuf();

		vShade.close();
		fShade.close();
		vertexCode = vStream.str();
		fragmentCode = fStream.str();


		const char* vertexShader = vertexCode.c_str();
		const char* fragmentShader = fragmentCode.c_str();

		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShader, NULL);
		glCompileShader(vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShader, NULL);
		glCompileShader(fragment);
		checkShaderError(vertex, "VERTEX");
		checkShaderError(fragment, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkShaderError(ID, "PROGRAM");
	}
	Shader(const char* vertexPath, const char* fragmentPath,const char* geometryPath)
	{
		std::ifstream vShade;
		std::ifstream fShade;
		std::ifstream gShade;

		vShade.open(vertexPath);
		fShade.open(fragmentPath);
		gShade.open(geometryPath);

		std::stringstream vStream, fStream, gStream;
		vStream << vShade.rdbuf();
		fStream << fShade.rdbuf();
		gStream << gShade.rdbuf();

		vShade.close();
		fShade.close();
		gShade.close();
		vertexCode = vStream.str();
		fragmentCode = fStream.str();
		geometryCode = gStream.str();



		const char* vertexShader = vertexCode.c_str();
		const char* fragmentShader = fragmentCode.c_str();
		const char* geometryShader = geometryCode.c_str();

		unsigned int vertex, fragment, geometry;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShader, NULL);
		glCompileShader(vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShader, NULL);
		glCompileShader(fragment);

		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry,1,&geometryShader,NULL);
		glCompileShader(geometry);

		
		checkShaderError(vertex, "VERTEX");
		checkShaderError(fragment, "FRAGMENT");
		checkShaderError(geometry, "GEOMETRY");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glAttachShader(ID, geometry);
		glLinkProgram(ID);
		checkShaderError(ID, "PROGRAM");
	}
	void use() {
		glUseProgram(ID);
	}

	int getUniformLocation(const char* var)
	{
		return glGetUniformLocation(this->ID, var);
	}

	void setUniformMatrix(const char& var, const glm::mat4 value)
	{
		glUniformMatrix4fv(getUniformLocation(&var), 1, GL_FALSE, glm::value_ptr(value));
	}
	void setVec3(const char var, const glm::vec3 value)
	{
		glUniform3f(getUniformLocation(&var), value.x, value.y, value.z);
	}
	void setFloat(const char var, int* value)
	{
		glUniform1iv(getUniformLocation(&var), 1, value);
	}
	void setInt(const char var, int value)
	{
		glUniform1i(getUniformLocation(&var), value);
	}

private:
	void checkShaderError(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};