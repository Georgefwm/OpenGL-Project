#pragma once

#ifndef SNOW_CLASS_H
#define SNOW_CLASS_H
#include <glad/glad.h>
#include "Libraries/include/GLFW/glfw3.h"
#include <vector>

struct RECT
{
	float top;
	float bottom;
	float left;
	float right;
};


class Snow
{
public:
	std::vector<float> m_XVertices;
	std::vector<float> m_YVertices;

	std::vector<float> m_XColor;
	std::vector<float> m_YColor;
	std::vector<float> m_ZColor;

	bool m_Stationary;
	int m_DeathTimer; 
	bool remove;
	const float m_FallSpeed = 200.0f;
	const float m_Size = 0.01f;
	float orderingValue;

	inline std::vector<float>& GetYValues() { return m_YVertices; }

	Snow(float normWidth, float normHeight);
	~Snow();

	Snow& operator=(const Snow& src)
	{
		// deliberately does not assign a = src.a
		return *this;
	}

	static void GetVerticesAsArray(float arr[], Snow& s, int offset);

	static void TickEvents(GLFWwindow* window, double deltaTime, Snow& s, std::vector<Snow>& entities, int index);

	static void Fall(int windowHeight, double deltaTime, Snow& s, std::vector<Snow>& entities);

	static void SetBottomCoord(Snow& s, float pos);

	static float GetBottomYPos(Snow& s);

	static RECT GetSides(Snow& s);

	void SetStationary();

	bool IsStationary();

};

#endif
