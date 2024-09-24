#version 330 core
layout(points) in;
layout(triangle_strip,max_vertices = 3) out;

in VS_OUT{
	vec3 color;
}gs_in[];

out vec3 FragColour;

void main()
{
	FragColour = gs_in[0].color;
	gl_Position = gl_in[0].gl_Position + vec4(-0.2, -0.2, 0.0, 0.0); 
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(.2f,0.0f,0.0f,0.0f);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position+ vec4(0.0f,0.10f,0.0f,0.0f);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position +vec4(0.0f,-0.10f,0.0f,0.0f);
	EmitVertex();
	EndPrimitive();
}

