#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.hpp"
#include "model.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

const char* g_shaderVertPath = "Shaders/model_loading.vert";
const char* g_shaderFragPath = "Shaders/model_loading.frag";

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

	Shader shader(g_shaderVertPath, g_shaderFragPath);

	camera.setNearPlane(Camera::NearPlane{45.0f, SCR_WIDTH, SCR_HEIGHT, 0.1f, 100.0f});

	double prev_time = glfwGetTime();
	double current_time = glfwGetTime();

	Model backpack("./Objects/backpack/backpack.obj");

	while(!glfwWindowShouldClose(window)){
		//input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

		shader.setMat4("model", model);
		shader.setMat4("view", camera.getView());
		shader.setMat4("projection", camera.getProjection());

		backpack.Draw(shader);

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

