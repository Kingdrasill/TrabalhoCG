#ifndef OBJETOS_H
#define OBJETOS_H

#include "Geometria.h"

#include <array>

#define NUM_ARV 2000
#define NUM_ARB 2000
#define CRC_FRENTE 8
#define CRC_X 10
#define CRC_Z 15

class Floor {
public:
	glm::vec3 Centro;
	GLuint Texture;
	glm::mat4 Model;
	GLuint FloorTV;
	GLuint FloorTI;
	GLuint FloorVAO;

	Floor () {}

	Floor(GLuint texture, float xsize, float ysize, float zsize, int repeat, glm::vec3 centro, float angulo) {
		Model = glm::mat4(1.0f);
		Model = glm::translate(Model, centro);
		Model = glm::rotate(Model, glm::radians(angulo), glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::translate(Model, -centro);
		
		Centro = centro;
		Texture = texture;

		FloorTV = 0;
		FloorTI = 0;
		FloorVAO = CarregaQuadrilatero(FloorTV, FloorTI, repeat, xsize, ysize, zsize, Centro);
	}

	void Render(Shader* shader) const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBindVertexArray(FloorVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, FloorTI, GL_UNSIGNED_INT, nullptr);
	}

	void Delete() const {
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

	void Render(Shader* shader) const {
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

	void Delete() const {
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

	void Render(Shader* shader) const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBindVertexArray(ArbustoVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, ArbustoTI, GL_UNSIGNED_INT, nullptr);
	}

	void Delete() const {
		glDeleteBuffers(1, &ArbustoVAO);
	}
};

class Floresta {
public:
	std::array<Arvore, NUM_ARV> arvores;
	std::array<Arbusto, NUM_ARB> arbustos;

	Floresta() {}

	Floresta(GLuint textTreeId, GLuint textFolhaId, GLuint textArbustoId) {
		for (int i = 0; i < NUM_ARV; i++) {
			float x;
			float z;

			if (rand() % 2 == 0) {
				x = (rand() % 2 == 0) ? getRandomInRange(-250, -12) : getRandomInRange(12, 250);
				z = (rand() % 2 == 0) ? getRandomInRange(-250, 0) : getRandomInRange(0, 250);
			}
			else {
				x = (rand() % 2 == 0) ? getRandomInRange(-250, 0) : getRandomInRange(0, 250);
				z = (rand() % 2 == 0) ? getRandomInRange(-250, -17) : getRandomInRange(17, 250);
			}

			float raio = 0.25 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.75 - 0.25)));
			float alturaB = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.5 - 1)));
			float alturaC = 4 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - 4)));
			float elevacaoC = 0.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (alturaB - 0.5)));

			arvores[i] = Arvore(textTreeId, textFolhaId, glm::vec3(x, (-1.0 + 0.5 * (alturaB - 1)), z), alturaB, alturaC, raio * 0.8, 2.2 * raio, elevacaoC);
		}
		
		for (int i = 0; i < NUM_ARV; i++) {
			float x;
			float z;

			if (rand() % 2 == 0) {
				x = (rand() % 2 == 0) ? getRandomInRange(-250, -12) : getRandomInRange(12, 250);
				z = (rand() % 2 == 0) ? getRandomInRange(-250, 0) : getRandomInRange(0, 250);
			}
			else {
				x = (rand() % 2 == 0) ? getRandomInRange(-250, 0) : getRandomInRange(0, 250);
				z = (rand() % 2 == 0) ? getRandomInRange(-250, -17) : getRandomInRange(17, 250);
			}

			float raio = 0.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.5 - 0.5)));

			arbustos[i] = Arbusto(textArbustoId, glm::vec3(x, -1.5f, z), raio);
		}
	}

	void Render(Shader* shader) const {
		for (int i = 0; i < NUM_ARV; i++) {
			arvores[i].Render(shader);
		}

		for (int i = 0; i < NUM_ARB; i++) {
			arbustos[i].Render(shader);
		}
	}

	void Delete() const {
		for (int i = 0; i < NUM_ARV; i++) {
			arvores[i].Delete();
		}

		for (int i = 0; i < NUM_ARB; i++) {
			arbustos[i].Delete();
		}
	}

private:
	float getRandomInRange(float min, float max) {
		return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
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

	void Render(Shader* shader) const {
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

	void Delete() const {
		glDeleteBuffers(1, &CercaLVAO);

		glDeleteBuffers(1, &CercaMVAO);

		glDeleteBuffers(1, &CercaRVAO);

		glDeleteBuffers(1, &CercaUVAO);

		glDeleteBuffers(1, &CercaDVAO);
	}
};

class Cercas {
public:
	std::array<Cerca, CRC_X> cercas_fundo;
	std::array<Cerca, CRC_Z> cercas_esq;
	std::array<Cerca, CRC_Z> cercas_dir;
	std::array<Cerca, CRC_FRENTE> cercas_frente;

	Cercas(GLuint CercaViradaId, GLuint CercaId) {
		float largura = 2;
		int count = 0;
		for (int i = 1; i < CRC_X / 2 + 1; i++) {
			cercas_fundo[count] = Cerca(CercaViradaId, CercaId, glm::vec3(-i * largura + largura / 2, -0.75, -15.0f), largura, 0.0f, 1.0f, 1.0f);
			count++;
			cercas_fundo[count] = Cerca(CercaViradaId, CercaId, glm::vec3(i * largura - largura / 2, -0.75, -15.0f), largura, 0.0f, 1.0f, 1.0f);
			count++;
		}

		count = 0;
		cercas_esq[0] = Cerca(CercaViradaId, CercaId, glm::vec3(-10.0f, -0.75, 0.0f), largura, 90.0f, 1.0f, 1.0f);
		count++;
		for (int i = 1; i < (CRC_Z - 1) / 2 + 1; i++) {
			cercas_esq[count] = Cerca(CercaViradaId, CercaId, glm::vec3(-10.0f, -0.75, -i * largura), largura, 90.0f, 1.0f, 1.0f);
			count++;
			cercas_esq[count] = Cerca(CercaViradaId, CercaId, glm::vec3(-10.0f, -0.75, i * largura), largura, 90.0f, 1.0f, 1.0f);
			count++;
		}

		count = 0;
		cercas_dir[0] = Cerca(CercaViradaId, CercaId, glm::vec3(10.0f, -0.75, 0.0f), largura, -90.0f, 1.0f, 1.0f);
		count++;
		for (int i = 1; i < (CRC_Z - 1) / 2 + 1; i++) {
			cercas_dir[count] = Cerca(CercaViradaId, CercaId, glm::vec3(10.0f, -0.75, -i * largura), largura, -90.0f, 1.0f, 1.0f);
			count++;
			cercas_dir[count] = Cerca(CercaViradaId, CercaId, glm::vec3(10.0f, -0.75, i * largura), largura, -90.0f, 1.0f, 1.0f);
			count++;
		}

		count = 0;
		for (int i = 1; i < CRC_FRENTE / 2 + 1; i++) {
			cercas_frente[count] = Cerca(CercaViradaId, CercaId, glm::vec3(-i * largura + largura / 2 - 2, -0.75, 15.0f), largura, 180.0f, 1.0f, 1.0f);
			count++;
			cercas_frente[count] = Cerca(CercaViradaId, CercaId, glm::vec3(i * largura - largura / 2 + 2, -0.75, 15.0f), largura, 180.0f, 1.0f, 1.0f);
			count++;
		}
	}

	void Render(Shader* shader) const {
		for (int i = 0; i < CRC_X; i++) {
			cercas_fundo[i].Render(shader);
		}

		for (int i = 0; i < CRC_Z; i++) {
			cercas_esq[i].Render(shader);
		}

		for (int i = 0; i < CRC_Z; i++) {
			cercas_dir[i].Render(shader);
		}

		for (int i = 0; i < CRC_FRENTE; i++) {
			cercas_frente[i].Render(shader);
		}
	}

	void Delete() const {
		for (int i = 0; i < CRC_X; i++) {
			cercas_fundo[i].Delete();
		}
		for (int i = 0; i < CRC_Z; i++) {
			cercas_esq[i].Delete();
		}
		for (int i = 0; i < CRC_Z; i++) {
			cercas_dir[i].Delete();
		}
		for (int i = 0; i < CRC_FRENTE; i++) {
			cercas_frente[i].Delete();
		}
	}
};

class Bloco {
public:
	glm::vec3 Centro;
	GLuint Texture;
	glm::mat4 Model;
	GLuint BlocoTV; // Número total de vértices
	GLuint BlocoTI; // Número total de índices
	GLuint BlocoVAO; // Vertex Array Object

	Bloco() {}

	Bloco(GLuint texture, float xsize, float ysize, float zsize, glm::vec3 centro) {
		Model = glm::mat4(1.0f);

		Centro = centro;
		Texture = texture;

		BlocoTV = 0;
		BlocoTI = 0;
		BlocoVAO = CarregaQuadrilatero(BlocoTV, BlocoTI, 1, xsize, ysize, zsize, Centro);
	}

	void Render(Shader* shader) const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBindVertexArray(BlocoVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, BlocoTI, GL_UNSIGNED_INT, nullptr);
	}

	void Delete() const {
		glDeleteBuffers(1, &BlocoVAO);
	}

	void Rotate(float angulo) {
		Model = glm::translate(Model, Centro);
		Model = glm::rotate(Model, glm::radians(angulo), glm::vec3(0.0f, 0.0f, 1.0f));
		Model = glm::translate(Model, -Centro);
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

	ParedePorta() {}

	ParedePorta(GLuint texture, float xsize, float ysize, float zsize, float xsizeporta, float ysizeporta, float angulo, glm::vec3 centro, glm::vec3 distanciaporta) {
		Model = glm::mat4(1.0f);
		Model = glm::translate(Model, centro);
		Model = glm::rotate(Model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		Model = glm::translate(Model, -centro);

		Centro = centro;
		Texture = texture;

		float xsizeE;
		float xsizeD;
		glm::vec3 centroE;
		glm::vec3 centroC;
		glm::vec3 centroD;


		xsizeE = xsize / 2 + distanciaporta.x - xsizeporta / 2;
		xsizeD = xsize - xsizeE - xsizeporta;
 
		centroE = centro + glm::vec3(distanciaporta.x - xsizeporta/2 - xsizeE/2, 0, distanciaporta.z);
		centroC = centro + glm::vec3(distanciaporta.x, ysize / 2 - distanciaporta.y, distanciaporta.z);
		centroD = centro + glm::vec3(distanciaporta.x + xsizeporta/2 + xsizeD/2, 0, distanciaporta.z);

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

	void Render(Shader* shader) const {
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

	void Delete() const {
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

	ParedeJanela() {}

	ParedeJanela(GLuint texture, float xsize, float ysize, float zsize, float xsizejanela, float ysizejanela, float angulo, glm::vec3 centro, glm::vec3 distanciajanela) {
		Model = glm::mat4(1.0f);
		Model = glm::translate(Model, centro);
		Model = glm::rotate(Model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		Model = glm::translate(Model, -centro);

		Centro = centro;
		Texture = texture;

		float xsizeE;
		float xsizeD;
		glm::vec3 centroE;
		glm::vec3 centroC;
		glm::vec3 centroD;
		glm::vec3 centroB;

		float yTopo = centro.y + ysize / 2; // 1.5
		float yBottom = centro.y - ysize / 2; // -1.5

		float yTopJanela = centro.y - distanciajanela.y + ysizejanela / 2;
		float yBottomJanela = centro.y - distanciajanela.y - ysizejanela / 2;

		xsizeE = xsize / 2 + distanciajanela.x - xsizejanela / 2;
		xsizeD = xsize - xsizeE - xsizejanela;

		centroE = centro + glm::vec3(distanciajanela.x -xsizejanela/2 - xsizeE/2, 0, 0);
		centroC = centro + glm::vec3(distanciajanela.x, -distanciajanela.y + ysizejanela / 2 + (yTopo - yTopJanela)/2, 0);
		centroD = centro + glm::vec3(distanciajanela.x + xsizejanela/2 + xsizeD/2, 0, 0);
		centroB = centro + glm::vec3(distanciajanela.x, -distanciajanela.y - ysizejanela / 2 - (yBottomJanela - yBottom)/2, 0);

		ParedeETV = 0;
		ParedeETI = 0;
		ParedeEVAO = CarregaQuadrilatero(ParedeETV, ParedeETI, 1, xsizeE, ysize, zsize, centroE);

		ParedeCTV = 0;
		ParedeCTI = 0;
		ParedeCVAO = CarregaQuadrilatero(ParedeCTV, ParedeCTI, 1, xsizejanela, yTopo - yTopJanela, zsize, centroC); 

		ParedeBTV = 0;
		ParedeBTI = 0;
		ParedeBVAO = CarregaQuadrilatero(ParedeBTV, ParedeBTI, 1, xsizejanela, yBottomJanela - yBottom, zsize, centroB); 

		ParedeDTV = 0;
		ParedeDTI = 0;
		ParedeDVAO = CarregaQuadrilatero(ParedeDTV, ParedeDTI, 1, xsizeD, ysize, zsize, centroD);
	}

	void Render(Shader* shader) const {
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

	void Delete() const {
		glDeleteBuffers(1, &ParedeEVAO);
		glDeleteBuffers(1, &ParedeDVAO);
		glDeleteBuffers(1, &ParedeCVAO);
		glDeleteBuffers(1, &ParedeBVAO);
	}
};

class Roof {
public:
	glm::vec3 Centro;
	GLuint TextureBase;
	glm::mat4 ModelBase;

	GLuint TextureRoof;
	glm::mat4 ModelRoofE;
	glm::mat4 ModelRoofD;

	// Teto Base
	GLuint RoofBaseTV; // Número total de vértices
	GLuint RoofBaseTI; // Número total de índices
	GLuint RoofBaseVAO; // Vertex Array Object

	// Teto Esquerda
	GLuint RoofETV; // Número total de vértices
	GLuint RoofETI; // Número total de índices
	GLuint RoofEVAO; // Vertex Array Object

	// Teto Direita
	GLuint RoofDTV; // Número total de vértices
	GLuint RoofDTI; // Número total de índices
	GLuint RoofDVAO; // Vertex Array Object

	Roof() {}

	Roof(GLuint textureB, GLuint textureR, float xsize, float ysize, float zsize, glm::vec3 centro) {
		ModelBase = glm::mat4(1.0f);
		ModelBase = glm::translate(ModelBase, centro);
		ModelBase = glm::rotate(ModelBase, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ModelBase = glm::rotate(ModelBase, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		ModelBase = glm::translate(ModelBase, -centro);

		ModelRoofE = glm::mat4(1);
		ModelRoofE = glm::translate(ModelRoofE, centro);
		ModelRoofE = glm::rotate(ModelRoofE, glm::radians(-74.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ModelRoofE = glm::translate(ModelRoofE, -centro);
		ModelRoofE = glm::translate(ModelRoofE, -glm::vec3(1,2.5,0));

		ModelRoofD = glm::mat4(1);
		ModelRoofD = glm::translate(ModelRoofD, centro);
		ModelRoofD = glm::rotate(ModelRoofD, glm::radians(74.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ModelRoofD = glm::translate(ModelRoofD, -centro);
		ModelRoofD = glm::translate(ModelRoofD, glm::vec3(1,-2.5, 0));

		Centro = centro;
		TextureBase = textureB;
		TextureRoof = textureR;

		RoofBaseTV = 0;
		RoofBaseTI = 0;
		RoofBaseVAO = CarregarPrisma(RoofBaseTV, RoofBaseTI, xsize, ysize, zsize, 3, centro);

		RoofETV = 0;
		RoofETI = 0;
		RoofEVAO = CarregaQuadrilatero(RoofETV, RoofETI, 2, 0.2f, 6.0f, zsize+9, centro);

		RoofDTV = 0;
		RoofDTI = 0;
		RoofDVAO = CarregaQuadrilatero(RoofDTV, RoofDTI, 2, 0.2f, 6.0f, zsize + 9, centro);
	}

	void Render(Shader* shader) const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureBase);

		glBindVertexArray(RoofBaseVAO);
		shader->setMat4("Model", ModelBase);
		glDrawElements(GL_TRIANGLES, RoofBaseTI, GL_UNSIGNED_INT, nullptr);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureRoof);

		glBindVertexArray(RoofEVAO);
		shader->setMat4("Model", ModelRoofE);
		glDrawElements(GL_TRIANGLES, RoofETI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(RoofDVAO);
		shader->setMat4("Model", ModelRoofD);
		glDrawElements(GL_TRIANGLES, RoofDTI, GL_UNSIGNED_INT, nullptr);
	}

	void Delete() const {
		glDeleteBuffers(1, &RoofBaseVAO);
		glDeleteBuffers(1, &RoofEVAO);
		glDeleteBuffers(1, &RoofDVAO);
	}
};

class Mesa {
public:
	glm::vec3 Centro;
	GLuint Texture;
	glm::mat4 Model;

	Bloco PernaSE;
	Bloco PernaSD;
	Bloco PernaIE;
	Bloco PernaID;

	Bloco Tampo;

	Mesa () {}

	Mesa(GLuint texturePerna, GLuint textureTampo, float xsize, float ysize, float zsize, glm::vec3 centro) {
		Tampo = Bloco(textureTampo, xsize, 0.1f, zsize, centro + glm::vec3(0, ysize / 2, 0));
		PernaSE = Bloco(texturePerna, 0.1, ysize, 0.1, centro + glm::vec3(-xsize / 2 + 0.1, 0, zsize / 2 - 0.1));
		PernaSD = Bloco(texturePerna, 0.1, ysize, 0.1, centro + glm::vec3(xsize / 2 - 0.1, 0, zsize / 2 - 0.1));
		PernaIE = Bloco(texturePerna, 0.1, ysize, 0.1, centro + glm::vec3(-xsize / 2 + 0.1, 0, -zsize / 2 + 0.1));
		PernaID = Bloco(texturePerna, 0.1, ysize, 0.1, centro + glm::vec3(xsize / 2 - 0.1, 0, -zsize / 2 + 0.1));
	}

	void Render(Shader* shader) const {
		Tampo.Render(shader);
		PernaSE.Render(shader);
		PernaSD.Render(shader);
		PernaIE.Render(shader);
		PernaID.Render(shader);

	}

	void Delete() const {
		Tampo.Delete();
		PernaSE.Delete();
		PernaSD.Delete();
		PernaIE.Delete();
		PernaID.Delete();
	}
};

class Cadeira {
public:
	glm::vec3 Centro;
	GLuint Texture;
	glm::mat4 Model;

	Bloco PernaSE;
	Bloco PernaSD;
	Bloco PernaIE;
	Bloco PernaID;

	Bloco TampoH;
	Bloco TampoV;

	Cadeira () {}

	Cadeira(GLuint texturePerna, GLuint textureTampo, float xsize, float ysize, float zsize, glm::vec3 centro, int posicaoTampo) {
		TampoH = Bloco(textureTampo, xsize, 0.1f, zsize, centro);
		PernaSE = Bloco(texturePerna, 0.1, ysize / 2, 0.1, centro + glm::vec3(-xsize / 2 + 0.1, -ysize / 4, zsize / 2 - 0.1));
		PernaSD = Bloco(texturePerna, 0.1, ysize / 2, 0.1, centro + glm::vec3(xsize / 2 - 0.1, -ysize / 4, zsize / 2 - 0.1));
		PernaIE = Bloco(texturePerna, 0.1, ysize / 2, 0.1, centro + glm::vec3(-xsize / 2 + 0.1, -ysize / 4, -zsize / 2 + 0.1));
		PernaID = Bloco(texturePerna, 0.1, ysize / 2, 0.1, centro + glm::vec3(xsize / 2 - 0.1, -ysize / 4, -zsize / 2 + 0.1));

		if (posicaoTampo == 0) {
			TampoV = Bloco(textureTampo, 0.1, ysize / 2, zsize, centro + glm::vec3(xsize / 2 - 0.05, ysize / 4, 0));
		}
		else if (posicaoTampo == 1) {
			TampoV = Bloco(textureTampo, 0.1, ysize / 2, zsize, centro + glm::vec3(-xsize / 2 + 0.05, ysize / 4, 0));
		}
		else if (posicaoTampo == 2) {
			TampoV = Bloco(textureTampo, xsize, ysize / 2, 0.1, centro + glm::vec3(0, ysize / 4, zsize / 2 - 0.05));
		}
		else if (posicaoTampo == 3) {
			TampoV = Bloco(textureTampo, xsize, ysize / 2, 0.1, centro + glm::vec3(0, ysize / 4, -zsize / 2 + 0.05));
		}
	}

	void Render(Shader* shader) const {
		TampoH.Render(shader);
		TampoV.Render(shader);
		PernaSE.Render(shader);
		PernaSD.Render(shader);
		PernaIE.Render(shader);
		PernaID.Render(shader);

	}

	void Delete() const {
		TampoH.Delete();
		TampoV.Delete();
		PernaSE.Delete();
		PernaSD.Delete();
		PernaIE.Delete();
		PernaID.Delete();
	}
};

class Lanterna {
public:
	Lanterna() {}

	Bloco PernaSE;
	Bloco PernaSD;
	Bloco PernaIE;
	Bloco PernaID;

	Bloco TampoS;
	Bloco TampoI;

	Bloco Central;

	Lanterna(GLuint textureCentro, GLuint textureCaixinha, float xsize, float ysize, float zsize, glm::vec3 centro) {
		TampoS = Bloco(textureCaixinha, xsize, 0.1f, zsize, centro + glm::vec3(0, ysize / 2, 0));
		TampoI = Bloco(textureCaixinha, xsize, 0.1f, zsize, centro + glm::vec3(0, -ysize / 2, 0));
		Central = Bloco(textureCentro, xsize * 0.7, ysize, zsize * 0.7, centro);
		PernaSE = Bloco(textureCaixinha, 0.1, ysize, 0.1, centro + glm::vec3(-xsize / 2 + 0.1, 0, zsize / 2 - 0.1));
		PernaSD = Bloco(textureCaixinha, 0.1, ysize, 0.1, centro + glm::vec3(xsize / 2 - 0.1, 0, zsize / 2 - 0.1));
		PernaIE = Bloco(textureCaixinha, 0.1, ysize, 0.1, centro + glm::vec3(-xsize / 2 + 0.1, 0, -zsize / 2 + 0.1));
		PernaID = Bloco(textureCaixinha, 0.1, ysize, 0.1, centro + glm::vec3(xsize / 2 - 0.1, 0, -zsize / 2 + 0.1));
	}

	void Render(Shader* shader) const {
		TampoS.Render(shader);
		TampoI.Render(shader);
		PernaSE.Render(shader);
		PernaSD.Render(shader);
		PernaIE.Render(shader);
		PernaID.Render(shader);
		Central.Render(shader);
	}

	void Delete() const {
		TampoS.Delete();
		TampoI.Delete();
		Central.Delete();
		PernaSE.Delete();
		PernaSD.Delete();
		PernaIE.Delete();
		PernaID.Delete();
	}
};

class LanternaBola {
public:
	LanternaBola() {}

	Bloco PernaSE;
	Bloco PernaSD;
	Bloco PernaIE;
	Bloco PernaID;

	Bloco TampoS;
	Bloco TampoI;

	glm::vec3 Centro;
	GLuint Texture;
	glm::mat4 Model;

	GLuint CentralTV;
	GLuint CentralTI;
	GLuint CentralVAO;

	LanternaBola(GLuint textureCentro, GLuint textureCaixinha, float xsize, float ysize, float zsize, glm::vec3 centro) {
		TampoS = Bloco(textureCaixinha, xsize, 0.1f, zsize, centro + glm::vec3(0, ysize / 2, 0));
		TampoI = Bloco(textureCaixinha, xsize, 0.1f, zsize, centro + glm::vec3(0, -ysize / 2, 0));
		PernaSE = Bloco(textureCaixinha, 0.1, ysize, 0.1, centro + glm::vec3(-xsize / 2 + 0.1, 0, zsize / 2 - 0.1));
		PernaSD = Bloco(textureCaixinha, 0.1, ysize, 0.1, centro + glm::vec3(xsize / 2 - 0.1, 0, zsize / 2 - 0.1));
		PernaIE = Bloco(textureCaixinha, 0.1, ysize, 0.1, centro + glm::vec3(-xsize / 2 + 0.1, 0, -zsize / 2 + 0.1));
		PernaID = Bloco(textureCaixinha, 0.1, ysize, 0.1, centro + glm::vec3(xsize / 2 - 0.1, 0, -zsize / 2 + 0.1));

		Centro = centro;
		Texture = textureCentro;
		Model = glm::mat4(1);
		Model = glm::translate(Model, centro);
		Model = glm::rotate(Model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::translate(Model, -centro);
		CentralTV = 0;
		CentralTI = 0;
		CentralVAO = CarregaEsfera(CentralTV, CentralTI, ysize * 0.4, centro);
	}

	void Render(Shader* shader) const {
		TampoS.Render(shader);
		TampoI.Render(shader);
		PernaSE.Render(shader);
		PernaSD.Render(shader);
		PernaIE.Render(shader);
		PernaID.Render(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBindVertexArray(CentralVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, CentralTI, GL_UNSIGNED_INT, nullptr);
	}

	void Delete() const {
		TampoS.Delete();
		TampoI.Delete();
		PernaSE.Delete();
		PernaSD.Delete();
		PernaIE.Delete();
		PernaID.Delete();

		glDeleteBuffers(1, &CentralVAO);
	}

	void Rotate(float angulo) {
		Model = glm::translate(Model, Centro);
		Model = glm::rotate(Model, glm::radians(angulo), glm::vec3(0.0f, 0.0f, 1.0f));
		Model = glm::translate(Model, -Centro);
	}
};

class Casa {
	ParedeJanela paredeT1;
	ParedeJanela paredeT2;
	ParedeJanela paredeT3;

	ParedeJanela paredeL1;
	ParedeJanela paredeL2;
	ParedeJanela paredeL3;
	ParedeJanela paredeL4;

	ParedeJanela paredeF1;
	ParedePorta paredeF2;
	ParedeJanela paredeF3;

	Bloco paredeD1;
	ParedePorta paredeD2;
	Bloco paredeD3;

	ParedePorta paredeC1;
	ParedePorta paredeC2;

	Roof roof;

	Floor teto;

	Floor chao;

	Floor passarela;

	Mesa mesaCentral;
	std::array<Cadeira, 6> cadeirasCentral;
	Lanterna lanternaCentral;

	Mesa mesaCorredor;

	Mesa mesaCE;
	Cadeira cadeiraCE;
	Lanterna lanternaCE;

	Mesa mesaCD;
	Cadeira cadeiraCD1;
	Cadeira cadeiraCD2;
	LanternaBola lanternaMundo;

public:
	Casa() {}

	Casa(GLuint textWallId, GLuint textRoofId, GLuint textFdId, GLuint textPvId, GLuint textMId, GLuint textLId, GLuint textLBId, GLuint textLMId) {
		// Parede de trás
		paredeT1 = ParedeJanela(textWallId, 4.3f, 3.5f, 0.3f, 2.0f, 1.5f, 0, glm::vec3(3.0f, 0.35f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		paredeT2 = ParedeJanela(textWallId, 4.3f, 3.5f, 0.3f, 2.0f, 1.5f, 0, glm::vec3(-3.0f, 0.35f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		paredeT3 = ParedeJanela(textWallId, 1.7f, 3.5f, 0.3f, 1.0f, 1.5f, 0, glm::vec3(0.0f, 0.35f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		// Paredes Laterais
		paredeL1= ParedeJanela(textWallId, 12.0f, 3.5f, 0.3f, 3.0f, 1.0f, -90, glm::vec3(-5.0f, 0.35f, 4.0f), glm::vec3(4.0f, -0.25f, 0.0f));
		paredeL2 = ParedeJanela(textWallId, 12.0f, 3.5f, 0.3f, 3.0f, 1.0f, -90, glm::vec3(5.0f, 0.35f, 4.0f), glm::vec3(4.0f, -0.25f, 0.0f));
		paredeL3 = ParedeJanela(textWallId, 8.0f, 3.5f, 0.3f, 3.0f, 1.0f, -90, glm::vec3(-5.0f, 0.35f, -6.0f), glm::vec3(0.0f, -0.25f, 0.0f));
		paredeL4 = ParedeJanela(textWallId, 8.0f, 3.5f, 0.3f, 3.0f, 1.0f, -90, glm::vec3(5.0f, 0.35f, -6.0f), glm::vec3(0.0f, -0.25f, 0.0f));

		// Parede da Frente
		paredeF1 = ParedeJanela(textWallId, 4.3f, 3.5f, 0.3f, 1.0f, 1.5f, 0, glm::vec3(3.0f, 0.35f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		paredeF2 = ParedePorta(textWallId, 1.7f, 3.5f, 0.3f, 1.0f, 2.0f, 0, glm::vec3(0.0f, 0.35f, 10.0f), glm::vec3(0.0f, 0.75f, 0.0f));
		paredeF3 = ParedeJanela(textWallId, 4.3f, 3.5f, 0.3f, 1.0f, 1.5f, 0, glm::vec3(-3.0f, 0.35f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		// Parede que divisa o corredor da sala principal
		paredeD1 = Bloco(textWallId, 4.0f, 3.5f, 0.3f, glm::vec3(3.0f, 0.35f, -2.0f));
		paredeD2 = ParedePorta(textWallId, 2.0f, 3.5f, 0.3f, 1.0f, 2.0f, 0, glm::vec3(0.0f, 0.35f, -2.0f), glm::vec3(0.0f, 0.75f, 0.0f));
		paredeD3 = Bloco(textWallId, 4.0f, 3.5f, 0.3f, glm::vec3(-3.0f, 0.35f, -2.0f));

		// Paredes do corredor
		paredeC1 = ParedePorta(textWallId, 8.0f, 3.5f, 0.3f, 1.0f, 2.0f, -90, glm::vec3(1.0f, 0.35f, -6.0f), glm::vec3(2.5f, 0.75f, 0.0f));
		paredeC2 = ParedePorta(textWallId, 8.0f, 3.5f, 0.3f, 1.0f, 2.0f, -90, glm::vec3(-1.0f, 0.35f, -6.0f), glm::vec3(2.5f, 0.75f, 0.0f));

		// Telhado
		roof = Roof(textWallId, textRoofId, 2.0f, 20.3f, 11.8f, glm::vec3(0, 2.6, 0));

		// Chão da casa
		chao = Floor(textFdId, 10.3f, 20.3f, 0.1f, 4, glm::vec3(0, -1.45, 0), 90.0f);

		// Passarela para casa
		passarela = Floor(textPvId, 3.0f, 10.0f, 0.1f, 3, glm::vec3(0, -1.45, 15.15), 90.0f);

		// Comodo central
		mesaCentral = Mesa(textMId, textMId, 3, 1, 6, glm::vec3(0, -0.9, 4));
		cadeirasCentral[0] = Cadeira(textMId, textMId, 0.75, 1, 1, glm::vec3(1.75, -0.9, 2), 0);
		cadeirasCentral[1] = Cadeira(textMId, textMId, 0.75, 1, 1, glm::vec3(1.75, -0.9, 4), 0);
		cadeirasCentral[2] = Cadeira(textMId, textMId, 0.75, 1, 1, glm::vec3(1.75, -0.9, 6), 0);
		cadeirasCentral[3] = Cadeira(textMId, textMId, 0.75, 1, 1, glm::vec3(-1.75, -0.9, 2), 1);
		cadeirasCentral[4] = Cadeira(textMId, textMId, 0.75, 1, 1, glm::vec3(-1.75, -0.9, 4), 1);
		cadeirasCentral[5] = Cadeira(textMId, textMId, 0.75, 1, 1, glm::vec3(-1.75, -0.9, 6), 1);
		lanternaCentral = Lanterna(textLId, textLBId, 0.5, 0.6, 0.5, glm::vec3(0, 0, 4));

		// Teto completo
		teto = Floor(textWallId, 10.0f, 20.0f, 0.1f, 2, glm::vec3(0, 2.0, 0), 90.0f);

		// Corredor
		mesaCorredor = Mesa(textMId, textMId, 1, 1, 0.5, glm::vec3(0, -0.9, -9.6));

		// Comodo Esquerdo
		mesaCE = Mesa(textMId, textMId, 1.25, 1, 2.8, glm::vec3(-4.2, -0.9, -6));
		cadeiraCE = Cadeira(textMId, textMId, 0.75, 1, 1, glm::vec3(-3.2, -0.9, -6), 0);
		lanternaCE = Lanterna(textLId, textLBId, 0.5, 0.6, 0.5, glm::vec3(-4.5, 0, -4.9));

		// Corredor Direito
		mesaCD = Mesa(textMId, textMId, 1.5, 1, 2.5, glm::vec3(3, -0.9, -7));
		cadeiraCD1 = Cadeira(textMId, textMId, 1, 1, 0.75, glm::vec3(3, -0.9, -5.5), 2);
		cadeiraCD2 = Cadeira(textMId, textMId, 1, 1, 0.75, glm::vec3(3, -0.9, -8.5), 3);
		lanternaMundo = LanternaBola(textLMId, textLBId, 0.75, 0.75, 0.75, glm::vec3(3, 0.075, -7));
	}

	void Render(Shader* shader) const {
		// Paredes de Trás
		paredeT1.Render(shader);
		paredeT2.Render(shader);
		paredeT3.Render(shader);

		// Paredes da Frente
		paredeF1.Render(shader);
		paredeF2.Render(shader);
		paredeF3.Render(shader);

		// Paredes Laterais
		paredeL1.Render(shader);
		paredeL2.Render(shader);
		paredeL3.Render(shader);
		paredeL4.Render(shader);

		// Parede divisória
		paredeD1.Render(shader);
		paredeD2.Render(shader);
		paredeD3.Render(shader);

		// Paredes corredor
		paredeC1.Render(shader);
		paredeC2.Render(shader);

		// Telhado
		roof.Render(shader);

		// Chão da casa
		chao.Render(shader);

		// Passarela para casa
		passarela.Render(shader);

		// Comodo central
		mesaCentral.Render(shader);
		for (int i = 0; i < 6; i++) {
			cadeirasCentral[i].Render(shader);
		}
		lanternaCentral.Render(shader);

		// Teto completo
		teto.Render(shader);

		// Corredor
		mesaCorredor.Render(shader);

		// Corredor
		mesaCE.Render(shader);
		cadeiraCE.Render(shader);
		lanternaCE.Render(shader);

		// Corredor
		mesaCD.Render(shader);
		cadeiraCD1.Render(shader);
		cadeiraCD2.Render(shader);
		lanternaMundo.Render(shader);
	}

	void Delete() const {
		// Paredes de Trás
		paredeT1.Delete();
		paredeT2.Delete();
		paredeT3.Delete();

		// Paredes da Frente
		paredeF1.Delete();
		paredeF2.Delete();
		paredeF3.Delete();

		// Paredes Laterais
		paredeL1.Delete();
		paredeL2.Delete();
		paredeL3.Delete();
		paredeL4.Delete();

		// Parede divisória
		paredeD1.Delete();
		paredeD2.Delete();
		paredeD3.Delete();

		// Paredes corredor
		paredeC1.Delete();
		paredeC2.Delete();

		// Telhado
		roof.Delete();

		// Chão da casa
		chao.Delete();

		// Passarela para casa
		passarela.Delete();

		// Comodo central
		mesaCentral.Delete();
		for (int i = 0; i < 6; i++) {
			cadeirasCentral[i].Delete();
		}

		// Teto completo
		teto.Delete();

		// Corredor
		mesaCorredor.Delete();

		// Corredor
		mesaCE.Delete();
		cadeiraCE.Delete();
		lanternaCE.Delete();

		// Corredor
		mesaCD.Delete();
		cadeiraCD1.Delete();
		cadeiraCD2.Delete();
		lanternaMundo.Delete();
	}

	void Rotate(float angulo) {
		lanternaMundo.Rotate(angulo);
	}
};

class Slenderman {
public:
	Bloco PE;
	Bloco PD;
	Bloco Corpo;
	Bloco BE;
	Bloco BD;
	Bloco Cabeca;

	Slenderman(){}

	Slenderman(GLuint textureCorpo, GLuint textureCabeca, glm::vec3 centro) {
		Corpo = Bloco(textureCorpo, 0.7, 1.0f, 0.15, centro);
		PE = Bloco(textureCorpo, 0.1, 1.0f, 0.1, centro + glm::vec3(-0.3, -1, 0));
		PD = Bloco(textureCorpo, 0.1, 1.0f, 0.1, centro + glm::vec3(0.3, -1, 0));
		BE = Bloco(textureCorpo, 0.1, 1.0, 0.1, centro + glm::vec3(-0.35, 0.0, 0));
		BD = Bloco(textureCorpo, 0.1, 1.0, 0.1, centro + glm::vec3(0.35, 0.0, 0));
		Cabeca = Bloco(textureCabeca, 0.3, 0.3, 0.3, centro + glm::vec3(0, 0.65, 0));

		BE.Rotate(-15);
		BD.Rotate(25);
	}

	void Render(Shader* shader) const {
		Corpo.Render(shader);
		PE.Render(shader);
		PD.Render(shader);
		BE.Render(shader);
		BD.Render(shader);
		Cabeca.Render(shader);
	}

	void Delete() const {
		Corpo.Delete();
		PD.Delete();
		PE.Delete();
		BE.Delete();
		BD.Delete();
		Cabeca.Delete();
	}

};

class Moon {
public:
	glm::vec3 Centro;
	GLuint Texture;
	glm::mat4 Model;

	GLuint MoonTV;
	GLuint MoonTI;
	GLuint MoonVAO;

	Moon () {}

	Moon(GLuint texture, glm::vec3 centro) {
		Model = glm::mat4(1.0f);
		Model = glm::translate(Model, centro);
		Model = glm::rotate(Model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::translate(Model, -centro);

		Texture = texture;
		Centro = centro;

		MoonTV = 0;
		MoonTI = 0;
		MoonVAO = CarregaEsfera(MoonTV, MoonTI, 1.0f, Centro);
	}

	void Render(Shader* shader) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBindVertexArray(MoonVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, MoonTI, GL_UNSIGNED_INT, nullptr);

	}
};

#endif // !OBJETOS_H
