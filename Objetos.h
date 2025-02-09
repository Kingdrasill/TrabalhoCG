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
		FloorVAO = CarregaQuadrilatero(FloorTV, FloorTI, 100, xsize, ysize, zsize, Centro);
	}

	void Render(Shader* shader) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBindVertexArray(FloorVAO);
		shader->setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, FloorTI, GL_UNSIGNED_INT, nullptr);
	}
};

#endif // !OBJETOS_H
