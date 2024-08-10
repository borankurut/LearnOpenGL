#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"
#include "image_loader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double pos_x, double pos_y);
void scrollCallback(GLFWwindow* window, double offset_x, double offset_y);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* g_vertexShaderPath = "Shaders/coordinate_space.vert";
const char* g_fragmentShaderPath = "Shaders/coordinate_space.frag";

//vertices
float g_vertices[] = { 
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,

	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f 
};

const glm::vec3 g_cubePositions[]{
	 glm::vec3( 0.0f, 0.0f, 0.0f), 
	 glm::vec3( 2.0f, 5.0f, -15.0f), 
	 glm::vec3(-1.5f, -2.2f, -2.5f), 
	 glm::vec3(-3.8f, -2.0f, -12.3f), 
	 glm::vec3( 2.4f, -0.4f, -3.5f), 
	 glm::vec3(-1.7f, 3.0f, -7.5f), 
	 glm::vec3( 1.3f, -2.0f, -2.5f), 
	 glm::vec3( 1.5f, 2.0f, -2.5f), 
	 glm::vec3( 1.5f, 0.2f, -1.5f), 
	 glm::vec3(-1.3f, 1.0f, -1.5f)
};


float delta_time = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

float yaw = 0.0f;
float pitch = 0.0f;

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

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	//disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	Shader shader(g_vertexShaderPath, g_fragmentShaderPath);

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	//create vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices), g_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	ImageLoader::ImageInfo smileInfo = ImageLoader::get().loadImage("Textures/container.jpg", 0);
	if(smileInfo.data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, smileInfo.width, smileInfo.height,  0, GL_RGB, GL_UNSIGNED_BYTE, smileInfo.data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "couldn't load image." << std::endl;
	}

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	ImageLoader::ImageInfo containerInfo = ImageLoader::get().loadImage("Textures/awesomeface.png", 0);
	if(containerInfo.data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, containerInfo.width, containerInfo.height,  0, GL_RGBA, GL_UNSIGNED_BYTE, containerInfo.data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "couldn't load image." << std::endl;
	}


	// transform matrices
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	camera.setNearPlane(Camera::NearPlane{45.0f, SCR_WIDTH, SCR_HEIGHT, 0.1f, 100.0f});

	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);
	shader.setMat4("transform", model);

	double prev_time = glfwGetTime();
	double current_time = glfwGetTime();

	while(!glfwWindowShouldClose(window)){
		//input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		camera.setFront(yaw, pitch);
		shader.setMat4("view", camera.getView());
		shader.setMat4("projection", camera.getProjection());



		for(int i = 0; i < 10; ++i){
			model = glm::mat4(1.0f);
			model = glm::translate(model, g_cubePositions[i]);
			model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
			
			if((i + 1) % 3 == 0){
				//rotate
				model = glm::rotate(model, glm::radians((float)glfwGetTime() * 50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
			}

			shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		//compute delta_time
		current_time = glfwGetTime();
		delta_time = prev_time - current_time;
		prev_time = current_time;

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

	const float cameraSpeed = 10.5f * delta_time;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.setPosition(camera.getPosition() + camera.getDirection() * cameraSpeed);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.setPosition(camera.getPosition() - camera.getDirection() * cameraSpeed);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.setPosition(camera.getPosition() - camera.getRight() * cameraSpeed);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.setPosition(camera.getPosition() + camera.getRight() * cameraSpeed);

}


float g_last_x = SCR_WIDTH / 2.0;
float g_last_y = SCR_HEIGHT / 2.0;

void mouseCallback(GLFWwindow* window, double pos_x, double pos_y){

	const float sensitivity = 0.1f;

	float x_off = pos_x - g_last_x;
	float y_off = -(pos_y - g_last_y); // y is reversed

	g_last_x = pos_x;
	g_last_y = pos_y;

	yaw += x_off * sensitivity;
	pitch += y_off * sensitivity;

	if(yaw > 180.0f) {
		yaw -= 360.0f;
	}
	else if(yaw < -180.0f) {
		yaw += 360.0f;
	}

	if(pitch > 89.0f)
		pitch = 89.0f;

	else if(pitch < -89.0f)
		pitch = -89.0f;

	/* std::cout << "x_off: " << x_off<< ", y_off: " << y_off << std::endl; */
}

void scrollCallback(GLFWwindow* window, double offset_x, double offset_y){
	/* std::cout << offset_x << ", " << offset_y << std::endl; */

	camera.setFov(camera.getFov() - offset_y);
	if(camera.getFov() < 1.0f)
		camera.setFov(1.0f);
	else if (camera.getFov() > 45.0f)
		camera.setFov(45.0f);
}

