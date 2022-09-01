#pragma once

#ifndef UTILS_H
#define UTILS_H
#include "Libraries/include/GLFW/glfw3.h"
#include <list>
#include "Utils.h"
#include <list>

namespace Utils
{
	inline float Normalise(GLFWwindow* window, float x, int max)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		return 2 * ((x - 0) / (max - 0)) - 1;
	}

	inline void GetUpdatedWorldBuffer(std::list<Snow> es, float* arr)
	{
		/*std::list<float> vBuf;
		for (Snow s : es)
		{
			for (int i = 0; i < 24; i++)
			{
				vBuf.push_back(s.snowVerts[i]);
			}
		}
		*arr = new GLfloat[2400];
		int k = 0;
		for (float i : vBuf) {
			(*arr)[k++] = i;
		}
	}*/
	}
}


#endif