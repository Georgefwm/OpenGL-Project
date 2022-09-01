#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO
{
public:
	GLuint ID;
	VBO(float* vertices, GLsizeiptr size);

	void UpdateBuffer(float* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif
