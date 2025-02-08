// TrabalhoFinal.h: arquivo de inclusão para arquivos de inclusão padrão do sistema,
// ou arquivos de inclusão específicos a um projeto.

#pragma once

#include "Shader.h"
#include "Camera.h"

#include <array>
#include <cassert>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

extern int Width;
extern int Height;

std::string LerArquivo(const char* caminho);

GLuint CarregarTextura(const char* caminhoTextura);

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
void GerarMalhaCilindro(GLuint resolucao, GLuint numCamadas, float altura, float raio, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices);
GLuint CarregaCilindro(GLuint& TotalVertices, GLuint& TotalIndices, float altura, float raio, glm::vec3 Centro);

// TODO: Referencie os cabeçalhos adicionais de que seu programa precisa aqui.
