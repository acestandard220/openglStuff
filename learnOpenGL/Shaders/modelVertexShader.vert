#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out vec3 Normal;
out vec3 FragPos;
out vec4 fragPosLightSpace;

out vec2 TexCoords;
uniform mat4 u_mvp;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
	
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    FragPos = vec3(model*vec4(aPos,1.0f));	
    TexCoords = aTexCoords;    
    fragPosLightSpace = lightSpaceMatrix * vec4(FragPos,1.0);
    gl_Position = u_mvp * model * vec4(aPos, 1.0);
}