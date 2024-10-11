#version 330 core
layout(location = 0) in vec2 aPos;
layout (location =1) in vec2 aTex;

out vec2 textCord;

uniform mat4 u_mvp;

void main()
{
	textCord = aTex;
	gl_Position =  vec4(aPos.x,aPos.y,0.0,1.0f);

}