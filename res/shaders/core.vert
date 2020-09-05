#version 460

uniform float Xoffset;
uniform float Yoffset;


void main()
{
	if (gl_VertexID == 0) gl_Position = vec4(0.25 + Xoffset,-0.25 + Yoffset,0.0,1.0);
	else if (gl_VertexID == 1) gl_Position = vec4(-0.25 + Xoffset,-0.25+Yoffset,0.0,1.0);
	else gl_Position = vec4(0.25 +Xoffset,0.25+Yoffset,0.0,1.0);
}