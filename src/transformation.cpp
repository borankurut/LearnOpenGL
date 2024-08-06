#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"
#include "image_loader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderPath = "Shaders/transformation.vert";
const char* fragmentShaderPath = "Shaders/transformation.frag";

//vertices
const float vertices[] = {
	// positions		// colors			// texture coords 
	0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // top right 
	0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // bottom right 
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // bottom left 
	-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f // top left 	
};

const unsigned int indices[] = { 
	// note that we start from 0! 
	0, 1, 3, // first triangle 
	1, 2, 3 // second triangle 
}; 

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

	Shader shader(vertexShaderPath, fragmentShaderPath);

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//create vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	ImageLoader::ImageInfo smileInfo = ImageLoader::get().loadImage("textures/container.jpg", 0);
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

	ImageLoader::ImageInfo containerInfo = ImageLoader::get().loadImage("textures/awesomeface.png", 0);
	if(containerInfo.data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, containerInfo.width, containerInfo.height,  0, GL_RGBA, GL_UNSIGNED_BYTE, containerInfo.data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "couldn't load image." << std::endl;
	}


	glm::mat4 transform1 = glm::mat4(1.0f);
	transform1 = glm::translate(transform1, glm::vec3(0.5f, 0.0f, 0.0f));
	transform1 = glm::scale(transform1, glm::vec3(0.5f, 0.5f, 0.5f));

	glm::mat4 transform2 = glm::mat4(1.0f);
	transform2 = glm::translate(transform2, glm::vec3(-0.5f, 0.0f, 0.0f));
	transform2 = glm::scale(transform2, glm::vec3(0.5f, 0.5f, 0.5f));




	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);
	shader.setMat4("transform", transform1);

	double prev_time = glfwGetTime();
	double current_time = glfwGetTime();
	float delta_time = 0.0f;

	while(!glfwWindowShouldClose(window)){
		//input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		const float rotation_speed = 40.0f * 10.0f;
		transform1 = glm::rotate(transform1, glm::radians(delta_time * rotation_speed), glm::vec3(0, 0, 1));

		shader.use();
		shader.setMat4("transform", transform1);

		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		transform2 = glm::mat4(1.0f);
		transform2 = glm::translate(transform2, glm::vec3(-0.5f, 0.0f, 0.0f));
		transform2 = glm::scale(transform2, glm::vec3(0.5f));

		transform2 = glm::scale(transform2, glm::vec3(glm::abs(glm::sin(current_time))));

		shader.setMat4("transform", transform2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	
		
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
}


