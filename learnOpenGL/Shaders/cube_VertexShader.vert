#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location =2) in vec2 aTex;



uniform mat4 u_mvp;
uniform mat4 model;



void main()
{
	gl_Position =  u_mvp * model * vec4(aPos,1.0f);
}