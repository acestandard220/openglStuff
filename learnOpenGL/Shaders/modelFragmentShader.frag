#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 fragPosLightSpace;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform vec3 camPos;
uniform vec3 spotDir;
uniform float cutoff;
uniform float outer_cutoff;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}


void main()
{     

     const float constant = 1.0f;
     const float linear = 0.0001;
     const float quadratic =0.000001f;
    
    vec3 lightPos = vec3(50,200,20);

     vec3 lightCol = vec3(1,0.8,0.8);
     vec3 norm = normalize(Normal);
     vec3 lightDir = normalize(lightPos-FragPos);   
      
      //Attenuation
     float distance = length(lightPos-FragPos);
     float attenuation = 1/(constant+ linear*distance +quadratic*(distance*distance));
     
     //Ambient Lighting
     float ambientStrength =0.1;
     vec3 ambient = attenuation * ambientStrength* lightCol;
     
     //float theta =  dot(lightDir,normalize(-spotDir));
     //float epsilon = (cutoff - outer_cutoff);
     //float intensity = clamp((theta - outer_cutoff) / epsilon, 0.0, 1.0);
   
     float intensity =1.0f;

     float diff = max(dot(lightDir,norm),0.0);
     vec3 diffuse = intensity*attenuation * diff * lightCol;
     
     
     float specularIntensity = 0.8;
     vec3 viewDir = normalize(viewPos-FragPos);
     vec3 reflectDir = reflect(-lightDir,norm);
     
     
     float spec = pow(max(0.0,dot(reflectDir,viewDir)),128);
     vec3 specular = intensity * attenuation * specularIntensity*spec*lightCol;
    
     float shad = ShadowCalculation(fragPosLightSpace);
     FragColor = vec4( attenuation * (ambient + (1.0f-shad)* (diffuse +specular)) * texture(texture_diffuse1, TexCoords).rgb,1.0f);
     
    // FragColor = vec4(viewPos,1.0f);
      
    
}
