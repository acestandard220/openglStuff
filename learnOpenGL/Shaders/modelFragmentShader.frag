#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform vec3 spotDir;
uniform float cutoff;
uniform float outer_cutoff;
uniform vec3 lightPos;
uniform int on;


struct DirLight{
    vec3 lightDirection;
    vec3 lightColor;
};
struct PointLight{
    float constant;
    float linear;
    float quadratic;

    vec3 lightColor;
    vec3 lightPosition;
};
uniform DirLight d_light;
uniform PointLight p_light[3];

vec3 Calc_DirLight(DirLight d_light,vec3 normal,vec3 viewPos);
vec3 Calc_PointLight(PointLight p_light,vec3 FragPos,vec3 normal,vec3 viewPos);

void main()
{     

   

     vec3 lightCol = vec3(1,0.8,0.8);
     vec3 norm = normalize(Normal);
     vec3 lightDir = normalize(lightPos-FragPos);   
      
      //Attenuation
     float distance = length(lightPos-FragPos);
     vec3 result = Calc_DirLight(d_light,norm,viewPos);
  
     result += Calc_PointLight(p_light[1],FragPos,norm,viewPos);
     result += Calc_PointLight(p_light[2],FragPos,norm,viewPos);
     result += Calc_PointLight(p_light[0],FragPos,norm,viewPos);
    // FragColor = vec4(viewPos,1.0f);
    FragColor = vec4(result,1.0f);
      
    
}
vec3 Calc_DirLight(DirLight d_light,vec3 normal,vec3 viewPos){
    
    vec3 lightDir = normalize(d_light.lightDirection);

    float ambientIntensity = 0.1;
    vec3 ambient = ambientIntensity *d_light.lightColor;

    float diff = max(dot(lightDir,normal),0.0);
    vec3 diffuse = diff * d_light.lightColor;

    float specularIntensity = 0.8;
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir,normal);
    
    
    float spec = pow(max(0.0,dot(reflectDir,viewDir)),128);
    vec3 specular = specularIntensity*spec*d_light.lightColor;

    return vec3(on * (ambient + diffuse + specular) * texture(texture_diffuse1,TexCoords).rgb);

}

vec3 Calc_PointLight(PointLight p_light,vec3 FragPos,vec3 normal,vec3 viewPos)
{
    
    float p_ambientIntensity =0.1;
    vec3 p_ambient = p_ambientIntensity * p_light.lightColor;

    float distance = length(p_light.lightPosition - FragPos);
    float attenuation = 1/(p_light.constant + p_light.linear*distance + p_light.quadratic *(distance*distance));
   
    vec3 p_lightDir = normalize(p_light.lightPosition - FragPos);

    float p_diff = max(dot(p_lightDir,normal),0.0);
    vec3 p_diffuse = p_diff * p_light.lightColor * attenuation;

    float p_specularIntensity = 0.8;
    vec3 p_viewDir = normalize(viewPos-FragPos);
    vec3 p_reflectDir = reflect(-p_lightDir,normal);
    float p_spec = pow(max(dot(p_reflectDir,p_viewDir),0.0),128);
    vec3 p_specular = p_spec * p_specularIntensity*p_light.lightColor * attenuation;

    return vec3((p_ambient + p_diffuse+p_specular) * texture(texture_diffuse1,TexCoords).rgb);
}