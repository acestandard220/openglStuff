#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;


void main()
{     
     vec3 lightCol = vec3(1.0,1,1);
     vec3 norm = normalize(Normal);

     float ambientStrength =0.1;
     vec3 ambient = ambientStrength* lightCol;

     vec3 lightPos = vec3(2,0.0,2.0);
     vec3 lightDir = normalize(lightPos-FragPos);
     float diff = max(dot(lightDir,norm),0.0);
     vec3 diffuse = diff * lightCol;


     float specularIntensity = 0.7;
     vec3 viewDir = normalize(FragPos-viewPos);
     vec3 reflectDir = reflect(lightDir,norm);
     float spec = pow(max(0.0,dot(reflectDir,viewDir)),128);
     vec3 specular = specularIntensity*spec*lightCol;

     FragColor = vec4((diffuse+ambient+specular) * texture(texture_diffuse1, TexCoords).rgb,1.0f);
     
}
