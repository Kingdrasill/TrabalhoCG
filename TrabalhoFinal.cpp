#include "TrabalhoFinal.h"

void Resize(GLFWwindow* window, int Width, int Height);

void ProcessInput(GLFWwindow* window);

void MouseCallback(GLFWwindow* Window, double xposIn, double yposIn);

void ScrollCallback(GLFWwindow* Window, double xoffset, double yoffset);

int Width = 1024;
int Height = 576;

Camera camera(glm::vec3(0.0f,0.0f,3.0f));
float lastX = Width / 2.0f;
float lastY = Height / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 LightPos(0.0f, 4.0f, 0.0f);

int main() {
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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Shader LightShader("shaders/light_vert.glsl", "shaders/light_frag.glsl");
	Shader ObjectShader("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");
	
	GLuint TexturaId = CarregarTextura("textures/mundo.jpg");
	GLuint TexturaNuvemId = CarregarTextura("textures/nuvem.jpg");
	//GLuint QuadVAO = CarregaGeometria();

	GLuint LETV = 0;
	GLuint LETI = 0;
	GLuint LEVAO = CarregaEsfera(LETV, LETI, 0.5, LightPos);
	std::cout << "Total de vertices da esfera: " << LETV << std::endl;
	std::cout << "Total de indices da esfera: " << LETI << std::endl;

	GLuint OETV = 0;
	GLuint OETI = 0;
	GLuint OEVAO = CarregaEsfera(OETV, OETI, 1, glm::vec3{ 0.0f,0.0f,0.0f });
	std::cout << "Total de vertices da esfera: " << OETV << std::endl;
	std::cout << "Total de indices da esfera: " << OETI << std::endl;

	GLuint OCTV = 0;
	GLuint OCTI = 0;
	GLuint OCVAO = CarregaCilindro(OCTV, OCTI, 10, 1, glm::vec3{ 2.0f,0.0f,0.0f });
	std::cout << "Total de vertices da esfera: " << OCTV << std::endl;
	std::cout << "Total de indices da esfera: " << OCTI << std::endl;

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::mat4 Model = glm::mat4(1.0f);
	constexpr float AnguloDeRotacao = glm::radians(90.0);
	glm::vec3 EixoRotacao{ 1,0,0 };
	glm::mat4 Transform = glm::rotate(Model, AnguloDeRotacao, EixoRotacao);

	ObjectShader.Use();
	ObjectShader.setInt("material.diffuse", 0);

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
		ObjectShader.setVec3("material.specular", 1.0f, 1.0f, 1.0f);
		ObjectShader.setFloat("material.shininess", 32.0f);

		// Luz Direcional
		ObjectShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		ObjectShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		ObjectShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		ObjectShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// Luz Pontual 1
		ObjectShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		ObjectShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		ObjectShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		ObjectShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		ObjectShader.setFloat("pointLights[0].constant", 1.0f);
		ObjectShader.setFloat("pointLights[0].linear", 0.09f);
		ObjectShader.setFloat("pointLights[0].quadratic", 0.032f);

		// Luz Pontual 2
		ObjectShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		ObjectShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		ObjectShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		ObjectShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		ObjectShader.setFloat("pointLights[1].constant", 1.0f);
		ObjectShader.setFloat("pointLights[1].linear", 0.09f);
		ObjectShader.setFloat("pointLights[1].quadratic", 0.032f);

		// Luz Pontual 3
		ObjectShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		ObjectShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		ObjectShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		ObjectShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		ObjectShader.setFloat("pointLights[2].constant", 1.0f);
		ObjectShader.setFloat("pointLights[2].linear", 0.09f);
		ObjectShader.setFloat("pointLights[2].quadratic", 0.032f);

		// Luz Pontual 4
		ObjectShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		ObjectShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		ObjectShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		ObjectShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		ObjectShader.setFloat("pointLights[3].constant", 1.0f);
		ObjectShader.setFloat("pointLights[3].linear", 0.09f);
		ObjectShader.setFloat("pointLights[3].quadratic", 0.032f);

		// Lanterna
		ObjectShader.setVec3("spotLight.position", camera.Position);
		ObjectShader.setVec3("spotLight.direction", camera.Front);
		ObjectShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		ObjectShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		ObjectShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		ObjectShader.setFloat("spotLight.constant", 1.0f);
		ObjectShader.setFloat("spotLight.linear", 0.09f);
		ObjectShader.setFloat("spotLight.quadratic", 0.032f);
		ObjectShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		ObjectShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		glm::mat4 Projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 10000.0f);
		glm::mat4 View = camera.GetViewMatrix();
		
		ObjectShader.setMat4("Projection", Projection);
		ObjectShader.setMat4("View", View);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexturaId);

		glBindVertexArray(OEVAO);
		ObjectShader.setMat4("Model", Transform);
		glDrawElements(GL_TRIANGLES, OETI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(OCVAO);
		ObjectShader.setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, OCTI, GL_UNSIGNED_INT, nullptr);

	   // Adicionado Luz Objeto
		LightShader.Use();
		LightShader.setMat4("Projection", Projection);
		LightShader.setMat4("View", View);
		LightShader.setVec3("LightColor", 1.0f,1.0f,1.0f);

		glBindVertexArray(LEVAO);
		LightShader.setMat4("Model", Model);
		glDrawElements(GL_TRIANGLES, LETI, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
		glUseProgram(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteBuffers(1, &OEVAO);
	glDeleteBuffers(1, &OCVAO);
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