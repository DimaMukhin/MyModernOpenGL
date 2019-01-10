#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

	// creating our VAO
	glGenVertexArrays(1, &VAO); // amount and where to store the id. VAO is metadata of vertices
	glBindVertexArray(VAO); // we need to bind it (see theory lecture for more details)

	glGenBuffers(1, &IBO); // amount and where to store...
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // bind it...
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW); // setting locations in the IBO

	// creating out VBO
	glGenBuffers(1, &VBO); // same as VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind our VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); // theory: VBO holds vertex locations. Static means we dont change (more theory)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);	// this is where we explain how <vertices> is structured (see documentation if you dont understand)
															// we say that each vertex is 3 values long, values are floats, we dont want to normalize them, no stride, start from 0
															// the first 0 here is very important, it will point at a 0 in out shader! (shader location)
															// shader location is what you use to input values into the shader
	glEnableVertexAttribArray(0); // enable the thing above so the shader can access it (first 0 matches the 0 here) (shader location)

	glBindBuffer(GL_ARRAY_BUFFER, 0); // bind to nothing AKA un-bind VBO
	glBindVertexArray(0); // bind to 0 AKA un-bind VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbinding IBO
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO); // work with VAO we created (it is connected with VBO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // work with IBO we created
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); // use IBO to draw trianges, 12 indices, we used unsigned ints, start from 0
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // un-bind out IBO
	glBindVertexArray(0); // un-bind our triangle VAO
}

void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

Mesh::~Mesh()
{
	ClearMesh();
}
