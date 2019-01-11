#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

class Shader
{
public:
	Shader();

	// create shader from string
	void CreateFromString(const char* vertexCode, const char* fragmentCode);

	// create shader from files
	void CreateFromFiles(const char* certexLocation, const char* fragmentLocation);

	// simply read file
	std::string ReadFile(const char* fileLocation);

	// get uniform projection location/id
	GLuint GetProjectionLocation();

	// get uniform model location/id
	GLuint GetModelLocation();

	// get uniform view location/id
	GLuint GetViewLocation();

	// get uniform ambient intensity location/id
	GLuint GetAmbientIntensityLocation();

	// get uniform ambient colour location/id
	GLuint GetAmbientColourLocation();

	// get uniform diffuse intensity location/id
	GLuint GetDiffuseIntensityLocation();
	
	// get uniform firection location/id
	GLuint GetDirectionLocation();

	// use the shader
	void UseShader();

	// clear the shader
	void ClearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection;

	// compile the shader
	void CompileShader(const char* vertexCode, const char* fragmentCode);

	// add shader code
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

