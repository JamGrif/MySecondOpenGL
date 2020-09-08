#version 460

out vec4 color; //out tag means the variable is an output
//gl_FragCoord access the coordinates of an incoming fragment
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0); //Set the colour of a pixel
}