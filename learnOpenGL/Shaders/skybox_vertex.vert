#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;


uniform mat4 u_mvp;

out vec3 textureCoord;

void main()
{
	textureCoord = aPos;
	gl_Position = u_mvp * vec4(aPos,1.0f);
}
// Dear future me you changed the code a bit to get cube maps to work
// You psssed in the local vertex posions of the cube into the fragment shader so it can e used in sampling
