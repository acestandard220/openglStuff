#version 330 core

out vec4 Frag;

in vec2 textureCoord;

uniform sampler2D text;

void main()
{
	Frag =texture(text,textureCoord);
}