#include "TrabalhoFinal.h"

int Width = 800;
int Height = 600;

FlyCamera Camera;

glm::vec2 PosicaoAnteriorCursor;
bool habilitarMovimentoMouse = false;

void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Modifiers) {
	if (Button == GLFW_MOUSE_BUTTON_LEFT) {
		if (Action == GLFW_PRESS) {
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			double X, Y;
			glfwGetCursorPos(Window, &X, &Y);

			PosicaoAnteriorCursor = glm::vec2{ X, Y };
			habilitarMovimentoMouse = true;
		}
		else if (Action == GLFW_RELEASE) {
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			habilitarMovimentoMouse = false;
		}
	}
}

void MouseMotionCallback(GLFWwindow* Window, double X, double Y) {
	if (habilitarMovimentoMouse) {
		glm::vec2 PosicaoAtualCursor{ X,Y };
		glm::vec2 Delta = PosicaoAnteriorCursor - PosicaoAtualCursor;

		Camera.OlharPara(-Delta.x, -Delta.y);
		PosicaoAnteriorCursor = PosicaoAtualCursor;
	}
}

void Resize(GLFWwindow* window, int NovaLargura, int NovaAltura) {
	int Width = NovaLargura;
	int Height = NovaAltura;

	Camera.AspectRatio = static_cast<float>(Width) / Height;

	glViewport(0, 0, Width, Height);
}

int main() {
	setlocale(LC_ALL, "pt_BR");
	assert(glfwInit() == GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(Width, Height, "CG com OpenGL", nullptr, nullptr);
	assert(window);

	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, MouseMotionCallback);
	glfwSetFramebufferSizeCallback(window, Resize);

	int statusGlwInit = glewInit();
	assert(glewInit() == GLEW_OK);

	GLint GLMajorVersion = 0;
	GLint GLMinorVersion = 0;

	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);

	std::cout << "Maior versão do OpenGL suportada: " << GLMajorVersion << std::endl;
	std::cout << "Menor versão do OpenGL suportada: " << GLMinorVersion << std::endl;

	std::cout << "Fabricante do driver de video: " << glGetString(GL_VENDOR) << std::endl;
	//std::cout << "Modelo da place de video: " << glGetString(GL_RENDER) << std::endl;
	std::cout << "Versão do OpenGL: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Versão do GLS: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	Resize(window, Width, Height);

	GLuint ProgramaId = LoadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");
	GLuint TexturaId = CarregarTextura("textures/mundo.jpg");
	GLuint TexturaNuvemId = CarregarTextura("textures/nuvem.jpg");

	//GLuint QuadVAO = CarregaGeometria();

	GLuint EsferaTotalVertices = 0;
	GLuint EsferaTotalIndices = 0;
	GLuint EsferaVAO = CarregaEsfera(EsferaTotalVertices, EsferaTotalIndices, 1, glm::vec3{ 0.0f,0.0f,0.0f });
	std::cout << "Total de vertices da esfera: " << EsferaTotalVertices << std::endl;
	std::cout << "Total de indices da esfera: " << EsferaTotalIndices << std::endl;

	GLuint CilindroTotalVertices = 0;
	GLuint CilindroTotalIndices = 0;
	GLuint CilindroVAO = CarregaCilindro(CilindroTotalVertices, CilindroTotalIndices, 2, 1, glm::vec3{ 2.0f,0.0f,0.0f });
	std::cout << "Total de vertices da esfera: " << CilindroTotalVertices << std::endl;
	std::cout << "Total de indices da esfera: " << CilindroTotalIndices << std::endl;

	glm::mat4 MatrizIdentidade = glm::identity<glm::mat4>();
	constexpr float AnguloDeRotacao = glm::radians(90.0);
	glm::vec3 EixoRotacao{ 1,0,0 };
	glm::mat4 MatrizModel = glm::rotate(MatrizIdentidade, AnguloDeRotacao, EixoRotacao);

	double TempoAtualAtulizadoFrameAnterior = glfwGetTime();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	LuzDirecional Luz;
	Luz.Direcao = glm::vec3{ 0.0f,0.0f,-1.0f };
	Luz.Intensidade = 1.0f;

	while (!glfwWindowShouldClose(window)) {
		double TempoAtualizadoFrameAtual = glfwGetTime();
		double DeltaTime = TempoAtualizadoFrameAtual - TempoAtualAtulizadoFrameAnterior;
		if (DeltaTime > 0) {
			TempoAtualAtulizadoFrameAnterior = TempoAtualizadoFrameAtual;
		}

		Luz.Direcao = Camera.Direction;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ProgramaId);

		glm::mat4 MatrizNormal = glm::inverse(glm::transpose(Camera.GetView() * MatrizModel));
		glm::mat4 MatrizViewProjection = Camera.GetViewProjection();
		glm::mat4 ModelViewProjection = MatrizViewProjection * MatrizModel;

		GLint TimeLoc = glGetUniformLocation(ProgramaId, "Time");
		glUniform1f(TimeLoc, TempoAtualizadoFrameAtual);

		GLint ModelViewProjectionLoc = glGetUniformLocation(ProgramaId, "ModelViewProjection");
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

		GLint MatrizNormalLoc = glGetUniformLocation(ProgramaId, "MatrizNormal");
		glUniformMatrix4fv(MatrizNormalLoc, 1, GL_FALSE, glm::value_ptr(MatrizNormal));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexturaId);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TexturaNuvemId);

		GLint TextureSamplerLoc = glGetUniformLocation(ProgramaId, "TextureSampler");
		glUniform1i(TextureSamplerLoc, 0);

		GLint TextureNuvemSamplerLoc = glGetUniformLocation(ProgramaId, "NuvemTextureSampler");
		glUniform1i(TextureNuvemSamplerLoc, 1);

		GLint DirezaoLuzLoc = glGetUniformLocation(ProgramaId, "DirecaoLuz");
		glUniform3fv(DirezaoLuzLoc, 1, glm::value_ptr(Camera.GetView() * glm::vec4{ Luz.Direcao, 0.0f }));

		GLint IntensidadeLuzLoc = glGetUniformLocation(ProgramaId, "IntensidadeLuz");
		glUniform1f(IntensidadeLuzLoc, Luz.Intensidade);

		GLint CameraPosLoc = glGetUniformLocation(ProgramaId, "CameraPos");
		glUniform3fv(CameraPosLoc, 1, glm::value_ptr(Camera.Direction));

		glPointSize(10.0f);
		glLineWidth(10.0f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindVertexArray(EsferaVAO);
		glDrawElements(GL_TRIANGLES, EsferaTotalIndices, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(CilindroVAO);
		glDrawElements(GL_TRIANGLES, CilindroTotalIndices, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
		glUseProgram(0);

		glfwPollEvents();
		glfwSwapBuffers(window);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			Camera.MoverParaFrente(1.0f * DeltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			Camera.MoverParaFrente(-1.0f * DeltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			Camera.MoverParaDireita(-1.0f * DeltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			Camera.MoverParaDireita(1.0f * DeltaTime);
		}
	}

	glDeleteBuffers(1, &EsferaVAO);
	glDeleteBuffers(1, &CilindroVAO);
	glfwTerminate();
	return 0;
}