#version 460
layout (location = 0) in vec3 position; //"layout(location=0)" is how this vertex attribute is associated with a particular buffer. Identifier is 0

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main()
{
	gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
}