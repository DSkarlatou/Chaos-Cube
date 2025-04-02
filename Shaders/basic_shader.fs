#version 330 core

out vec4 FragColor;

in vec3 VertexColor;

uniform vec4 Color;

void main()
{
	FragColor = Color;
}