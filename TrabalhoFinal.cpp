#include "TrabalhoFinal.h"

void Resize(GLFWwindow* window, int Width, int Height);

void ProcessInput(GLFWwindow* window);

void MouseCallback(GLFWwindow* Window, double xposIn, double yposIn);

void ScrollCallback(GLFWwindow* Window, double xoffset, double yoffset);

int Width = 1280;
int Height = 720;

Camera camera(glm::vec3(0.0f,0.0f,0.0f));
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
	
	glfwWindowHint(GLFW_DECORATED, NULL);
	GLFWmonitor* MyMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(MyMonitor);
	Width = mode->width;
	Height = mode->height;
	GLFWwindow* window = glfwCreateWindow(Width, Height, "CG com OpenGL", MyMonitor, nullptr);
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

	GLuint GramaId = CarregarTextura("textures/grass3.jpg");
	Floor floor(GramaId, 1000.0f, 1.0f, 1000.0f, 200, glm::vec3(0.0f, -2.0f, 0.0f), 0);

	GLuint TreeId = CarregarTextura("textures/tree.jpg");
	GLuint FolhasId = CarregarTextura("textures/folhas2.jpg");
	Floresta floresta = Floresta(TreeId, FolhasId, FolhasId);

	GLuint CercaViradaId = CarregarTextura("textures/tree-flipped.jpg");
	GLuint CercaId = CarregarTextura("textures/tree.jpg");
	Cercas cercas = Cercas(CercaViradaId, CercaId);

	GLuint WallId = CarregarTextura("textures/wall.png");
	GLuint RoofId = CarregarTextura("textures/roof.jpg");
	GLuint FoundationId = CarregarTextura("textures/darl-plank.jpg");
	GLuint TableId = CarregarTextura("textures/table.jpg");
	GLuint PavingId = CarregarTextura("textures/rocks.jpg");
	GLuint LanternId = CarregarTextura("textures/lanterna.jpg");
	GLuint LanternBoxId = CarregarTextura("textures/metal.jpg");
	GLuint MundoId = CarregarTextura("textures/mundo.jpg");
	Casa casa = Casa(WallId, RoofId, FoundationId, PavingId, TableId, LanternId, LanternBoxId, MundoId);


	GLuint RoxoId = CarregarTextura("textures/roxo.jpg");
	GLuint PretoId = CarregarTextura("textures/preto.jpg");
	Slenderman enderman = Slenderman(PretoId, RoxoId, glm::vec3(0,0.8,-20));

	glm::vec3 L1Pos = glm::vec3((7.0f / 3.0f), 0.35f, 15.0f);
	glm::vec3 L2Pos = glm::vec3(-(7.0f / 3.0f), 0.35, 15.0f);
	glm::vec3 LCPos = glm::vec3(0, -0.3, 4);
	glm::vec3 LCEPos = glm::vec3(-4.5, -0.3, -4.9);
	glm::vec3 LMPos = glm::vec3(3, -0.15, -7);

	Lanterna lanterna1(LanternId, LanternBoxId, 0.5, 0.6, 0.5, L1Pos);
	Lanterna lanterna2(LanternId, LanternBoxId, 0.5, 0.6, 0.5, L2Pos);

	ObjectShader.Use();
	ObjectShader.setInt("material.diffuse", 0);

	GLuint MoonId = CarregarTextura("textures/moon.jpg");
	Moon moon = Moon(MoonId, glm::vec3(0.0f, 30.0f, -50.0f));

	DirectionalLight dl(
		glm::vec3(-(moon.Centro - glm::vec3(0,0,0))),
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5, 0.5, 0.5),
		glm::vec3(0.5,0.5,0.5)
	);

	PointLight pl1(
		L1Pos, 
		1.0f, 
		0.09f, 
		0.128f, 
		glm::vec3(0.5f, 0.5f, 0.5f), 
		glm::vec3(0.8f, 0.8f, 0.8f), 
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.99, 0.89, 0.64)
	);
	PointLight pl2(
		L2Pos,
		1.0f,
		0.09f,
		0.128f,
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.99, 0.89, 0.64)
	);
	PointLight pl3(
		LCPos,
		1.0f,
		0.09f,
		0.128f,
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.99, 0.89, 0.64)
	);
	PointLight pl4(
		LCEPos,
		1.0f,
		0.09f,
		0.256f,
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.99, 0.89, 0.64)
	);
	PointLight pl5(
		LMPos,
		1.0f,
		0.09f,
		0.256f,
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0, 0, 1)
	);

	SpotLight sl(
		camera.Position,
		camera.Front,
		glm::cos(glm::radians(25.0f)),
		glm::cos(glm::radians(27.5f)),
		1.0f,
		0.09f,
		0.032f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.75f, 0.75f, 0.75f),
		glm::vec3(0.988, 0.831, 0.404),
		glm::vec3(0.988, 0.831, 0.404)
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


		glm::mat4 Projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 10000.0f);
		glm::mat4 View = camera.GetViewMatrix();

		ObjectShader.setMat4("Projection", Projection);
		ObjectShader.setMat4("View", View);

		// Luz Direcional
		dl.setToShader(&ObjectShader);

		// Luz Pontuais
		pl1.setToShader(&ObjectShader, 0);
		pl2.setToShader(&ObjectShader, 1);
		pl3.setToShader(&ObjectShader, 2);
		pl4.setToShader(&ObjectShader, 3);
		pl5.setToShader(&ObjectShader, 4);

		// Lanterna
		sl.updateCameraValues(camera.Position, camera.Front);
		sl.setToShader(&ObjectShader);

		floor.Render(&ObjectShader);
		floresta.Render(&ObjectShader);
		cercas.Render(&ObjectShader);
		casa.Render(&ObjectShader);
		casa.Rotate(0.5);
		lanterna1.Render(&ObjectShader);
		lanterna2.Render(&ObjectShader);
		enderman.Render(&ObjectShader);

	   // Adicionado Luz Objeto
		LightShader.Use();
		LightShader.setMat4("Projection", Projection);
		LightShader.setMat4("View", View);
		LightShader.setInt("TextureSampler", 0);
		moon.Render(&LightShader);

		glBindVertexArray(0);
		glUseProgram(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	floor.Delete();
	floresta.Delete();
	cercas.Delete();
	casa.Delete();
	lanterna1.Delete();
	lanterna2.Delete();
	enderman.Delete();
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
