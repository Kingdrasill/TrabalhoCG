#include "TrabalhoFinal.h"

void FlyCamera::MoverParaFrente(float quantidade) {
	this->Location += glm::normalize(Direction) * quantidade * Speed;
}

void FlyCamera::MoverParaDireita(float quantidade) {
	//Calcula o produto vetorial
	glm::vec3 Right = glm::normalize(glm::cross(Direction, Up));

	this->Location += Right * quantidade * Speed;
}

void FlyCamera::OlharPara(float quantidadeEixoYyaw, float quantidadeEixoXpitch) {
	// Aplicando a velocidade do movimento
	quantidadeEixoYyaw *= Sensitivity;
	quantidadeEixoXpitch *= Sensitivity;

	// Matriz identidade
	const glm::mat4 MatrizIdentidade = glm::identity<glm::mat4>();
	// Produto vetorial
	glm::vec3 Right = glm::normalize(glm::cross(Direction, Up));

	// Criando as matrizes de rotação
	glm::mat4 MatrizRotacaoExioY = glm::rotate(MatrizIdentidade,
		glm::radians(quantidadeEixoYyaw),
		Up);
	glm::mat4 MatrizRotacaoExioX = glm::rotate(MatrizIdentidade,
		glm::radians(quantidadeEixoXpitch),
		Right);

	// Plicando nos vetores de direção
	this->Up = MatrizRotacaoExioX * glm::vec4{ this->Up , 0.0f };
	this->Direction = MatrizRotacaoExioY * MatrizRotacaoExioX * glm::vec4{ this->Direction , 0.0f };
}
