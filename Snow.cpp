#include "Snow.h"
#include "Utils.h"
#include <iostream>


Snow::Snow(float normWidth, float normHeight)
{
	float left = normWidth - (m_Size / 2);
	float top = normHeight - (m_Size / 2);

	this->m_XVertices = { left + m_Size, left, left + m_Size, left + m_Size, left, left + m_Size };
	this->m_YVertices = { top, top + m_Size, top + m_Size, top, top, top + m_Size };

	this->m_XColor = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, };
	this->m_YColor = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, };
	this->m_ZColor = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, };	
}

void Snow::GetVerticesAsArray(float arr[], int offset) {
	int arrayOffset = offset;
	for (int i = 0; i < 6; i++)
	{
		arr[arrayOffset+0] = this->m_XVertices[i];
		arr[arrayOffset+1] = this->m_YVertices[i] * (-1); // does the trick
		arr[arrayOffset+2] = this->m_XColor[i];
		arr[arrayOffset+3] = this->m_YColor[i];
		arr[arrayOffset+4] = this->m_ZColor[i];
		arrayOffset += 5;
	}
}

Snow::~Snow()
{
}

void Snow::TickEvents(GLFWwindow* window, double deltaTime, Snow& s)
{
	if (!s.m_Static)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		Fall(height, deltaTime, s);
	}
}

bool Snow::CanFallFurther(int windowHeight, Snow& s, std::vector<Snow> entities)
{
	for (float& f : s.m_YVertices)
	{
		if (f > 1.0f) return false;
	}
	return true;

	// TODO: check if other snow is beneath this one
}

void Snow::Fall(int windowHeight, double deltaTime, Snow& s)
{
	float normFallSpeed = s.m_FallSpeed / windowHeight;

	if (s.m_YVertices[5] + normFallSpeed * deltaTime > 1.0f)
	{
		// If snow would fall below the bottom of window: set to bottom of window
		SetBottomCoord(s, 1.0f);
		s.m_Static = true;
	}
	for (float& y : s.GetYValues())
	{
		float tmp = y + normFallSpeed * deltaTime;

		 y = tmp;
	}
}

void Snow::SetBottomCoord(Snow& s, float pos) {
	bool topXValues[6] = { true, false, false, true, true, false };
	int i = 0;
	for (auto& y : s.GetYValues())
	{
		if (topXValues[i]) s.m_YVertices[i] = pos - s.m_Size;
		else s.m_YVertices[i] = pos;
		i++;
	}
}
