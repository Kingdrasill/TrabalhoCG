#include "TrabalhoFinal.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLuint CarregarTextura(const char* caminhoTextura) {
	std::cout << "Carregando textura: " << caminhoTextura << std::endl;
	
	GLuint TexturaId;
	glGenTextures(1, &TexturaId);
	glBindTexture(GL_TEXTURE_2D, TexturaId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int LarguraImagem = 0;
	int AlturaImagem = 0;
	int NumeroDeComponentes = 0;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* DadosTextura = stbi_load(
		caminhoTextura,
		&LarguraImagem,
		&AlturaImagem,
		&NumeroDeComponentes,
		3);
	assert(DadosTextura);

	if (DadosTextura) {
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB,
			LarguraImagem,
			AlturaImagem,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			DadosTextura);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Erro ao carregar a textura";
	}
	stbi_image_free(DadosTextura);
	glBindTexture(GL_TEXTURE_2D, 0);

	return TexturaId;
}