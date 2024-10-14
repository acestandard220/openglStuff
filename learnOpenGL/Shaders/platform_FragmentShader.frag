#version 330 core

in vec2 TexCoord;

uniform sampler2D Texture;
out vec4 FragColor;
 
void main()
{
	vec4 texColor = texture(Texture, TexCoord);
    //FragColor = vec4(0.4,0.8,0.9,1.0);
	FragColor = texColor;

}