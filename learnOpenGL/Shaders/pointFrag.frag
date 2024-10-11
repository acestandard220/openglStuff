#version 330 core

in vec3 fColor;
out vec4 Frag;

void main()
{
	Frag = vec4(fColor,1.0f);
}
