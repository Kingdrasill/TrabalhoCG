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

	Arvore(GLuint textureB, GLuint textureC, glm::vec3 centroB, float alturaB, float alturaC, float raioB, float raioC, float elevacaoC) {
		ModelBase = glm::mat4(1.0f);

		ModelCopa = glm::mat4(1.0f);

		CentroBase = centroB;
		TextureBase = textureB;

		CentroCopa = centroB;
		if (alturaB - elevacaoC <= alturaC / 2) {
			CentroCopa.y = (centroB.y - alturaB / 2) + (alturaC / 2.0f) + elevacaoC;
		}
		else {
			CentroCopa.y = (centroB.y) + elevacaoC;
		}
		TextureCopa = textureC;

		BaseTV = 0;
		BaseTI = 0;
		BaseVAO = CarregaCilindro(BaseTV, BaseTI, 10, alturaB, raioB, CentroBase);

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

class Arbusto {
public:
	glm::vec3 Centro;
	GLuint Texture;
	glm::mat4 Model;
	GLuint ArbustoTV;
	GLuint ArbustoTI;
	GLuint ArbustoVAO;

	Arbusto() {}

	Arbusto(GLuint texture, glm::vec3 centro, float raio) {
		Model = glm::mat4(1.0f);
		Model = glm::translate(Model, centro);
		Model = glm::rotate(Model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::translate(Model, -centro);

		Centro = centro;
		Texture = texture;

		ArbustoTV = 0;
		ArbustoTI = 0;
		ArbustoVAO = CarregarSemiesfera(ArbustoTV, ArbustoTI, raio, Centro);
	}

	void Render(Shader* shader) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBindVertexArray(ArbustoVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, ArbustoTI, GL_UNSIGNED_INT, nullptr);
	}
};

class Cerca {
public:
	glm::vec3 Centro;
	GLuint Texture;
	glm::mat4 ModelVertical;
	glm::mat4 ModelHorizontalU;
	glm::mat4 ModelHorizontalD;

	GLuint CercaLTV;
	GLuint CercaLTI;
	GLuint CercaLVAO;

	GLuint CercaMTV;
	GLuint CercaMTI;
	GLuint CercaMVAO;

	GLuint CercaRTV;
	GLuint CercaRTI;
	GLuint CercaRVAO;

	GLuint CercaUTV;
	GLuint CercaUTI;
	GLuint CercaUVAO;

	GLuint CercaDTV;
	GLuint CercaDTI;
	GLuint CercaDVAO;
	
	Cerca() {}

	Cerca(GLuint texture, glm::vec3 centro, float largura, float angulo) {
		ModelVertical = glm::mat4(1.0f);
		ModelVertical = glm::translate(ModelVertical, -centro);
		ModelVertical = glm::rotate(ModelVertical, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		ModelVertical = glm::translate(ModelVertical, centro);
		Texture = texture;

		glm::vec3 centroL = centro - glm::vec3(largura, 0.0f, 0.0f);
		glm::vec3 centroM = centro;
		glm::vec3 centroR = centro + glm::vec3(largura, 0.0f, 0.0f);

		CercaLTV = 0;
		CercaLTI = 0;
		CercaLVAO = CarregaQuadrilatero(CercaLTV, CercaLTI, 1, 0.5, 2.0f, 0.2f, centroL);

		CercaMTV = 0;
		CercaMTI = 0;
		CercaMVAO = CarregaQuadrilatero(CercaMTV, CercaMTI, 1, 0.5, 2.0f, 0.2f, centroM);

		CercaRTV = 0;
		CercaRTI = 0;
		CercaRVAO = CarregaQuadrilatero(CercaRTV, CercaRTI, 1, 0.5, 2.0f, 0.2f, centroR);


		glm::vec3 centroU = centro + glm::vec3(0.0f, 0.5, 0.1f);
		glm::vec3 centroD = centro + glm::vec3(0.0f, -0.5, 0.1f);

		CercaUTV = 0;
		CercaUTI = 0;
		CercaUVAO = CarregaQuadrilatero(CercaUTV, CercaUTI, 1, 2*largura+0.75, 0.5, 0.2f, centroU);

		CercaDTV = 0;
		CercaDTI = 0;
		CercaDVAO = CarregaQuadrilatero(CercaDTV, CercaDTI, 1, 2*largura+0.75, 0.5, 0.2f, centroD);
	}

	void Render(Shader* shader) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBindVertexArray(CercaLVAO);
		shader->setMat4("Model", ModelVertical);
		glDrawElements(GL_TRIANGLES, CercaLTI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(CercaMVAO);
		shader->setMat4("Model", ModelVertical);
		glDrawElements(GL_TRIANGLES, CercaMTI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(CercaRVAO);
		shader->setMat4("Model", ModelVertical);
		glDrawElements(GL_TRIANGLES, CercaRTI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(CercaUVAO);
		shader->setMat4("Model", ModelVertical);
		glDrawElements(GL_TRIANGLES, CercaUTI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(CercaDVAO);
		shader->setMat4("Model", ModelVertical);
		glDrawElements(GL_TRIANGLES, CercaDTI, GL_UNSIGNED_INT, nullptr);
	}
};

#endif // !OBJETOS_H
