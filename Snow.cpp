#include "Snow.h"
#include "Utils.h"
#include <iostream>


Snow::Snow(float normWidth, float normHeight)
{
	float left = normWidth - (m_Size / 2);
	float top = normHeight - (m_Size / 2);

	this->m_XVertices = { left,				left,			left + m_Size,		left + m_Size,		left,	left + m_Size };
	this->m_YVertices = { top,				top + m_Size,	top + m_Size,		top,				top,	top + m_Size };

	this->m_XColor = { 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, };
	this->m_YColor = { 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, };
	this->m_ZColor = { 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, };

	this->orderingValue = m_YVertices[5];
}


void Snow::GetVerticesAsArray(float arr[], Snow& s, int offset) {
	int arrayOffset = offset;
	for (int i = 0; i < 6; i++)
	{
		arr[arrayOffset+0] = s.m_XVertices[i];
		arr[arrayOffset+1] = s.m_YVertices[i] * (-1); // does the trick
		arr[arrayOffset+2] = s.m_XColor[i];
		arr[arrayOffset+3] = s.m_YColor[i];
		arr[arrayOffset+4] = s.m_ZColor[i];
		arrayOffset += 5;
	}
}

Snow::~Snow()
{
}

void Snow::TickEvents(GLFWwindow* window, double deltaTime, Snow& s, std::vector<Snow>& entities, int index)
{
	if (!s.m_Static)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		//if (!IsColliding(s, entities, index))
		//{
			Fall(height, deltaTime, s);
		//}
	}
	else s.m_DeathTimer--;
}

bool Snow::IsColliding(Snow& s, std::vector<Snow> entities, int index)
{
	std::vector<float> thisSides = Snow::GetSides(s);
	for (int i = index; i < entities.size(); i++)
	{
		if (entities[i].m_Static)
		{
			std::vector<float> otherSides = Snow::GetSides(entities[i]);
			//	  this.bottom > other.top   //   this.left > other.right     //   this.right < other.right
			if (thisSides[3] > otherSides[2] && thisSides[0] < otherSides[1] && thisSides[0] < otherSides[1])
			{

			}
		}
	}
	return false;

	// TODO: check if other snow is beneath this one
}

void Snow::Fall(int windowHeight, double deltaTime, Snow& s)
{
	float normFallSpeed = s.m_FallSpeed / windowHeight;

	// If snow would fall below the bottom of window: set to bottom of window
	if (GetBottomYPos(s) + normFallSpeed * deltaTime > 1.0f)
	{
		SetBottomCoord(s, 1.0f);
		s.m_Static = true;
		return;
	}

	// usual case: just move vertices down the screen
	for (float& y : s.GetYValues())
	{
		float tmp = y + normFallSpeed * deltaTime;

		 y = tmp;
	}
	s.orderingValue = s.m_YVertices[5];
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
	s.orderingValue = s.m_YVertices[5];
}

float Snow::GetBottomYPos(Snow& s)
{
	return s.m_YVertices[5];
}

std::vector<float> Snow::GetSides(Snow& s)
{
	std::vector<float> sides = { s.m_XVertices[1] , s.m_XVertices[0], s.m_YVertices[0], s.m_YVertices[1] };
	return sides;
}



