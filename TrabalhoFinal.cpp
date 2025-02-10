#include "TrabalhoFinal.h"

void Resize(GLFWwindow* window, int Width, int Height);

void ProcessInput(GLFWwindow* window);

void MouseCallback(GLFWwindow* Window, double xposIn, double yposIn);

void ScrollCallback(GLFWwindow* Window, double xoffset, double yoffset);

float getRandomInRange(float min, float max);

#define NUM_ARV 2000
#define NUM_ARB 2000

int Width = 1280;
int Height = 720;

Camera camera(glm::vec3(0.0f,0.0f,3.0f));
float lastX = Width / 2.0f;
float lastY = Height / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 LightPos(0.0f, 4.0f, 0.0f);

int main() {
	std::srand(std::time(0));
	setlocale(LC_ALL, "pt_BR");
	assert(glfwInit() == GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(Width, Height, "CG com OpenGL", nullptr, nullptr);
	assert(window);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Resize);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Resize(window, Width, Height);

	glEnable(GL_DEPTH_TEST);

	Shader LightShader("shaders/light_vert.glsl", "shaders/light_frag.glsl");
	Shader ObjectShader("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");

	GLuint TexturaMundoId = CarregarTextura("textures/mundo.jpg");
	GLuint TexturaNuvemId = CarregarTextura("textures/nuvem.jpg");
	GLuint TexturaGrassId = CarregarTextura("textures/grass.jpg");
	GLuint TexturaGramaTerrorId = CarregarTextura("textures/grass3.jpg");
	GLuint TexturaTreeId = CarregarTextura("textures/tree.jpg");
	GLuint TexturaFolhasId = CarregarTextura("textures/folhas2.jpg");
	GLuint TexturaArbustoId = CarregarTextura("textures/folhas2.jpg");

	GLuint LETV = 0;
	GLuint LETI = 0;
	GLuint LEVAO = CarregarSemiesfera(LETV, LETI, 1, LightPos);

	Floor floor(TexturaGramaTerrorId, 1000.0f, 1.0f, 1000.0f, glm::vec3(0.0f, -2.0f, 0.0f));

	std::array<Arvore, NUM_ARV> arvores;
	for (int i = 0; i < NUM_ARV; i++) {
		float x;
		float z;

		if (rand() % 2 == 0) {
			x = (rand() % 2 == 0) ? getRandomInRange(-250, -12) : getRandomInRange(12, 250);
			z = (rand() % 2 == 0) ? getRandomInRange(-250, 0) : getRandomInRange(0, 250);
		}
		else {
			x = (rand() % 2 == 0) ? getRandomInRange(-250, 0) : getRandomInRange(0, 250);
			z = (rand() % 2 == 0) ? getRandomInRange(-250, -12) : getRandomInRange(12, 250);
		}

		float raio = 0.25 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.75 - 0.25)));
		float alturaB = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.5 - 1)));
		float alturaC = 4 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - 4)));
		float elevacaoC = 0.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (alturaB - 0.5)));
		
		arvores[i] = Arvore(TexturaTreeId, TexturaFolhasId, glm::vec3(x, (-1.0 + 0.5 * (alturaB - 1)), z), alturaB, alturaC, raio*0.8, 2.2*raio, elevacaoC);
	}

	std::array<Arbusto, NUM_ARB> arbustos;
	for (int i = 0; i < NUM_ARV; i++) {
		float x;
		float z;

		if (rand() % 2 == 0) {
			x = (rand() % 2 == 0) ? getRandomInRange(-250, -12) : getRandomInRange(12, 250);
			z = (rand() % 2 == 0) ? getRandomInRange(-250, 0) : getRandomInRange(0, 250);
		}
		else {
			x = (rand() % 2 == 0) ? getRandomInRange(-250, 0) : getRandomInRange(0, 250);
			z = (rand() % 2 == 0) ? getRandomInRange(-250, -12) : getRandomInRange(12, 250);
		}

		float raio = 0.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.5 - 0.5)));

		arbustos[i] = Arbusto(TexturaArbustoId, glm::vec3(x, -1.5f, z), raio);
	}

	Cerca cerca(TexturaTreeId, glm::vec3(0, -0.5, 0), 1, 0.0f);
	Cerca cerca2(TexturaTreeId, glm::vec3(-2*1-0.75, -0.5, 0), 1, 90.0f);

	ObjectShader.Use();
	ObjectShader.setInt("material.diffuse", 0);

	DirectionalLight dl(
		glm::vec3(0.0f, -10.0f, 0.0f),
		glm::vec3(0.3f, 0.3f, 0.3f),
		glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	);

	PointLight pl1(glm::vec3(100.0f, 100.0f, 100.0f), 1.0f, 0.09f, 0.032f, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f));
	PointLight pl2(glm::vec3(2.3f, -3.3f, -4.0f), 1.0f, 0.09f, 0.032f, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f));
	PointLight pl3(glm::vec3(-4.0f, 2.0f, -12.0f), 1.0f, 0.09f, 0.032f, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f));
	PointLight pl4(glm::vec3(0.0f, 0.0f, -3.0f), 1.0f, 0.09f, 0.032f, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f));

	SpotLight sl(
		camera.Position,
		camera.Front,
		glm::cos(glm::radians(25.0f)),
		glm::cos(glm::radians(27.5f)),
		1.0f,
		0.09f,
		0.008f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window);

		glPointSize(10.0f);
		glLineWidth(10.0f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ObjectShader.Use();
		ObjectShader.setVec3("ViewPos", 1.0f, 1.0f, 1.0f);
		ObjectShader.setVec3("material.specular", 0.0f, 0.0f, 0.0f);
		ObjectShader.setFloat("material.shininess", 1024.0f);

		// Luz Direcional
		dl.setToShader(&ObjectShader);

		// Luz Pontuais
		pl1.setToShader(&ObjectShader, 0);

		// Lanterna
		sl.updateCameraValues(camera.Position, camera.Front);
		sl.setToShader(&ObjectShader);

		glm::mat4 Projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 10000.0f);
		glm::mat4 View = camera.GetViewMatrix();
		
		ObjectShader.setMat4("Projection", Projection);
		ObjectShader.setMat4("View", View);

		floor.Render(&ObjectShader);

		for (int i = 0; i < NUM_ARV; i++) {
			arvores[i].Render(&ObjectShader);
		}

		for (int i = 0; i < NUM_ARB; i++) {
			arbustos[i].Render(&ObjectShader);
		}

		cerca.Render(&ObjectShader);
		cerca2.Render(&ObjectShader);

	   // Adicionado Luz Objeto
		LightShader.Use();
		LightShader.setMat4("Projection", Projection);
		LightShader.setMat4("View", View);
		LightShader.setVec3("LightColor", 1.0f,1.0f,1.0f);

		glBindVertexArray(0);
		glUseProgram(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteBuffers(1, &LEVAO);
	glfwTerminate();
	return 0;
}

void Resize(GLFWwindow* window, int Width, int Height) {
	glViewport(0, 0, Width, Height);
}

void ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcressKeyboard(FOWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcressKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcressKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcressKeyboard(RIGHT, deltaTime);
}

void MouseCallback(GLFWwindow* Window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void ScrollCallback(GLFWwindow* Window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

float getRandomInRange(float min, float max) {
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}