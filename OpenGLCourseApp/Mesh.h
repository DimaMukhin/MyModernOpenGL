#pragma once

#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	// create the mesh
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);

	// draw the mesh
	void RenderMesh();

	// delete from the graphics card
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

