#version 330 core

out vec4 Frag;

in vec3 textureCoord;

uniform samplerCube text;

void main()
{
	Frag =texture(text,textureCoord);
}