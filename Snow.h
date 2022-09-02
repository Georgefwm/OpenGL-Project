#pragma once

#ifndef SNOW_CLASS_H
#define SNOW_CLASS_H
#include <glad/glad.h>
#include "Libraries/include/GLFW/glfw3.h"
#include <vector>


class Snow
{
public:
	std::vector<float> m_XVertices;
	std::vector<float> m_YVertices;

	std::vector<float> m_XColor;
	std::vector<float> m_YColor;
	std::vector<float> m_ZColor;

	bool m_Static = false;
	int m_DeathTimer = 10000; // in frames
	const float m_FallSpeed = 100.0f;
	const float m_Size = 0.01f;

	inline std::vector<float>& GetYValues() { return m_YVertices; }


	Snow(float normWidth, float normHeight);
	~Snow();

	void GetVerticesAsArray(float arr[], int offset = 0);

	static void TickEvents(GLFWwindow* window, double deltaTime, Snow& s);

	static bool CanFallFurther(int windowHeight, Snow& s, std::vector<Snow> entities);

	static void Fall(int windowHeight, double deltaTime, Snow& s);

	static void SetBottomCoord(Snow& s, float pos);
};

#endif
