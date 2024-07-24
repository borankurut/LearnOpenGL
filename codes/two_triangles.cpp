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
"void main(){\n"
	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

const char* fragmentShaderOrange = 
"#version 330 core\n"
"out vec4 FragColor;\n"

"void main() { \n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

const char* fragmentShaderYellow= 
"#version 330 core\n"
"out vec4 FragColor;\n"

"void main() { \n"
"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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
	const int infoBufferSize = 512;
	char infoLog[infoBufferSize]; 

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // integer verctor but success int? 

	if(!success){
		glGetShaderInfoLog(vertexShader, infoBufferSize, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; 
	}

	// Fragment Shader:

	//create shader object
	unsigned int fragmentShaders[2];

	fragmentShaders[0] = glCreateShader(GL_FRAGMENT_SHADER);

	//attach the shader code and compile
	glShaderSource(fragmentShaders[0], 1, &fragmentShaderOrange, NULL);
	glCompileShader(fragmentShaders[0]);

	glGetShaderiv(fragmentShaders[0], GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(fragmentShaders[0], infoBufferSize, NULL, infoLog); 
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; 
	}

	fragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);

	//attach the shader code and compile
	glShaderSource(fragmentShaders[1], 1, &fragmentShaderYellow, NULL);
	glCompileShader(fragmentShaders[1]);

	glGetShaderiv(fragmentShaders[1], GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(fragmentShaders[1], infoBufferSize, NULL, infoLog); 
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; 
	}


	// create shader programs and link the shaders:

	//create the shader program object
	unsigned int shaderPrograms[2];
	shaderPrograms[0] = glCreateProgram();
	shaderPrograms[1] = glCreateProgram();

	// attach the shaders and link
	glAttachShader(shaderPrograms[0], vertexShader);
	glAttachShader(shaderPrograms[0], fragmentShaders[0]);
	glLinkProgram(shaderPrograms[0]);

	// create shader program and link the shaders:

	// attach the shaders and link
	glAttachShader(shaderPrograms[1], vertexShader);
	glAttachShader(shaderPrograms[1], fragmentShaders[1]);
	glLinkProgram(shaderPrograms[1]);

	// delete the shaders after linking.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaders[0]);
	glDeleteShader(fragmentShaders[1]);


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


	while(!glfwWindowShouldClose(window)){
		//input
		processInput(window);


		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

	
		//draw triangles:
		glUseProgram(shaderPrograms[0]);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);  

		//draw triangles:
		glUseProgram(shaderPrograms[1]);
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
