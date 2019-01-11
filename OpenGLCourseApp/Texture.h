#pragma once

#include <GL\glew.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();

	Texture(const char* fileLoc);

	void LoadTexture();

	void UserTexture();

	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;
	const char* fileLocation;
};

