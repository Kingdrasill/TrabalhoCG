#version 330 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec3 InNormal;
layout (location = 2) in vec3 InColor;
layout (location = 3) in vec2 InUV;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 Normal;
out vec2 UV;
out vec3 FragPos;

void main(){
	FragPos = vec3(Model * vec4(InPosition, 1.0));

	Normal = mat3(transpose(inverse(Model))) * InNormal;
	UV = InUV;

	gl_Position = Projection * View * Model * vec4(InPosition, 1.0);
}