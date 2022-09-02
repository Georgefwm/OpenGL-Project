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
	this->m_DeathTimer = 500;
	this->m_Stationary = false;
	this->remove = false;
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

void Snow::SetStationary()
{
	this->m_Stationary = true;
}

bool Snow::IsStationary()
{
	return this->m_Stationary;
}

void Snow::TickEvents(GLFWwindow* window, double deltaTime, Snow& s, std::vector<Snow>& entities, int index)
{
	if (!s.IsStationary())
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		Fall(height, deltaTime, s, entities);
	}

	// TODO: fixing weird deleting behaviour
	/*else
	{
		if (s.m_DeathTimer <= 0) s.remove = true;
		else s.m_DeathTimer = s.m_DeathTimer - 1;
	}*/
}

// Handles all aspects of falling
void Snow::Fall(int windowHeight, double deltaTime, Snow& s, std::vector<Snow>& entities)
{
	float normFallSpeed = s.m_FallSpeed / windowHeight;


	RECT RectA = Snow::GetSides(s);
	
	for (int i = 0; i < entities.size(); i++)
	{	
		// make sure to only collide with stationary snow, to avoid mid-air freezing
		if (entities[i].IsStationary() && !s.IsStationary())
		{
			Snow& ref = entities[i];
			RECT RectB = Snow::GetSides(ref);

			// if within some y range
			if (GetBottomYPos(s) + normFallSpeed * deltaTime > RectB.top)
			{	
				// then check if within some x range
				if (sqrt(pow((RectA.left + (s.m_Size / 2)) - (RectB.left + (s.m_Size / 2)), 2)) < s.m_Size/1.5)  // not perfect but looks really weird when not divided
				{
					SetBottomCoord(s, GetBottomYPos(s) + normFallSpeed * deltaTime);
					s.SetStationary();
					return;
				}
			}
		}
		
	}

	// If snow would fall below the bottom of window: set to bottom of window
	if (GetBottomYPos(s) + normFallSpeed * deltaTime > 1.0f)
	{
		SetBottomCoord(s, 1.0f);
		s.SetStationary();
		return;
	}

	// usual case: just move vertices down the screen
	// could add velocity later :/
	for (float& y : s.GetYValues())
	{
		float tmp = y + normFallSpeed * deltaTime;
		 y = tmp;
	}
	s.orderingValue = s.m_YVertices[5];
}


// set the bottom y coords to a set point, usefull for when shit hits other things or the window border
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


// guess...
float Snow::GetBottomYPos(Snow& s)
{
	return s.m_YVertices[5];
}


// usefull for calculating collisions between particles
RECT Snow::GetSides(Snow& s)
{
	RECT r;
	r.left = s.m_XVertices[1];
	r.right = s.m_XVertices[0];
	r.top = s.m_YVertices[0];
	r.bottom = s.m_YVertices[1];
	return r;
}



