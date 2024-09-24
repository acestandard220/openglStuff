#version 330 core

in vec2 TexCoord;

uniform sampler2D tex;

out vec4 FragColor;
 
void main()
{
	vec4 texColor = texture(tex, TexCoord);

    FragColor = texColor;

}