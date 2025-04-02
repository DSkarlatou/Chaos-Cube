#version 330 core
layout (location = 0) in vec3 Pos;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

out vec3 VertexColor;

void main()
{
	VertexColor = Pos;
	gl_Position = Projection * View * Model * vec4(Pos, 1.0f);
}