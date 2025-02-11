#version 330 core

uniform sampler2D TextureSampler;
in vec2 UV;

out vec4 FragColor;

void main()
{
    vec3 TextureColor = texture(TextureSampler, UV).rgb;
    FragColor = vec4(TextureColor, 1.0);
}