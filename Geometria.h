#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

struct Vertice {
	glm::vec3 Posicao;
	glm::vec3 Normal;
	glm::vec3 Cor;
	glm::vec2 UV;
};

void GerarMalhaQuadrilatero(GLuint resolucao, float repeat, float xsize, float ysize, float zsize, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices);

GLuint CarregaQuadrilatero(GLuint& TotalVertices, GLuint& TotalIndices, float repeat, float xsize, float ysize, float zsize, glm::vec3 Centro);

void GerarMalhaEsfera(GLuint resolucao, float raio, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices);

GLuint CarregaEsfera(GLuint& TotalVertices, GLuint& TotalIndices, float raio, glm::vec3 Centro);

void GerarMalhaCilindro(GLuint resolucao, GLuint numCamadas, float altura, float raio, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices);

GLuint CarregaCilindro(GLuint& TotalVertices, GLuint& TotalIndices, GLuint numCamadas, float altura, float raio, glm::vec3 Centro);

void GerarMalhaEscada(GLuint numdegrau, float xdegrau, float ydegrau, float zdegrau, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices);

GLuint CarregaEscada(GLuint& TotalVertices, GLuint& TotalIndices, GLuint numdegrau, float xdegrau, float ydegrau, float zdegrau, glm::vec3 Centro);

void GerarMalhaCone(GLuint resolucao, float altura, float raio, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices);

GLuint CarregaCone(GLuint& TotalVertices, GLuint& TotalIndices, float altura, float raio, glm::vec3 Centro);

#endif