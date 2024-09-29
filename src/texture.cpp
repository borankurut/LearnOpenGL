#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"
#include "texture.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderPath = "Shaders/vs_texture.vert";
const char* fragmentShaderPath = "Shaders/fs_texture.frag";

float smileyAmount = 0.0f;

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
	Shader shader(vertexShaderPath, fragmentShaderPath);

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


	// init textures
	float ar1[2], ar2[2];
	Texture2 texture1("Textures/cami.png", GL_TEXTURE0, GL_RGBA, GL_RGBA);
	Texture2 texture2("Textures/celal.png", GL_TEXTURE1, GL_RGBA, GL_RGBA);
	ar1[0] = 1.0f;
	ar1[1] = 1.0f / texture1.p_imageInfo.getAspectRatio();

	ar2[0] = 1.0f;
	ar2[1] = 1.0f / texture2.p_imageInfo.getAspectRatio();

	//init object
	//create the VAOs and VBOs
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//first triangle
	// bind vao and set state
	glBindVertexArray(VAO);

	// bind vbo and config
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0); //positions
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float))); //colors
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float))); //colors

	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1); 
	glEnableVertexAttribArray(2); 

	// unbind the vao and vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0); // this is allowed because vao used GL_ARRAY_BUFFER already.
	glBindVertexArray(0);


	shader.use();

	texture1.use("texture1", shader);
	texture2.use("texture2", shader);

	glUniform2f(glGetUniformLocation(shader.ID, "aspectRatio1"), ar1[0], ar1[1]);
	glUniform2f(glGetUniformLocation(shader.ID, "aspectRatio2"), ar2[0], ar2[1]);

	while(!glfwWindowShouldClose(window)){
		//input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	
		//draw
		shader.use();
		shader.setFloat("smileyAmount", smileyAmount);

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
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

	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && smileyAmount <= 1.0f){
		smileyAmount += 0.005f;
	}
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && smileyAmount >= 0.0f){
		smileyAmount -= 0.005f;
	}
}

