#version 330 core

uniform sampler2D TextureSampler;

in vec3 Color;
in vec2 UV;

out vec4 OutColor;

void main(){
	OutColor = vec4(Color, 1.0);
}