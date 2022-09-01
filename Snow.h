#pragma once

#ifndef SNOW_CLASS_H
#define SNOW_CLASS_H
#include <glad/glad.h>
#include "Libraries/include/GLFW/glfw3.h"


struct Vec2
{
	float x, y;
};

struct Vec3
{
	float x, y, z;
};

struct Vertex
{
	Vec2 Position;
	Vec3 Color;
};


class Snow
{
public:
	Vertex snowVerts[6];
	

	const float m_FallSpeed = 1.0f;

	const float m_Size = 0.01f;


	Snow(float normWidth, float normHeight);
	~Snow();

	void GetVerticesAsArray(float arr[], int offset = 0);

	void TickEvents(GLFWwindow* window, double deltaTime);

	bool CanFallFurther(int windowHeight);

	void Fall(int windowHeight, double deltaTime);
};

#endif
