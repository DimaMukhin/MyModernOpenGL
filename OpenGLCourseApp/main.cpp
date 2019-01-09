#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, IBO, shader, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
static const char* vShader = "										\n\
#version 330														\n\
																	\n\
layout (location = 0) in vec3 pos;									\n\
																	\n\
out vec4 vCol;																	\n\
																	\n\
uniform mat4 model;																	\n\
uniform mat4 projection;																	\n\
																	\n\
void main()															\n\
{																	\n\
	gl_Position = projection * model * vec4(pos, 1.0f);					\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);																	\n\
}																	\n\
";

// fragment Shader
static const char* fShader = "										\n\
#version 330														\n\
																	\n\
in vec4 vCol;																	\n\
																	\n\
out vec4 color;														\n\
																	\n\
void main()															\n\
{																	\n\
	color = vCol;							\n\
}																	\n\
";

void CreateTriangle()
{
	// this is indexed draws
	// we will use this to tell opengl what trianges to draw easier
	// every 3 indexes is 1 triangle (way easier)
	// we use IBO for this thing
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// creating our VAO
	glGenVertexArrays(1, &VAO); // amount and where to store the id. VAO is metadata of vertices
	glBindVertexArray(VAO); // we need to bind it (see theory lecture for more details)

	glGenBuffers(1, &IBO); // amount and where to store...
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // bind it...
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // setting locations in the IBO

	// creating out VBO
	glGenBuffers(1, &VBO); // same as VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind our VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // theory: VBO holds vertex locations. Static means we dont change (more theory)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);	// this is where we explain how <vertices> is structured (see documentation if you dont understand)
															// we say that each vertex is 3 values long, values are floats, we dont want to normalize them, no stride, start from 0
															// the first 0 here is very important, it will point at a 0 in out shader! (shader location)
															// shader location is what you use to input values into the shader
	glEnableVertexAttribArray(0); // enable the thing above so the shader can access it (first 0 matches the 0 here) (shader location)

	glBindBuffer(GL_ARRAY_BUFFER, 0); // bind to nothing AKA un-bind VBO

	glBindVertexArray(0); // bind to 0 AKA un-bind VAO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbinding IBO
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType); // create empty shader for the type and get the id
	
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	// weird error handling
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// now we start the weird error handling stuff
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the: %d shader, shader type: %s\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram(); // create program and get id for shader

	if (!shader)
	{
		printf("Error creating shader program");
		return;
	}

	// adding shaders to the program
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	// weird error handling
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader); // link everything

	// now we start the weird error handling stuff
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: %s\n", eLog);
		return;
	}

	glValidateProgram(shader); // make sure shader valid
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: %s\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
	uniformProjection = glGetUniformLocation(shader, "projection");
}

int main()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW initialization falied!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // detect old opengl or deprecated code and dissallow it
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // allow forward compatibility

	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// enable depth testing, opengl will correctly draw stuff based on its depth
	glEnable(GL_DEPTH_TEST);

	// setup viewport size (set drawable part of window)
	glViewport(0, 0, bufferWidth, bufferHeight);

	// create triangle and compile shader
	CreateTriangle();
	CompileShaders();

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat) bufferWidth / (GLfloat) bufferHeight, 0.1f, 100.0f);

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// GET + Handle user input events
		glfwPollEvents();

		// animation thingy
		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		curAngle += 0.5f;
		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		if (sizeDirection)
		{
			curSize += 0.01f;
		}
		else
		{
			curSize -= 0.01f;
		}

		if (curSize >= maxSize || curSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// use shader program we created
		glUseProgram(shader);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // giving the shader our uniform value

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); // giving the shader our uniform projection value

		glBindVertexArray(VAO); // work with VAO we created (it is connected with VBO)

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // work with IBO we created

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0); // use IBO to draw trianges, 12 indices, we used unsigned ints, start from 0

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // un-bind out IBO

		glBindVertexArray(0); // un-bind our triangle VAO

		glUseProgram(0); // use program 0 AKA un-assign the shader

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}