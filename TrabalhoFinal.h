// TrabalhoFinal.h: arquivo de inclusão para arquivos de inclusão padrão do sistema,
// ou arquivos de inclusão específicos a um projeto.

#pragma once

#include "Light.h"
#include "Camera.h"
#include "Objetos.h"

#include <array>
#include <cassert>
#include <random>
#include <cstdlib> 
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern int Width;
extern int Height;

std::string LerArquivo(const char* caminho);

GLuint CarregarTextura(const char* caminhoTextura);

struct LuzDirecional {
	glm::vec3 Direcao;
	GLfloat Intensidade;
};

// TODO: Referencie os cabeçalhos adicionais de que seu programa precisa aqui.
