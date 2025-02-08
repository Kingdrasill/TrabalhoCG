#include "TrabalhoFinal.h"

GLuint CarregaGeometria() {
	std::array<Vertice, 6> Quadrado = {
		Vertice{
			glm::vec3{-1.0f,-1.0f,0.0f},
			glm::vec3{0.0f,0.0f,1.0f},
			glm::vec3{1.0f,0.0f,0.0f},
			glm::vec2{0.0f,0.0f}
		},
		Vertice{
			glm::vec3{1.0f,-1.0f,0.0f},
			glm::vec3{0.0f,0.0f,1.0f},
			glm::vec3{0.0f,1.0f,0.0f},
			glm::vec2{1.0f,0.0f}
		},
		Vertice{
			glm::vec3{1.0f,1.0f,0.0f},
			glm::vec3{0.0f,0.0f,1.0f},
			glm::vec3{1.0f,0.0f,0.0f},
			glm::vec2{1.0f,1.0f}
		},
		Vertice{
			glm::vec3{-1.0f,1.0f,0.0f},
			glm::vec3{0.0f,0.0f,1.0f},
			glm::vec3{0.0f,0.0f,1.0f},
			glm::vec2{0.0f,1.0f}
		}
	};

	std::array<glm::ivec3, 2> Indices = {
		glm::ivec3{0, 1, 3},
		glm::ivec3{3, 1, 2}
	};

	GLuint VertexBufer;
	glGenBuffers(1, &VertexBufer);

	GLuint ElementBuffer;
	glGenBuffers(1, &ElementBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Quadrado), Quadrado.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices.data(), GL_STATIC_DRAW);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufer);
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

void GerarMalhaCilindro(GLuint resolucao, float altura, float raio, glm::vec3 Centro, std::vector<Vertice>& Vertices, std::vector<glm::ivec3>& Indices) {
	Vertices.clear();
	Indices.clear();

	constexpr float DoisPi = glm::two_pi<float>();
	const float InversoResolucao = 1.0f / static_cast<float>(resolucao);

	// Generate cylinder vertices
	for (GLuint i = 0; i <= resolucao; ++i) {
		float angulo = i * InversoResolucao * DoisPi;
		float x = raio * glm::cos(angulo);
		float z = raio * glm::sin(angulo);

		glm::vec3 normal = glm::normalize(glm::vec3{ x, 0.0f, z });

		// Top and bottom vertices
		Vertices.push_back(Vertice{ Centro + glm::vec3{x, altura / 2.0f, z}, normal, glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec2{i * InversoResolucao, 1.0f} });
		Vertices.push_back(Vertice{ Centro + glm::vec3{x, -altura / 2.0f, z}, normal, glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec2{i * InversoResolucao, 0.0f} });
	}

	// Top center vertex
	GLuint topoIndex = Vertices.size();
	Vertices.push_back(Vertice{ Centro + glm::vec3{0.0f, altura / 2.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec2{0.5f, 1.0f} });

	// Bottom center vertex
	GLuint baseIndex = Vertices.size();
	Vertices.push_back(Vertice{ Centro + glm::vec3{0.0f, -altura / 2.0f, 0.0f}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec2{0.5f, 0.0f} });

	// Generate side faces
	for (GLuint i = 0; i < resolucao; ++i) {
		GLuint p0 = i * 2;
		GLuint p1 = (i + 1) * 2;
		GLuint p2 = p1 + 1;
		GLuint p3 = p0 + 1;

		Indices.push_back(glm::ivec3{ p0, p1, p2 });
		Indices.push_back(glm::ivec3{ p0, p2, p3 });
	}

	// Generate top and bottom faces
	for (GLuint i = 0; i < resolucao; ++i) {
		GLuint p0 = i * 2;
		GLuint p1 = (i + 1) * 2;

		// Top face
		Indices.push_back(glm::ivec3{ topoIndex, p1, p0 });

		// Bottom face
		GLuint p0b = p0 + 1;
		GLuint p1b = p1 + 1;
		Indices.push_back(glm::ivec3{ baseIndex, p0b, p1b });
	}
}

GLuint CarregaCilindro(GLuint& TotalVertices, GLuint& TotalIndices, float altura, float raio, glm::vec3 Centro) {
	std::vector<Vertice> Vertices;
	std::vector<glm::ivec3> Triangulos;

	GerarMalhaCilindro(50, altura, raio, Centro, Vertices, Triangulos);

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