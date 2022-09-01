#include "Snow.h"
#include "Utils.h"
#include <stdlib.h>


Snow::Snow(float normWidth, float normHeight)
{
	Vertex v1;
	v1.Position = { normWidth - (m_Size / 2), normHeight - (m_Size / 2) };
	v1.Color = { 1.0f, 1.0f, 1.0f };

	Vertex v2;
	v2.Position = { normWidth - (m_Size / 2), normHeight + (m_Size / 2) };
	v2.Color = { 1.0f, 1.0f, 1.0f };

	Vertex v3;
	v3.Position = { normWidth + (m_Size / 2), normHeight + (m_Size / 2) };
	v3.Color = { 1.0f, 1.0f, 1.0f };

	Vertex v4;
	v4.Position = { normWidth + (m_Size / 2), normHeight - (m_Size / 2) };
	v4.Color = { 1.0f, 1.0f, 1.0f };

	Vertex v5;
	v5.Position = { normWidth - (m_Size / 2), normHeight - (m_Size / 2) };
	v5.Color = { 1.0f, 1.0f, 1.0f };

	Vertex v6;
	v6.Position = { normWidth + (m_Size / 2), normHeight + (m_Size / 2) };
	v6.Color = { 0.5f, 0.5f, 0.5f };

	Vertex tmp[] = { v1, v2, v3, v4, v5, v6 };
	std::memcpy(&this->snowVerts, &tmp, sizeof(tmp));
}

void Snow::GetVerticesAsArray(float arr[], int offset) {
	int arrayOffset = offset;
	for (int i = 0; i < 6; i++)
	{
		arr[arrayOffset+0] = this->snowVerts[i].Position.x;
		arr[arrayOffset+1] = this->snowVerts[i].Position.y * (-1); // does the trick
		arr[arrayOffset+2] = this->snowVerts[i].Color.x;
		arr[arrayOffset+3] = this->snowVerts[i].Color.y;
		arr[arrayOffset+4] = this->snowVerts[i].Color.z;
		arrayOffset += 5;
	}

}

Snow::~Snow()
{
}

void Snow::TickEvents(GLFWwindow* window, double deltaTime)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	if (CanFallFurther(height))
	{
		Fall(height, deltaTime);
	}
}

bool Snow::CanFallFurther(int windowHeight)
{
	for (Vertex v : this->snowVerts)
	{
		if (v.Position.y > 1.0f) return false;
	}
	return true;

	// TODO: check if other snow is beneath this one
}

void Snow::Fall(int windowHeight, double deltaTime)
{
	float normFallSpeed = m_FallSpeed / windowHeight;

	if (this->snowVerts[5].Position.y + normFallSpeed * deltaTime > 1)
	{
		// If snow would fall below the bottom of window: set to bottom of window
		this->snowVerts[0].Position.y = 1.0f - m_Size;
		this->snowVerts[1].Position.y = 1.0f;
		this->snowVerts[2].Position.y = 1.0f - m_Size;
		this->snowVerts[3].Position.y = 1.0f;
		this->snowVerts[4].Position.y = 1.0f - m_Size;
		this->snowVerts[5].Position.y = 1.0f;

		return;
	}

	Vertex v[6];
	std::memcpy(&v, &this->snowVerts, sizeof(this->snowVerts));
	for (Vertex newV : v)
	{
		float moveDelta = newV.Position.y + normFallSpeed * (float)deltaTime;
		newV.Position.y = moveDelta;
	}
	std::memcpy(&this->snowVerts, &v, sizeof(v));
}
