#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <vector>
#include <algorithm>
#include <iterator>

#define WINDOWWIDTH 800
#define WINDOWHEIGHT 800
#define MAXSNOWCOUNT 1000

#include "Texture.h"
#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Snow.h"
#include "Utils.h"


std::vector<Snow> entities;
bool lButtonDown = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			lButtonDown = true;
		else if (GLFW_RELEASE == action)
			lButtonDown = false;
	}	
}

void WhileLMousePressed(GLFWwindow* window)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	if (entities.size() < MAXSNOWCOUNT)
	{
		Snow s = Snow(Utils::Normalise(window, xpos, width), Utils::Normalise(window, ypos, height));
		entities.push_back(s);

	}
}

int main()
{
	entities.clear();
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "Snow", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, WINDOWWIDTH, WINDOWHEIGHT);



	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
	
	float * vertices;

	vertices = (float*)malloc(sizeof(float) * 30 * MAXSNOWCOUNT);

	
	glEnableVertexAttribArray(0);

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	
	
	// Generates Element Buffer Object and links it to indices
	//EBO EBO1(s.snowIndices, sizeof(s.snowIndices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 2, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	//EBO1.Unbind();

	

	// Texture - Not using atm
	//Texture popCat("swirls.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	//popCat.texUnit(shaderProgram, "tex0", 0);

	// Original code from the tutorial
	/*Texture popCat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);*/

	// Binds keys
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	double lastFrame = glfwGetTime();
	

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// get deltatime of this frame
		double currentFrame = glfwGetTime();
		double deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		

		//// Take care of all GLFW events
		glfwPollEvents();

		if (lButtonDown) WhileLMousePressed(window);


		std::vector<int> elemsToRemove;
		for (int i = 0; i < entities.size(); i++)
		{
			Snow& ref = entities[i];
			Snow::TickEvents(window, deltaTime, ref, entities, i);

			// remove expired elements
			if (ref.m_DeathTimer <= 0) elemsToRemove.push_back(i);
		}

		for (int i : elemsToRemove)
		{
			if (i < entities.size())
			{
				entities.erase(entities.begin() + i);
			}
		}
		
		elemsToRemove.clear();

		// Specify the color of the background
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		
		int offset = 0;
		int count = 0;
		for (int i = 0; i < entities.size(); i++)
		{
			Snow& ref = entities[i];
			Snow::GetVerticesAsArray(vertices, ref, offset);
			offset += 30;
			count += 6;
		}

		VBO1.UpdateBuffer(vertices, sizeof(float) * offset);
		

		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawArrays(GL_TRIANGLES, 0, count);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		
		


		//std::cout << "errors: " << glGetError() << std::endl;

	}
	
	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	//EBO1.Delete();
	shaderProgram.Delete();
	free(vertices);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}