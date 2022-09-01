#include "VBO.h"
#include <iostream>

VBO::VBO(float* vertices, GLsizeiptr size)
{
	//glGenVertexArrays(1, &this->ID);
	glGenBuffers(1, &this->ID);
	
	//glBindVertexArray(this->ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->ID);

	glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_DYNAMIC_DRAW);
	
}

void VBO::UpdateBuffer(float* vertices, GLsizeiptr size)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->ID);
	glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_DYNAMIC_DRAW);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}
