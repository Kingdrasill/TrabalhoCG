#ifndef LIGHT_H
#define LIGHT_H

#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

class DirectionalLight {
public:
	glm::vec3 Direction;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Color;

	DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color) {
		Direction = direction;
		Ambient = ambient;
		Diffuse = diffuse;
		Specular = specular;
		Color = color;
	}

	void setToShader(Shader* shader) {
		shader->setVec3("dirLight.direction", Direction);
		shader->setVec3("dirLight.ambient", Ambient);
		shader->setVec3("dirLight.diffuse", Diffuse);
		shader->setVec3("dirLight.specular", Specular);
		shader->setVec3("dirLight.color", Color);
	}
};

class PointLight {
public:
	glm::vec3 Position;
	float Constant;
	float Linear;
	float Quadratic;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Color;

	PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color) {
		Position = position;
		Constant = constant;
		Linear = linear;
		Quadratic = quadratic;
		Ambient = ambient;
		Diffuse = diffuse;
		Specular = specular;
		Color = color;
	}

	void setToShader(Shader* shader, int id) {
		shader->setVec3("pointLights[" + std::to_string(id) + "].position", Position);
		shader->setVec3("pointLights[" + std::to_string(id) + "].ambient", Ambient);
		shader->setVec3("pointLights[" + std::to_string(id) + "]..diffuse", Diffuse);
		shader->setVec3("pointLights[" + std::to_string(id) + "].specular", Specular);
		shader->setFloat("pointLights[" + std::to_string(id) + "].constant", Constant);
		shader->setFloat("pointLights[" + std::to_string(id) + "].linear", Linear);
		shader->setFloat("pointLights[" + std::to_string(id) + "].quadratic", Quadratic);
		shader->setVec3("pointLights[" + std::to_string(id) + "].color", Color);
	}
};

class SpotLight {
public:
	glm::vec3 Position;
	glm::vec3 Direction;
	float CutOff;
	float OuterCutOff;
	float Constant;
	float Linear;
	float Quadratic;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Color;

	SpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color) {
		Position = position;
		Direction = direction;
		CutOff = cutOff;
		OuterCutOff = outerCutOff;
		Constant = constant;
		Linear = linear;
		Quadratic = quadratic;
		Ambient = ambient;
		Diffuse = diffuse;
		Specular = specular;
		Color = color;
	}

	void updateCameraValues(glm::vec3 position, glm::vec3 direction) {
		Position = position;
		Direction = direction;
	}

	void setToShader(Shader* shader) {
		shader->setVec3("spotLight.position", Position);
		shader->setVec3("spotLight.direction", Direction);
		shader->setFloat("spotLight.cutOff", CutOff);
		shader->setFloat("spotLight.outerCutOff", OuterCutOff);
		shader->setFloat("spotLight.constant", Constant);
		shader->setFloat("spotLight.linear", Linear);
		shader->setFloat("spotLight.quadratic", Quadratic);
		shader->setVec3("spotLight.ambient", Ambient);
		shader->setVec3("spotLight.diffuse", Diffuse);
		shader->setVec3("spotLight.specular", Specular);
		shader->setVec3("spotLight.color", Color);
	}
};

#endif // !LIGHT_H
