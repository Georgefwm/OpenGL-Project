#ifndef CONTROLLER_CLASS_H
#define CONTROLLER_CLASS_H
#include "Libraries/include/GLFW/glfw3.h"

namespace Controller
{
	void HandleMouseInput(GLFWwindow* window) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
	}
};

#endif
