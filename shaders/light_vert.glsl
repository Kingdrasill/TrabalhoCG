#version 330 core
layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec3 InNormal;
layout (location = 2) in vec3 InColor;
layout (location = 3) in vec2 InUV;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 UV;

void main()
{
	UV = InUV;
	gl_Position = Projection * View * Model * vec4(InPosition, 1.0);
}