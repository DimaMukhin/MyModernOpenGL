#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

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

	// get uniform specular intensity location/id
	GLuint GetSpecularIntensityLocation();

	// get uniform shininess location/id
	GLuint GetShininessLocation();

	// get uniform eye position location/id
	GLuint GetEyePositionLocation();

	void SetDirectionalLight(DirectionalLight *dLight);

	void SetPointLights(PointLight *pLight, unsigned int lightCount);

	void SetSpotLights(SpotLight *sLight, unsigned int lightCount);

	void SetTexture(GLuint textureUnit);

	void SetDirectionalShadowMap(GLuint textureUnit);

	void setDirectionalLightTransform(glm::mat4* lTransform);

	// use the shader
	void UseShader();

	// clear the shader
	void ClearShader();

	~Shader();

private:
	int pointLightCount;
	int spotLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView,
		uniformEyePosition, uniformSpecularIntensity, uniformShininess,
		uniformTexture,
		uniformDirectionalLightTransform, uniformDirectionalShadowMap;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	// compile the shader
	void CompileShader(const char* vertexCode, const char* fragmentCode);

	// add shader code
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

