#version 330 core
out vec4 FragColor;

in vec2 textCoord;

uniform sampler2D text;
void main()
{
	FragColor = texture(text, textCoord);
}
