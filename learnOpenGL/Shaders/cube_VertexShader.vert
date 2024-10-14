#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location =2) in vec2 aTex;

out VS_OUT{
	vec2 textCoord;
	vec3 FragPos;
	vec3 Normal;
	vec4 FragLighSpacePos;
}vs_out;


uniform mat4 u_mvp;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;


void main()
{
	vs_out.textCoord = aTex;
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;  
	vs_out.FragPos = vec3(model * vec4(aPos,1.0f));
	vs_out.FragLighSpacePos = lightSpaceMatrix * vec4(vs_out.FragPos,1.0);
	gl_Position =  u_mvp * vec4(aPos,1.0f);
}