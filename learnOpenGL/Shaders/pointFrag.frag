#version 330 core

in vec3 FragColour;
out vec4 Frag;

void main()
{
	Frag = vec4(FragColour,1.0f);
}
