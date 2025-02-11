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

	void Delete() {
		glDeleteBuffers(1, &FloorVAO);
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

	void Delete() {
		glDeleteBuffers(1, &BaseVAO);

		glDeleteBuffers(1, &CopaVAO);
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

	void Delete() {
		glDeleteBuffers(1, &ArbustoVAO);
	}
};

class Cerca {
public:
	glm::vec3 Centro;
	GLuint Texture;
	GLuint TextureFlipped;

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

	Cerca(GLuint texture, GLuint textureFlipped, glm::vec3 centro, float largura, float angulo, float centroH, float tamanhoH) {
		float lrgInd = largura / 6;

		ModelVertical = glm::mat4(1.0f);
		ModelVertical = glm::translate(ModelVertical, centro);
		ModelVertical = glm::rotate(ModelVertical, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		ModelVertical = glm::translate(ModelVertical, -centro);

		Texture = texture;
		TextureFlipped = textureFlipped;

		glm::vec3 centroL = centro - glm::vec3(2*lrgInd, 0.0f, 0.0f);
		glm::vec3 centroM = centro;
		glm::vec3 centroR = centro + glm::vec3(2*lrgInd, 0.0f, 0.0f);

		CercaLTV = 0;
		CercaLTI = 0;
		CercaLVAO = CarregaQuadrilatero(CercaLTV, CercaLTI, 1, lrgInd, 1.5f, 0.2f, centroL);

		CercaMTV = 0;
		CercaMTI = 0;
		CercaMVAO = CarregaQuadrilatero(CercaMTV, CercaMTI, 1, lrgInd, 1.5f, 0.2f, centroM);

		CercaRTV = 0;
		CercaRTI = 0;
		CercaRVAO = CarregaQuadrilatero(CercaRTV, CercaRTI, 1, lrgInd, 1.5f, 0.2f, centroR);


		glm::vec3 centroU = centro + glm::vec3(0.0f, centroH*lrgInd, 0.1f);
		glm::vec3 centroD = centro + glm::vec3(0.0f, -centroH*lrgInd, 0.1f);

		CercaUTV = 0;
		CercaUTI = 0;
		CercaUVAO = CarregaQuadrilatero(CercaUTV, CercaUTI, 1, 6*lrgInd, tamanhoH*lrgInd, 0.2f, centroU);

		CercaDTV = 0;
		CercaDTI = 0;
		CercaDVAO = CarregaQuadrilatero(CercaDTV, CercaDTI, 1, 6*lrgInd, tamanhoH*lrgInd, 0.2f, centroD);
	}

	void Render(Shader* shader) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureFlipped);

		glBindVertexArray(CercaLVAO);
		shader->setMat4("Model", ModelVertical);
		glDrawElements(GL_TRIANGLES, CercaLTI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(CercaMVAO);
		shader->setMat4("Model", ModelVertical);
		glDrawElements(GL_TRIANGLES, CercaMTI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(CercaRVAO);
		shader->setMat4("Model", ModelVertical);
		glDrawElements(GL_TRIANGLES, CercaRTI, GL_UNSIGNED_INT, nullptr);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBindVertexArray(CercaUVAO);
		shader->setMat4("Model", ModelVertical);
		glDrawElements(GL_TRIANGLES, CercaUTI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(CercaDVAO);
		shader->setMat4("Model", ModelVertical);
		glDrawElements(GL_TRIANGLES, CercaDTI, GL_UNSIGNED_INT, nullptr);
	}

	void Delete() {
		glDeleteBuffers(1, &CercaLVAO);

		glDeleteBuffers(1, &CercaMVAO);

		glDeleteBuffers(1, &CercaRVAO);

		glDeleteBuffers(1, &CercaUVAO);

		glDeleteBuffers(1, &CercaDVAO);
	}
};

class Parede {
public:
	glm::vec3 Centro;
	GLuint Texture;
	glm::mat4 Model;
	GLuint ParedeTV; // Número total de vértices
	GLuint ParedeTI; // Número total de índices
	GLuint ParedeVAO; // Vertex Array Object


	Parede(GLuint texture, float xsize, float ysize, float zsize, glm::vec3 centro) {
		Model = glm::mat4(1.0f);

		Centro = centro;
		Texture = texture;

		ParedeTV = 0;
		ParedeTI = 0;
		ParedeVAO = CarregaQuadrilatero(ParedeTV, ParedeTI, 1, xsize, ysize, zsize, Centro);
	}

	void Render(Shader* shader) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBindVertexArray(ParedeVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, ParedeTI, GL_UNSIGNED_INT, nullptr);
	}

	void Delete() {
		glDeleteBuffers(1, &ParedeVAO);
	}
};

class ParedePorta {
public:
	glm::vec3 Centro;
	GLuint Texture;
	glm::mat4 Model;
	// Parede Esquerda
	GLuint ParedeETV; // Número total de vértices
	GLuint ParedeETI; // Número total de índices
	GLuint ParedeEVAO; // Vertex Array Object

	// Parede Direita
	GLuint ParedeDTV; // Número total de vértices
	GLuint ParedeDTI; // Número total de índices
	GLuint ParedeDVAO; // Vertex Array Object

	// Parede Cima
	GLuint ParedeCTV; // Número total de vértices
	GLuint ParedeCTI; // Número total de índices
	GLuint ParedeCVAO; // Vertex Array Object

	ParedePorta(GLuint texture, float xsize, float ysize, float zsize, float xsizeporta, float ysizeporta, glm::vec3 centro, glm::vec3 distanciaporta) {
		Model = glm::mat4(1.0f);

		Centro = centro;
		Texture = texture;

		float xsizeE;
		float xsizeD;
		float xcentroE;
		float xcentroD;
		glm::vec3 centroE;
		glm::vec3 centroC;
		glm::vec3 centroD;


		xsizeE = xsize / 2 + distanciaporta.x - xsizeporta / 2;
		xcentroE = xsizeE / 2 - distanciaporta.x + xsizeporta / 2;
		xsizeD = xsize - xsizeE - xsizeporta;
		xcentroD = centro.x - xsize / 2 + xsizeD / 2;


		centroE = centro + glm::vec3(xcentroE, 0, 0);		 
		centroC = centro - distanciaporta + glm::vec3(0, ysize / 2, 0);
		centroD = centro + glm::vec3(xcentroD, 0, 0);

		ParedeETV = 0;
		ParedeETI = 0;
		ParedeEVAO = CarregaQuadrilatero(ParedeETV, ParedeETI, 1, xsizeE, ysize, zsize, centroE);

		ParedeCTV = 0;
		ParedeCTI = 0;
		ParedeCVAO = CarregaQuadrilatero(ParedeCTV, ParedeCTI, 1, xsizeporta, ysize - ysizeporta, zsize, centroC);

		ParedeDTV = 0;
		ParedeDTI = 0;
		ParedeDVAO = CarregaQuadrilatero(ParedeDTV, ParedeDTI, 1, xsizeD, ysize, zsize, centroD);
	}

	void Render(Shader* shader) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBindVertexArray(ParedeEVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, ParedeETI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(ParedeDVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, ParedeDTI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(ParedeCVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, ParedeCTI, GL_UNSIGNED_INT, nullptr);
	}

	void Delete() {
		glDeleteBuffers(1, &ParedeEVAO);
		glDeleteBuffers(1, &ParedeDVAO);
		glDeleteBuffers(1, &ParedeCVAO);
	}
};

class ParedeJanela {
public:
	glm::vec3 Centro;
	GLuint Texture;
	glm::mat4 Model;
	// Parede Esquerda
	GLuint ParedeETV; // Número total de vértices
	GLuint ParedeETI; // Número total de índices
	GLuint ParedeEVAO; // Vertex Array Object

	// Parede Direita
	GLuint ParedeDTV; // Número total de vértices
	GLuint ParedeDTI; // Número total de índices
	GLuint ParedeDVAO; // Vertex Array Object

	// Parede Cima
	GLuint ParedeCTV; // Número total de vértices
	GLuint ParedeCTI; // Número total de índices
	GLuint ParedeCVAO; // Vertex Array Object

	// Parede Baixo
	GLuint ParedeBTV; // Número total de vértices
	GLuint ParedeBTI; // Número total de índices
	GLuint ParedeBVAO; // Vertex Array Object

	ParedeJanela(GLuint texture, float xsize, float ysize, float zsize, float xsizejanela, float ysizejanela, glm::vec3 centro, glm::vec3 distanciajanela) {
		Model = glm::mat4(1.0f);

		Centro = centro;
		Texture = texture;

		float xsizeE;
		float xsizeD;
		float xcentroE;
		float xcentroD;
		glm::vec3 centroE;
		glm::vec3 centroC;
		glm::vec3 centroD;
		glm::vec3 centroB;

		float yTopo = centro.y + ysize / 2; // 1.5
		float yBottom = centro.y - ysize / 2; // -1.5

		float yTopJanela = centro.y - distanciajanela.y + ysizejanela / 2; // 0 - 0.5 + 0.5 = 0
		float yBottomJanela = centro.y - distanciajanela.y - ysizejanela / 2; // 0 - 0.5 - 0.5 = -1

		xsizeE = xsize / 2 + distanciajanela.x - xsizejanela / 2;
		xcentroE = xsizeE / 2 - distanciajanela.x + xsizejanela / 2;
		xsizeD = xsize - xsizeE - xsizejanela;
		xcentroD = centro.x - xsize / 2 + xsizeD / 2;


		centroE = centro + glm::vec3(xcentroE, 0, 0);
		centroC = centro - distanciajanela + glm::vec3(0, ysizejanela / 2 + (yTopo - yTopJanela)/ 2, 0); // 0 - 0.5 + 0.5 + (1.5 - 0)/2 = 0.75 = 1.5 e até 0 
		centroD = centro + glm::vec3(xcentroD, 0, 0);
		centroB = centro - distanciajanela + glm::vec3(0, - ysizejanela / 2 - (yBottomJanela - yBottom) / 2, 0); // 0 - 0.5 - 0.5 + (1.5 - 0)/2 = 0.75 = 1.5 e até 0 

		ParedeETV = 0;
		ParedeETI = 0;
		ParedeEVAO = CarregaQuadrilatero(ParedeETV, ParedeETI, 1, xsizeE, ysize, zsize, centroE);

		ParedeCTV = 0;
		ParedeCTI = 0;
		ParedeCVAO = CarregaQuadrilatero(ParedeCTV, ParedeCTI, 1, xsizejanela, yTopo - yTopJanela, zsize, centroC); // 1,5

		ParedeBTV = 0;
		ParedeBTI = 0;
		ParedeBVAO = CarregaQuadrilatero(ParedeBTV, ParedeBTI, 1, xsizejanela, yBottomJanela - yBottom, zsize, centroB); // -1 - -1.5 = 0.5

		ParedeDTV = 0;
		ParedeDTI = 0;
		ParedeDVAO = CarregaQuadrilatero(ParedeDTV, ParedeDTI, 1, xsizeD, ysize, zsize, centroD);
	}

	void Render(Shader* shader) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBindVertexArray(ParedeEVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, ParedeETI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(ParedeDVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, ParedeDTI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(ParedeCVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, ParedeCTI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(ParedeBVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, ParedeBTI, GL_UNSIGNED_INT, nullptr);
	}

	void Delete() {
		glDeleteBuffers(1, &ParedeEVAO);
		glDeleteBuffers(1, &ParedeDVAO);
		glDeleteBuffers(1, &ParedeCVAO);
		glDeleteBuffers(1, &ParedeBVAO);
	}
};

#endif // !OBJETOS_H
