#version 330 core

in the{
    vec3 Normals;
	vec3 Positions;
};


out vec4 Frag;

uniform samplerCube screenTexture;
uniform vec3 cameraPos;

void main()
{
	vec3 I = normalize(Positions - cameraPos);
	vec3 R = reflect(I, normalize(Normals));


	Frag = vec4(texture(screenTexture, -R).rgb, 1.0);
	
	  


	
}