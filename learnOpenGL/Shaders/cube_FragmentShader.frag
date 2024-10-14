#version 330 core
out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform sampler2D normalTexture;
uniform vec3 viewPos;
//uniform vec3 lightPos;

in VS_OUT{
	vec2 textCoord;
	vec3 FragPos;
	vec3 Normal;
	vec4 FragLighSpacePos;
}fs_in;

float Shadow(vec4 Fraglight)
{
	vec3 projCoord = Fraglight.xyz/Fraglight.w;
	projCoord = projCoord * 0.5 + 0.5; 

	float closestDpeth = texture(shadowMap,projCoord.xy).r;
	float currentDepth = projCoord.z;
	float shadow = (currentDepth > closestDpeth) ? 1.0:0.0;

	return shadow;
}


void main()
{
	vec3 light = vec3(1.0,1.0,1.0);
	vec3 lightPos =vec3(1.2f, 1.0f, 2.0f);
	float ambientStrength =0.1;
	vec3 ambient = ambientStrength * light;

	//vec3 norm  = normalize(fs_in.Normal);
	vec3 norm = texture(normalTexture,fs_in.textCoord).rgb;
	norm = normalize(norm*2.0-1.0);

	vec3 lightDir  = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = diff *light;

	float specularStrength = 0.5;
	vec3 reflectionDir = reflect(-lightDir,norm);
	vec3 viewDir = normalize(viewPos-fs_in.FragPos);
	float spec = pow(max(dot(reflectionDir,viewDir),0),128);

	vec3 specular = spec * specularStrength * light;

	float shad = Shadow(fs_in.FragLighSpacePos);

	vec3 colour = texture(diffuseTexture, fs_in.textCoord).rgb;
	vec3 result = (ambient +(1.0-shad)* diffuse +specular)* colour;
	FragColor = vec4(result,1.0);

	//Texture with normal as diffuse
	//FragColor = texture(normalTexture,fs_in.textCoord);
	
}
