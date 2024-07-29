#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderFileName = "./Shaders/vertexShaderOutPosition.vert";
const char* fragmentShaderFileName= "./Shaders/fragmentShaderPos.frag";

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


	// shaders:
	
	Shader shader(vertexShaderFileName, fragmentShaderFileName);

	// our triangle vertices:
	const float triangleVertices1[] = {
		-0.5f, -0.5f, 0.0f, 
		0.5f, -0.5f, 0.0f, 
		0.0f, 0.5f, 0.0f 
	};

	//init the triangle:
	
	//create the VAOs and VBOs
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//first triangle
	// bind vao and set state
	glBindVertexArray(VAO);

	// bind vbo and config
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices1), triangleVertices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

	glEnableVertexAttribArray(0); 

	// unbind the vao and vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0); // this is allowed because vao used GL_ARRAY_BUFFER already.
	glBindVertexArray(0);

	while(!glfwWindowShouldClose(window)){
		//input
		processInput(window);


		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw triangles:
		shader.use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);  

		// check call events and swap buffers.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
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
