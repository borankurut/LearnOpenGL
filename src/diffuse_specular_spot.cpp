#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.hpp"
#include "cube.hpp"
#include "material.hpp"
#include "light.hpp"
#include "texture.hpp"
#include "utils.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

const char* g_lightingShaderVertPath = "Shaders/spot_light.vert";
const char* g_lightingShaderFragPath = "Shaders/spot_light.frag";

float delta_time = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

WindowContext context(&camera);

int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment on macos
	

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if(window == NULL){
		std::cout << "Failed to create the window.\n";
		glfwTerminate();
		return 1;
	}
	
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize the GLAD.\n";
		return 1;
	}

	glfwSwapInterval(0);  // Disable V-Sync

	glfwSetWindowUserPointer(window, (void*) &context);

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, Camera::mouseCallback);
	glfwSetScrollCallback(window, Camera::scrollCallback);

	//disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	Shader lightingShader(g_lightingShaderVertPath, g_lightingShaderFragPath);

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	//create vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// transform matrices
	glm::mat4 cubeModel = glm::mat4(1.0f);
	cubeModel = glm::translate(cubeModel, glm::vec3(0.0f, 0.0f, 0.0f));
	/* glm::mat3 cubeNormalMatrix = glm::mat3(glm::transpose(glm::inverse(cubeModel))); */

	SpotLight light;
	light.position_or_direction = glm::vec4(1.2f, 1.0f, 2.0f, 1.0f);
	light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	light.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

	light.constant = 1.0f;
	light.linear = 0.09f;
	light.quadratic = 0.032f;

	light.direction = camera.getFront();
	light.cutOff = glm::cos(glm::radians(12.5f));

	camera.setNearPlane(Camera::NearPlane{45.0f, SCR_WIDTH, SCR_HEIGHT, 0.1f, 100.0f});

	double prev_time = glfwGetTime();
	double current_time = glfwGetTime();

	lightingShader.use();
	lightingShader.setLight("light", light);

	Texture diffuseTexture = Texture("Textures/container2.png", GL_TEXTURE0, GL_RGBA, GL_RGBA);

	Texture specularTexture = Texture("Textures/container2_specular.png", GL_TEXTURE1, GL_RGBA, GL_RGBA);

	Texture emmisionTexture = Texture("Textures/emmisionTexture.png", GL_TEXTURE2, GL_RGBA, GL_RGBA);

	diffuseTexture.use("material.diffuse", lightingShader);
	specularTexture.use("material.specular", lightingShader);
	/* emmisionTexture.use("material.emmisionMap", lightingShader); */
	lightingShader.setFloat("material.shininess", 32.0f);

	while(!glfwWindowShouldClose(window)){
		//input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
		glBindVertexArray(VAO);

		for (int i = 0; i < 10; ++i){
			// transform matrices
			glm::mat4 cubeModel = glm::mat4(1.0f);
			cubeModel = glm::translate(cubeModel, cubePositions[i]);
			/* glm::mat3 cubeNormalMatrix = glm::mat3(glm::transpose(glm::inverse(cubeModel))); */

			glm::mat3 cubeNormalMatrix = glm::mat3(glm::transpose(glm::inverse(cubeModel * camera.getView()))); // view normal.

			lightingShader.setMat3("normalMatrix", cubeNormalMatrix);
			lightingShader.setVec3("viewPos", camera.getPosition());
			lightingShader.setMat4("model", cubeModel);
			lightingShader.setMat4("view", camera.getView());
			lightingShader.setMat4("projection", camera.getProjection());
			lightingShader.setSpotLight("light", light);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		light.position_or_direction = glm::vec4(camera.getPosition(), 1.0f);

		// this is because we use view space in the shader, TODO: THIS IS SHIT, GET RID OF THIS.
		light.direction = glm::vec3(camera.getView() * glm::vec4(camera.getFront(), 1.0f));
		/* light.direction = camera.getFront(); */

		//compute delta_time
		current_time = glfwGetTime();
		delta_time = current_time - prev_time;
		prev_time = current_time;

		/* std::cout << "Fps:" << 1/delta_time << "\n"; */

		// check call events and swap buffers.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
	const float speed = camera.speed * delta_time;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.setPosition(camera.getPosition() + camera.getFront() * speed);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.setPosition(camera.getPosition() - camera.getFront() * speed);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.setPosition(camera.getPosition() - camera.getRight() * speed);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.setPosition(camera.getPosition() + camera.getRight() * speed);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.setPosition(camera.getPosition() + camera.getUp() * speed);
}

