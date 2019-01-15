#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

bool Texture::LoadTexture()
{
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textureID); // generating a texture
	glBindTexture(GL_TEXTURE_2D, textureID); // binding the texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // handling overflow in x axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // handling overflow in y axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // try to blend texels when you zoon out  oftexture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // try to blend texels when you zoon into texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData); // load image into our bound texture
	glGenerateMipmap(GL_TEXTURE_2D); // generate mipmaps for us

	glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture

	stbi_image_free(texData); // release image data from memory?

	return true;
}

bool Texture::LoadTextureA()
{
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textureID); // generating a texture
	glBindTexture(GL_TEXTURE_2D, textureID); // binding the texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // handling overflow in x axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // handling overflow in y axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // try to blend texels when you zoon out  oftexture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // try to blend texels when you zoon into texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); // load image into our bound texture
	glGenerateMipmap(GL_TEXTURE_2D); // generate mipmaps for us

	glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture

	stbi_image_free(texData); // release image data from memory?

	return true;
}

void Texture::UserTexture()
{
	glActiveTexture(GL_TEXTURE0); // fShader has access to empty textures, we will enable this one
	glBindTexture(GL_TEXTURE_2D, textureID); // bind the texture we created to GL_TEXTURE0 so it is no longer empty and fShader can use it
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::~Texture()
{
	ClearTexture();
}
