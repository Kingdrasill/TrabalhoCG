#include "Geometria.h"

void GerarMalhaQuadrilatero(GLuint resolucao, float repeat, float xsize, float ysize, float zsize, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices) {
	Vertices.clear();
	Indices.clear();

	struct Face {
		glm::vec3 normal;
		glm::vec3 offset;
		glm::vec3 right;
		glm::vec3 up;
	};

	std::vector<Face> faces = {
		{{0, 0, 1}, {0, 0, zsize / 2}, {xsize, 0, 0}, {0, ysize, 0}},  // Front
		{{0, 0, -1}, {0, 0, -zsize / 2}, {-xsize, 0, 0}, {0, ysize, 0}}, // Back
		{{1, 0, 0}, {xsize / 2, 0, 0}, {0, 0, -zsize}, {0, ysize, 0}},  // Right
		{{-1, 0, 0}, {-xsize / 2, 0, 0}, {0, 0, zsize}, {0, ysize, 0}}, // Left
		{{0, 1, 0}, {0, ysize / 2, 0}, {xsize, 0, 0}, {0, 0, -zsize}},  // Top
		{{0, -1, 0}, {0, -ysize / 2, 0}, {xsize, 0, 0}, {0, 0, zsize}}, // Bottom
	};

	GLuint vertexOffset = 0;

	for (const auto& face : faces) {
		const float dx = 1.0f / (resolucao - 1);
		const float dy = 1.0f / (resolucao - 1);

		for (GLuint i = 0; i < resolucao; ++i) {
			for (GLuint j = 0; j < resolucao; ++j) {
				glm::vec3 posicao = Centro + face.offset
					+ (i * dx - 0.5f) * face.right
					+ (j * dy - 0.5f) * face.up;

				glm::vec2 texCoord = glm::vec2(i * dx * repeat, j * dy * repeat);

				Vertices.push_back({ posicao, face.normal, glm::vec3(1.0f), texCoord });
			}
		}

		for (GLuint i = 0; i < resolucao - 1; ++i) {
			for (GLuint j = 0; j < resolucao - 1; ++j) {
				GLuint P0 = vertexOffset + i + j * resolucao;
				GLuint P1 = vertexOffset + (i + 1) + j * resolucao;
				GLuint P2 = vertexOffset + (i + 1) + (j + 1) * resolucao;
				GLuint P3 = vertexOffset + i + (j + 1) * resolucao;

				Indices.push_back(glm::ivec3{ P0, P1, P3 });
				Indices.push_back(glm::ivec3{ P3, P1, P2 });
			}
		}

		vertexOffset += resolucao * resolucao;
	}
}

GLuint CarregaQuadrilatero(GLuint& TotalVertices, GLuint& TotalIndices, float repeat, float xsize, float ysize, float zsize, glm::vec3 Centro) {
	std::vector<Vertice> Vertices;
	std::vector<glm::ivec3> Triangulos;

	GerarMalhaQuadrilatero(10, repeat, xsize, ysize, zsize, Centro, Vertices, Triangulos);

	TotalVertices = Vertices.size();
	TotalIndices = Triangulos.size() * 3;

	GLuint VertexBuffer;
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	GLsizeiptr size = Vertices.size() * sizeof(Vertice);
	glBufferData(GL_ARRAY_BUFFER, size, Vertices.data(), GL_STATIC_DRAW);

	GLuint ElementBuffer;
	glGenBuffers(1, &ElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	GLsizeiptr sizeIndice = TotalIndices * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndice, Triangulos.data(), GL_STATIC_DRAW);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice),
		nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Normal)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Cor)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, UV)));

	glBindVertexArray(0);

	return VAO;
}

void GerarMalhaEsfera(GLuint resolucao, float raio, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices) {
	Vertices.clear();
	Indices.clear();

	constexpr float Pi = glm::pi<float>();
	constexpr float DoisPi = glm::two_pi<float>();

	const float InversoResolucao = 1.0f / static_cast<float>(resolucao - 1);

	for (GLuint UIndice = 0; UIndice < resolucao; ++UIndice) {
		const float U = UIndice * InversoResolucao;
		const float Phi = glm::mix(0.0f, DoisPi, U);

		for (GLuint VIndice = 0; VIndice < resolucao; ++VIndice) {
			const float V = VIndice * InversoResolucao;
			const float Theta = glm::mix(0.0f, Pi, V);

			glm::vec3 PosicaoVertice = Centro + raio * glm::vec3{
				glm::sin(Theta) * glm::cos(Phi),
				glm::sin(Theta) * glm::sin(Phi),
				glm::cos(Theta),
			};

			Vertice Vertice{
				PosicaoVertice,
				glm::normalize(PosicaoVertice - Centro),
				glm::vec3{1.0f, 1.0f, 1.0f},
				glm::vec2{1.0f - U, V}
			};

			Vertices.push_back(Vertice);
		}
	}

	for (GLuint U = 0; U < resolucao - 1; ++U) {
		for (GLuint V = 0; V < resolucao - 1; ++V) {
			GLuint P0 = U + V * resolucao;
			GLuint P1 = (U + 1) + V * resolucao;
			GLuint P2 = (U + 1) + (V + 1) * resolucao;
			GLuint P3 = U + (V + 1) * resolucao;

			Indices.push_back(glm::ivec3{ P0, P1, P3 });
			Indices.push_back(glm::ivec3{ P3, P1, P2 });
		}
	}
}

GLuint CarregaEsfera(GLuint& TotalVertices, GLuint& TotalIndices, float raio, glm::vec3 Centro) {
	std::vector<Vertice> Vertices;
	std::vector<glm::ivec3> Triangulos;

	GerarMalhaEsfera(50, raio, Centro, Vertices, Triangulos);

	TotalVertices = Vertices.size();
	TotalIndices = Triangulos.size() * 3;

	GLuint VertexBuffer;
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	GLsizeiptr size = Vertices.size() * sizeof(Vertice);
	glBufferData(GL_ARRAY_BUFFER, size, Vertices.data(), GL_STATIC_DRAW);

	GLuint ElementBuffer;
	glGenBuffers(1, &ElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	GLsizeiptr sizeIndice = TotalIndices * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndice, Triangulos.data(), GL_STATIC_DRAW);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice),
		nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Normal)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Cor)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, UV)));

	glBindVertexArray(0);

	return VAO;
}

void GerarMalhaCilindro(GLuint resolucao, GLuint numCamadas, float altura, float raio, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices) {
	Vertices.clear();
	Indices.clear();

	constexpr float DoisPi = glm::two_pi<float>();
	const float InversoResolucao = 1.0f / static_cast<float>(resolucao);
	const float InversoCamadas = 1.0f / static_cast<float>(numCamadas - 1);  // Normalize heights

	// Generate cylinder vertices in multiple layers
	for (GLuint j = 0; j < numCamadas; ++j) {
		float alturaAtual = -altura / 2.0f + j * InversoCamadas * altura; // Interpolating heights
		float vTexCoord = static_cast<float>(j) * InversoCamadas;

		for (GLuint i = 0; i <= resolucao; ++i) {
			float angulo = i * InversoResolucao * DoisPi;
			float x = raio * glm::cos(angulo);
			float z = raio * glm::sin(angulo);

			glm::vec3 normal = glm::normalize(glm::vec3{ x, 0.0f, z });

			// Adding vertices in multiple layers
			Vertices.push_back(Vertice{
				Centro + glm::vec3{x, alturaAtual, z},
				normal,
				glm::vec3{1.0f, 1.0f, 1.0f},
				glm::vec2{1.0f - i * InversoResolucao, vTexCoord}
				});
		}
	}

	// Indices for side faces (connecting vertical layers)
	for (GLuint j = 0; j < numCamadas - 1; ++j) {
		for (GLuint i = 0; i < resolucao; ++i) {
			GLuint p0 = j * (resolucao + 1) + i;
			GLuint p1 = p0 + 1;
			GLuint p2 = p0 + (resolucao + 1);
			GLuint p3 = p2 + 1;

			// Ensure correct face orientation (CCW order)
			Indices.push_back(glm::ivec3{ p0, p2, p3 });
			Indices.push_back(glm::ivec3{ p0, p3, p1 });
		}
	}

	// === Add center vertices for the base and top ===
	GLuint topoIndex = Vertices.size();
	Vertices.push_back(Vertice{ Centro + glm::vec3{0.0f, altura / 2.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec2{0.5f, 1.0f} });

	GLuint baseIndex = Vertices.size();
	Vertices.push_back(Vertice{ Centro + glm::vec3{0.0f, -altura / 2.0f, 0.0f}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec2{0.5f, 0.0f} });

	// === Generate top and bottom faces ===
	for (GLuint i = 0; i < resolucao; ++i) {
		GLuint p0 = i;
		GLuint p1 = (i + 1) % resolucao;

		// Top face (counterclockwise order)
		GLuint topOffset = (numCamadas - 1) * (resolucao + 1);
		Indices.push_back(glm::ivec3{ topoIndex, topOffset + p1, topOffset + p0 });

		// Bottom face (clockwise order to match front face winding)
		Indices.push_back(glm::ivec3{ baseIndex, p0, p1 });
	}
}


GLuint CarregaCilindro(GLuint& TotalVertices, GLuint& TotalIndices, GLuint numCamadas, float altura, float raio, glm::vec3 Centro) {
	std::vector<Vertice> Vertices;
	std::vector<glm::ivec3> Triangulos;

	GerarMalhaCilindro(50, numCamadas, altura, raio, Centro, Vertices, Triangulos);

	TotalVertices = Vertices.size();
	TotalIndices = Triangulos.size() * 3;

	GLuint VertexBuffer;
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	GLsizeiptr size = Vertices.size() * sizeof(Vertice);
	glBufferData(GL_ARRAY_BUFFER, size, Vertices.data(), GL_STATIC_DRAW);

	GLuint ElementBuffer;
	glGenBuffers(1, &ElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	GLsizeiptr sizeIndice = TotalIndices * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndice, Triangulos.data(), GL_STATIC_DRAW);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice),
		nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Normal)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Cor)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, UV)));

	glBindVertexArray(0);

	return VAO;
}

void GerarMalhaEscada(GLuint numdegrau, float xdegrau, float ydegrau, float zdegrau, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices) {
	Vertices.clear();
	Indices.clear();
	GLuint vertexOffset = 0;

	for (GLuint step = 0; step < numdegrau; ++step) {
		float stepY = step * ydegrau;
		float stepZ = step * zdegrau;

		// Generate solid steps from ground (y=0) to current step height
		for (GLuint fillStep = 0; fillStep <= step; ++fillStep) {
			float fillStepY = fillStep * ydegrau;
			glm::vec3 stepCenter = Centro + glm::vec3(0, fillStepY, -stepZ);

			// Each block is a cuboid (six faces)
			struct Face {
				glm::vec3 normal;
				glm::vec3 offset;
				glm::vec3 right;
				glm::vec3 up;
			};

			std::vector<Face> faces = {
				{{0, 0, 1}, {0, 0, zdegrau / 2}, {xdegrau, 0, 0}, {0, ydegrau, 0}},  // Front
				{{0, 0, -1}, {0, 0, -zdegrau / 2}, {-xdegrau, 0, 0}, {0, ydegrau, 0}}, // Back
				{{1, 0, 0}, {xdegrau / 2, 0, 0}, {0, 0, -zdegrau}, {0, ydegrau, 0}},  // Right
				{{-1, 0, 0}, {-xdegrau / 2, 0, 0}, {0, 0, zdegrau}, {0, ydegrau, 0}}, // Left
				{{0, 1, 0}, {0, ydegrau / 2, 0}, {xdegrau, 0, 0}, {0, 0, -zdegrau}},  // Top
				{{0, -1, 0}, {0, -ydegrau / 2, 0}, {xdegrau, 0, 0}, {0, 0, zdegrau}}, // Bottom
			};

			for (const auto& face : faces) {
				float dx = 1.0f, dy = 1.0f;
				for (GLuint i = 0; i < 2; ++i) {
					for (GLuint j = 0; j < 2; ++j) {
						glm::vec3 posicao = stepCenter + face.offset
							+ (i * dx - 0.5f) * face.right
							+ (j * dy - 0.5f) * face.up;

						glm::vec2 texCoord = glm::vec2(i, j);
						Vertices.push_back({ posicao, face.normal, glm::vec3(1.0f), texCoord });
					}
				}

				// Generate indices for each face
				GLuint P0 = vertexOffset;
				GLuint P1 = vertexOffset + 1;
				GLuint P2 = vertexOffset + 3;
				GLuint P3 = vertexOffset + 2;

				Indices.push_back(glm::ivec3{ P0, P1, P3 });
				Indices.push_back(glm::ivec3{ P3, P1, P2 });

				vertexOffset += 4;
			}
		}
	}
}

GLuint CarregaEscada(GLuint& TotalVertices, GLuint& TotalIndices, GLuint numdegrau, float xdegrau, float ydegrau, float zdegrau, glm::vec3 Centro) {
	std::vector<Vertice> Vertices;
	std::vector<glm::ivec3> Triangulos;

	GerarMalhaEscada(numdegrau, xdegrau, ydegrau, zdegrau, Centro, Vertices, Triangulos);

	TotalVertices = Vertices.size();
	TotalIndices = Triangulos.size() * 3;

	GLuint VertexBuffer;
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	GLsizeiptr size = Vertices.size() * sizeof(Vertice);
	glBufferData(GL_ARRAY_BUFFER, size, Vertices.data(), GL_STATIC_DRAW);

	GLuint ElementBuffer;
	glGenBuffers(1, &ElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	GLsizeiptr sizeIndice = TotalIndices * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndice, Triangulos.data(), GL_STATIC_DRAW);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice),
		nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Normal)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Cor)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, UV)));

	glBindVertexArray(0);

	return VAO;
}

void GerarMalhaCone(GLuint resolucao, float altura, float raio, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices) {
	Vertices.clear();
	Indices.clear();

	constexpr float DoisPi = glm::two_pi<float>();
	const float InversoResolucao = 1.0f / static_cast<float>(resolucao);

	// === Gerar vértices ===

	// Vértice superior do cone
	GLuint topoIndex = 0;
	Vertices.push_back(Vertice{
		Centro + glm::vec3{0.0f, altura / 2.0f, 0.0f},  // Ponto no topo
		glm::normalize(glm::vec3{0.0f, 1.0f, 0.0f}),   // Normal aproximada
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec2{0.5f, 1.0f}
		});

	// Vértices da base circular
	for (GLuint i = 0; i < resolucao; ++i) {
		float angulo = i * InversoResolucao * DoisPi;
		float x = raio * glm::cos(angulo);
		float z = raio * glm::sin(angulo);

		glm::vec3 posicaoBase = Centro + glm::vec3{ x, -altura / 2.0f, z };
		glm::vec3 normalBase = glm::normalize(glm::vec3{ x, raio / altura, z }); // Aproximação da normal

		Vertices.push_back(Vertice{
			posicaoBase,
			normalBase,
			glm::vec3{1.0f, 1.0f, 1.0f},
			glm::vec2{1.0f - i * InversoResolucao, 0.0f}
			});
	}

	// Vértice central da base (para completar a base)
	GLuint baseCenterIndex = Vertices.size();
	Vertices.push_back(Vertice{
		Centro + glm::vec3{0.0f, -altura / 2.0f, 0.0f},
		glm::vec3{0.0f, -1.0f, 0.0f},  // Normal para baixo
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec2{0.5f, 0.0f}
		});

	// === Gerar índices ===

	// Conexões entre a base e o topo
	for (GLuint i = 0; i < resolucao; ++i) {
		GLuint p0 = i + 1;
		GLuint p1 = (i + 1) % resolucao + 1;
		Indices.push_back(glm::ivec3{ topoIndex, p1, p0 });  // Face lateral
	}

	// Conexões para a base
	for (GLuint i = 0; i < resolucao; ++i) {
		GLuint p0 = i + 1;
		GLuint p1 = (i + 1) % resolucao + 1;
		Indices.push_back(glm::ivec3{ baseCenterIndex, p0, p1 });  // Face da base
	}
}

GLuint CarregaCone(GLuint& TotalVertices, GLuint& TotalIndices, float altura, float raio, glm::vec3 Centro) {
	std::vector<Vertice> Vertices;
	std::vector<glm::ivec3> Triangulos;

	GerarMalhaCone(50, altura, raio, Centro, Vertices, Triangulos);

	TotalVertices = Vertices.size();
	TotalIndices = Triangulos.size() * 3;

	GLuint VertexBuffer;
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	GLsizeiptr size = Vertices.size() * sizeof(Vertice);
	glBufferData(GL_ARRAY_BUFFER, size, Vertices.data(), GL_STATIC_DRAW);

	GLuint ElementBuffer;
	glGenBuffers(1, &ElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	GLsizeiptr sizeIndice = TotalIndices * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndice, Triangulos.data(), GL_STATIC_DRAW);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice),
		nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Normal)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Cor)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, UV)));

	glBindVertexArray(0);

	return VAO;
}

void GerarMalhaSemiesfera(GLuint resolucao, float raio, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices) {
	Vertices.clear();
	Indices.clear();

	constexpr float Pi = glm::pi<float>();
	constexpr float DoisPi = glm::two_pi<float>();

	const float InversoResolucao = 1.0f / static_cast<float>(resolucao - 1);

	for (GLuint UIndice = 0; UIndice < resolucao; ++UIndice) {
		const float U = UIndice * InversoResolucao;
		const float Phi = glm::mix(0.0f, DoisPi, U); // Full 360-degree rotation

		for (GLuint VIndice = 0; VIndice < resolucao; ++VIndice) {
			const float V = VIndice * InversoResolucao;
			const float Theta = glm::mix(0.0f, Pi / 2, V); // Only top half (0 to Pi/2)

			glm::vec3 PosicaoVertice = Centro + raio * glm::vec3{
				glm::sin(Theta) * glm::cos(Phi),
				glm::sin(Theta) * glm::sin(Phi),
				glm::cos(Theta),
			};

			Vertice Vertice{
				PosicaoVertice,
				glm::normalize(PosicaoVertice - Centro),
				glm::vec3{1.0f, 1.0f, 1.0f},
				glm::vec2{1.0f - U, V}
			};

			Vertices.push_back(Vertice);
		}
	}

	for (GLuint U = 0; U < resolucao - 1; ++U) {
		for (GLuint V = 0; V < resolucao - 1; ++V) {
			GLuint P0 = U + V * resolucao;
			GLuint P1 = (U + 1) + V * resolucao;
			GLuint P2 = (U + 1) + (V + 1) * resolucao;
			GLuint P3 = U + (V + 1) * resolucao;

			Indices.push_back(glm::ivec3{ P0, P1, P3 });
			Indices.push_back(glm::ivec3{ P3, P1, P2 });
		}
	}
}

GLuint CarregarSemiesfera(GLuint& TotalVertices, GLuint& TotalIndices, float raio, glm::vec3 Centro) {
	std::vector<Vertice> Vertices;
	std::vector<glm::ivec3> Triangulos;

	GerarMalhaSemiesfera(50, raio, Centro, Vertices, Triangulos);

	TotalVertices = Vertices.size();
	TotalIndices = Triangulos.size() * 3;

	GLuint VertexBuffer;
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	GLsizeiptr size = Vertices.size() * sizeof(Vertice);
	glBufferData(GL_ARRAY_BUFFER, size, Vertices.data(), GL_STATIC_DRAW);

	GLuint ElementBuffer;
	glGenBuffers(1, &ElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	GLsizeiptr sizeIndice = TotalIndices * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndice, Triangulos.data(), GL_STATIC_DRAW);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice),
		nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Normal)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Cor)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, UV)));

	glBindVertexArray(0);

	return VAO;
}

void GerarMalhaPrisma(GLuint resolucao, float repeat, float xsize, float ysize, float zsize, GLuint numSides, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices) {
	Vertices.clear();
	Indices.clear();

	float angleStep = 2.0f * glm::pi<float>() / numSides;
	float halfHeight = ysize / 2.0f;
	float halfX = xsize / 2.0f;
	float halfZ = zsize / 2.0f;

	GLuint vertexOffset = 0;
	std::vector<std::vector<GLuint>> layerIndices;

	// Generate multiple layers along the height
	for (GLuint layer = 0; layer <= resolucao; ++layer) {
		float y = -halfHeight + (layer * (ysize / resolucao)); // Interpolated Y position
		std::vector<GLuint> currentLayer;

		for (GLuint i = 0; i < numSides; ++i) {
			float angle = i * angleStep;
			glm::vec3 offset = glm::vec3(halfX * cos(angle), y, halfZ * sin(angle)); // Scale based on xsize and zsize

			// Add vertex
			Vertices.push_back({ Centro + offset, glm::vec3(cos(angle), 0, sin(angle)), glm::vec3(1.0f), glm::vec2((cos(angle) + 1) / 2, (sin(angle) + 1) / 2) });
			currentLayer.push_back(vertexOffset++);
		}
		layerIndices.push_back(currentLayer);
	}

	// Create faces between layers
	for (GLuint layer = 0; layer < resolucao; ++layer) {
		for (GLuint i = 0; i < numSides; ++i) {
			GLuint next = (i + 1) % numSides;
			GLuint v1 = layerIndices[layer][i];
			GLuint v2 = layerIndices[layer][next];
			GLuint v3 = layerIndices[layer + 1][i];
			GLuint v4 = layerIndices[layer + 1][next];

			Indices.push_back(glm::ivec3(v1, v2, v3));
			Indices.push_back(glm::ivec3(v3, v2, v4));
		}
	}

	// Bottom and top center vertices
	GLuint baseCenterIndex = vertexOffset++;
	GLuint topCenterIndex = vertexOffset++;

	Vertices.push_back({ Centro + glm::vec3(0, -halfHeight, 0), glm::vec3(0, -1, 0), glm::vec3(1.0f), glm::vec2(0.5f, 0.5f) });
	Vertices.push_back({ Centro + glm::vec3(0, halfHeight, 0), glm::vec3(0, 1, 0), glm::vec3(1.0f), glm::vec2(0.5f, 0.5f) });

	// Create base and top faces
	for (GLuint i = 0; i < numSides; ++i) {
		GLuint next = (i + 1) % numSides;
		Indices.push_back(glm::ivec3(baseCenterIndex, layerIndices[0][next], layerIndices[0][i]));
		Indices.push_back(glm::ivec3(topCenterIndex, layerIndices[resolucao][i], layerIndices[resolucao][next]));
	}
}

GLuint CarregarPrisma(GLuint& TotalVertices, GLuint& TotalIndices, float xsize, float ysize, float zsize, GLuint numSides, glm::vec3 Centro) {
	std::vector<Vertice> Vertices;
	std::vector<glm::ivec3> Triangulos;

	GerarMalhaPrisma(50, 1, xsize, ysize, zsize, numSides, Centro, Vertices, Triangulos);

	TotalVertices = Vertices.size();
	TotalIndices = Triangulos.size() * 3;

	GLuint VertexBuffer;
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	GLsizeiptr size = Vertices.size() * sizeof(Vertice);
	glBufferData(GL_ARRAY_BUFFER, size, Vertices.data(), GL_STATIC_DRAW);

	GLuint ElementBuffer;
	glGenBuffers(1, &ElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	GLsizeiptr sizeIndice = TotalIndices * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndice, Triangulos.data(), GL_STATIC_DRAW);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice),
		nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Normal)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Cor)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, UV)));

	glBindVertexArray(0);

	return VAO;
}