#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;



void main()
{    
     FragColor = texture(texture_diffuse1, TexCoords);
     FragColor = texture(texture_specular1, TexCoords);
     FragColor = texture(texture_normal1,TexCoords);
     FragColor = texture(texture_height1,TexCoords);

     FragColor = vec4(.5f,0.3f,1.0f,1.0f);
}