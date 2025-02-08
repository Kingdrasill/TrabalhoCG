#version 330 core

uniform sampler2D TextureSampler;
uniform sampler2D NuvemTextureSampler;

uniform float Time;

uniform vec2 VelocidadeRotacaoNuvem = vec2(0.008, 0.0);

in vec3 Normal;
in vec3 Color;
in vec2 UV;

uniform vec3 DirecaoLuz;
uniform float IntensidadeLuz;
uniform vec3 CameraPos;

// Declarando uma variável de saída do tipo vec4
out vec4 OutColor;

void main(){
	
	//Renormalizando o vetor Normal (por causa da interpolação linear)
	vec3 N = normalize(Normal);

	//Calcular o vertor L, inveter a direção da luz
	vec3 L = -normalize(DirecaoLuz);

	float calculoIluminacao = max(dot(N, L), 0);
	
	vec3 ViewDirection = normalize(DirecaoLuz);
	vec3 V = -ViewDirection;

	vec3 R = reflect(-L, N);

	float Alpha = 50.0;
	float Specular = pow(max(dot(R, V),0), Alpha);
	Specular = max(Specular, 0);

	vec3 TextureMundoColor = texture(TextureSampler, UV).rgb;

	vec3 TextureNuvemColor = texture(NuvemTextureSampler, UV + Time * VelocidadeRotacaoNuvem).rgb;

	// Aplicando a Luz na textura
	vec3 FinalColor =  (TextureMundoColor + TextureNuvemColor) * IntensidadeLuz * calculoIluminacao + Specular;

	OutColor = vec4(FinalColor, 1.0);
}