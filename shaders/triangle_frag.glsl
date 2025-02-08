#version 330 core

struct Material {
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 ViewPos;

in vec3 Normal;
in vec2 UV;
in vec3 FragPos;

out vec4 FragColor;

void main(){
	vec3 Ambient = light.ambient * texture(material.diffuse, UV).rgb;

	vec3 Norm = normalize(Normal);

	vec3 LightDir = normalize(light.position - FragPos);
	float Diff = max(dot(Norm, LightDir), 0.0);
	vec3 Diffuse = light.diffuse * Diff * texture(material.diffuse, UV).rgb;

	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 ReflectDir = reflect(-LightDir, Norm);

	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), material.shininess);
	vec3 Specular = light.specular * Spec * material.specular;

	float theta = dot(LightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    Diffuse  *= intensity;
    Specular *= intensity;

	float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    Ambient *= attenuation;  
    Diffuse *= attenuation;
    Specular *= attenuation; 

	vec3 Result = Ambient + Diffuse + Specular;
	FragColor = vec4(Result, 1.0);
}