#ifndef OBJETOS_H
#define OBJETOS_H

#include "Geometria.h"

class Floor {
public:
	glm::vec3 Centro;
	GLuint Texture;
	glm::mat4 Model;
	GLuint FloorTV;
	GLuint FloorTI;
	GLuint FloorVAO;


	Floor(GLuint texture, float xsize, float ysize, float zsize, glm::vec3 centro) {
		Model = glm::mat4(1.0f);
		
		Centro = centro;
		Texture = texture;

		FloorTV = 0;
		FloorTI = 0;
		FloorVAO = CarregaQuadrilatero(FloorTV, FloorTI, 200, xsize, ysize, zsize, Centro);
	}

	void Render(Shader* shader) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBindVertexArray(FloorVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, FloorTI, GL_UNSIGNED_INT, nullptr);
	}

};

class Arvore {
public:
	glm::vec3 CentroBase;
	GLuint TextureBase;
	glm::mat4 ModelBase;
	GLuint BaseTV;
	GLuint BaseTI;
	GLuint BaseVAO;

	glm::vec3 CentroCopa;
	GLuint TextureCopa;
	glm::mat4 ModelCopa;
	GLuint CopaTV;
	GLuint CopaTI;
	GLuint CopaVAO;

	Arvore() {}

	Arvore(GLuint texturB, GLuint textureC, glm::vec3 centroB, float alturaB, float alturaC, float raioB, float raioC) {
		ModelBase = glm::mat4(1.0f);

		ModelCopa = glm::mat4(1.0f);

		CentroBase = centroB;
		TextureBase = texturB;

		CentroCopa = centroB;
		CentroCopa.y = alturaB + alturaC / 2 - 3.0f;
		TextureCopa = textureC;

		BaseTV = 0;
		BaseTI = 0;
		BaseVAO = CarregaCilindro(BaseTV, BaseTI, 10, alturaB, raioB, centroB);

		CopaTV = 0;
		CopaTI = 0;
		CopaVAO = CarregaCone(CopaTV, CopaTI, alturaC, raioC, CentroCopa);
	}

	void Render(Shader* shader) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureBase);

		glBindVertexArray(BaseVAO);
		shader->setMat4("Model", ModelBase);
		glDrawElements(GL_TRIANGLES, BaseTI, GL_UNSIGNED_INT, nullptr);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureCopa);

		glBindVertexArray(CopaVAO);
		shader->setMat4("Model", ModelCopa);
		glDrawElements(GL_TRIANGLES, CopaTI, GL_UNSIGNED_INT, nullptr);
	}
};

#endif // !OBJETOS_H
