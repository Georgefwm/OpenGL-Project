#pragma once

#ifndef UTILS_H
#define UTILS_H
#include "Libraries/include/GLFW/glfw3.h"
#include <list>
#include "Utils.h"
#include <list>

namespace Utils
{
	// normalises the value supplied to a range of [-1, 1]
	inline float Normalise(GLFWwindow* window, float x, int max)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		return 2 * ((x - 0) / (max - 0)) - 1;
	}
}


#endif