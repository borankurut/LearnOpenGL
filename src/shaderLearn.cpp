#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"

"out vec4 vertexColor;\n"

"void main(){\n"
	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"

	"vertexColor = vec4(aColor.x, aColor.y, aColor.z, 1.0f);\n"

"}\n";

const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"

"void main() { \n"
"	FragColor = vertexColor;\n"
"}\n";

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


	//build and compile our shader program

	// Vertex Shader:

	//create shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//attach the shader code and compile
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success; 
	char infoLog[512]; 
	const int infoBufferSize = 512;

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // integer verctor but success int? 

	if(!success){
		glGetShaderInfoLog(vertexShader, infoBufferSize, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; 
	}

	// Fragment Shader:

	//create shader object
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//attach the shader code and compile
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(fragmentShader, infoBufferSize, NULL, infoLog); 
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; 
	}

	// create shader program and link the shaders:

	//create the shader program object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// attach the shaders and link
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// delete the shaders after linking.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// our triangle vertices:
	const float triangleVertices[] = {
		//vertex			//colors
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f 
	};

	//init the triangle:
	
	//create the VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// create the vertex buffer object:
	unsigned VBO;
	glGenBuffers(1, &VBO);

	// bind vao and set state
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));

	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1); 

	// unbind the vao and vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0); // this is allowed because vao used GL_ARRAY_BUFFER already.
	glBindVertexArray(0);
	
	glUseProgram(shaderProgram); // do we need to use the shame shader program and bind the same vao every frame?
	while(!glfwWindowShouldClose(window)){
		//input
		processInput(window);


		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw the first triangle

		/* glUseProgram(shaderProgram); // do we need to use the shame shader program and bind the same vao every frame? */
		glBindVertexArray(VAO);
		
		// draw triangle:
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
