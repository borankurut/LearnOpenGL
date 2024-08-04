#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"
#include "image_loader.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderFileName = "Shaders/vs_texture.vert";
const char* fragmentShaderFileName = "Shaders/fs_texture.frag";

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
	Shader shader(vertexShaderFileName, fragmentShaderFileName);

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

	//textures:
	float ar1[2], ar2[2];
	unsigned int texture1;	
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	unsigned char* data = ImageLoader::get().loadImage("Textures/cami.png", &width, &height, &nrChannels, 0);

	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		ar1[0] = 1.0f;
		ar1[1] = (float) height/width;
	}

	else{
		std::cerr << "Couldn't load texture.\n";
	}
	ImageLoader::get().freeImage(data);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//texture2
	unsigned int texture2;	
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	//params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	data = ImageLoader::get().loadImage("Textures/celal.png", &width, &height, &nrChannels, 0);

	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		ar2[0] = 1.0f;
		ar2[1] = (float) height/width;
	}

	else{
		std::cerr << "Couldn't load texture.\n";
	}
	ImageLoader::get().freeImage(data);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

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
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	glUniform2f(glGetUniformLocation(shader.ID, "aspectRatio1"), ar1[0], ar1[1]);
	glUniform2f(glGetUniformLocation(shader.ID, "aspectRatio2"), ar2[0], ar2[1]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

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
		smileyAmount += 0.001f;
	}
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && smileyAmount >= 0.0f){
		smileyAmount -= 0.001f;
	}
}

