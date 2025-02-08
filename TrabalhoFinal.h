// TrabalhoFinal.h: arquivo de inclusão para arquivos de inclusão padrão do sistema,
// ou arquivos de inclusão específicos a um projeto.

#pragma once

#include <iostream>
#include <array>
#include <cassert>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

extern int Width;
extern int Height;

std::string LerArquivo(const char* caminho);

void CheckShader(GLuint ShaderId);
GLuint LoadShaders(const char* VertexShaderFile, const char* FragmentShaderFile);
GLint CarregarTextura(const char* caminhoTextura);

struct Vertice {
	glm::vec3 Posicao;
	glm::vec3 Normal;
	glm::vec3 Cor;
	glm::vec2 UV;
};

struct LuzDirecional {
	glm::vec3 Direcao;
	GLfloat Intensidade;
};

GLuint CarregaGeometria();
void GerarMalhaEsfera(GLuint resolucao, float raio, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices);
GLuint CarregaEsfera(GLuint& TotalVertices, GLuint& TotalIndices, float raio, glm::vec3 Centro);
void GerarMalhaCilindro(GLuint resolucao, float altura, float raio, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices);
GLuint CarregaCilindro(GLuint& TotalVertices, GLuint& TotalIndices, float altura, float raio, glm::vec3 Centro);

class FlyCamera {
public:
	void MoverParaFrente(float quantidade);
	void MoverParaDireita(float quantidade);
	void OlharPara(float quantidadeEixoYyaw, float quantidadeEixoXpitch);
	
	glm::mat4 GetView() const {
		return glm::lookAt(Location, Location + Direction, Up);
	}
	
	glm::mat4 GetViewProjection() const {
		glm::mat4 Projection = glm::perspective(FieldOfView, AspectRatio, Near, Far);
		return Projection * GetView();
	}

	//Parametros de Interatividade, define a velocidade do movimento
	float Speed = 1;
	float Sensitivity = 0.1f;

	//Parametros da Matriz de view
	glm::vec3 Location{ 0.0f, 0.0f, 5.0f };
	glm::vec3 Direction{ 0.0f, 0.0f, -1.0f };
	glm::vec3 Up{ 0.0f, 1.0f, 0.0f };

	//Parâmetros da Matriz Projection
	float FieldOfView = glm::radians(45.0f);
	float AspectRatio = Width / Height;
	float Near = 0.01f;
	float Far = 1000.0f;
};

// TODO: Referencie os cabeçalhos adicionais de que seu programa precisa aqui.
