#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderFileName = "./Shaders/vertexShaderPlain.vert";
const char* fragmentShaderFileName = "./Shaders/fragmentShaderOrange.frag";
const char* fragmentShaderYellowFileName = "./Shaders/fragmentShaderYellow.frag";

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
	
	Shader yellowShader(vertexShaderFileName, fragmentShaderYellowFileName);
	Shader orangeShader(vertexShaderFileName, fragmentShaderFileName);

	// our triangle vertices:
	const float triangleVertices1[] = {
		-1.0f, -0.5f, 0.0f, 
		0.0f, -0.5f, 0.0f, 
		-0.5f, 0.5f, 0.0f 
	};

	const float triangleVertices2[] = {
		0.0f, -0.5f, 0.0f, 
		1.0f, -0.5f, 0.0f, 
		0.5f, 0.5f, 0.0f 
	};



	//init the triangle:
	
	//create the VAOs and VBOs
	unsigned int VAOs[2],VBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	//first triangle
	// bind vao and set state
	glBindVertexArray(VAOs[0]);

	// bind vbo and config
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices1), triangleVertices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

	glEnableVertexAttribArray(0); 

	// unbind the vao and vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0); // this is allowed because vao used GL_ARRAY_BUFFER already.
	glBindVertexArray(0);


	// second triangle
	// bind vao and set state
	glBindVertexArray(VAOs[1]);

	// bind vbo and config
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices2), triangleVertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

	glEnableVertexAttribArray(0); 

	// unbind the vao and vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0); // this is allowed because vao used GL_ARRAY_BUFFER already.
	glBindVertexArray(0);

	float hor_off = 0.0f;

	while(!glfwWindowShouldClose(window)){
		//input
		processInput(window);


		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	
		hor_off += 0.001;
		if (hor_off > 0.5) {
			hor_off = 0.0;
		}


		//draw triangles:
		yellowShader.use();
		yellowShader.setFloat("hor_off", hor_off);

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);  

		//draw triangles:
		orangeShader.use();
		orangeShader.setFloat("hor_off", hor_off);

		glBindVertexArray(VAOs[1]);
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
