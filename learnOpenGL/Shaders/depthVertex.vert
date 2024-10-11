#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrx;

void main()
{
	gl_Position = lightSpaceMatrx * vec4(aPos,1.0);

}
