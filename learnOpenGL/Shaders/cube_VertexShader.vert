#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out the{
    vec3 Normals;
	vec3 Positions;
};


uniform mat4 u_mvp;
uniform mat4 model;

void main()
{
    Normals = mat3(transpose(inverse(model))) * aNormal;
	Positions = vec3(model * vec4(aPos,1.0f));
	
	gl_Position =  u_mvp * vec4(aPos,1.0f);
}