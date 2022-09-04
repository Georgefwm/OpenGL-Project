#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <vector>
#include <thread>
#include <algorithm>
#include <iterator>

#define WINDOWWIDTH 800
#define WINDOWHEIGHT 800
#define MAXSNOWCOUNT 10000
#define SNOWSPAWNRATECAP 0.01

#include "Texture.h"
#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Snow.h"
#include "Utils.h"


std::vector<Snow> entities;
bool lButtonDown = false;
bool paused = false;

// Handles mouse button input events
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			lButtonDown = true;
		else if (GLFW_RELEASE == action)
			lButtonDown = false;
	}
}

// Handles keyboard input events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		entities.clear(); // Just delete everything

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		paused = !paused;
}

void WhileLMousePressed(double x, double y, int width, int height)
{
	if (entities.size() < MAXSNOWCOUNT) // Attempt to avoid invalid index errors
	{
		Snow s = Snow(Utils::Normalise(x, width), Utils::Normalise(y, height));
		entities.push_back(s);
	}
}


int WinMain()
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

	// Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "Pixel Snow - By George McLachlan", NULL, NULL);
	if (window == NULL) // make sure it obeys
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, WINDOWWIDTH, WINDOWHEIGHT);

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
	
	// Tet up big buffer for gpu to use
	float * vertices;
	vertices = (float*)malloc(sizeof(float) * 30 * MAXSNOWCOUNT);

	glEnableVertexAttribArray(0);

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 2, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();

	// Texture - Not using atm
	// Texture tex("swirls.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	// tex.texUnit(shaderProgram, "tex0", 0);

	// Binds keys - tell GLFW to listen for these types of inputs
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);

	double lastFrame = glfwGetTime();
	double lastSpawn = glfwGetTime();
	
	
	// Snow s = Snow(0, 0);
	// entities.push_back(s);
	// Good for debugging

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Get deltatime of this frame - good for calculating movement of objects
		double currentFrame = glfwGetTime();
		double deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Take care of all GLFW events
		glfwPollEvents();

		while (paused)
		{
			glfwPollEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			// Important to keep deltaTime running as looks really janky when 
			// you exit the pause loop
			double currentFrame = glfwGetTime();
			double deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
		}

		// This lets you hold down the button to continue spawning snow
		if (lButtonDown && currentFrame-lastSpawn > SNOWSPAWNRATECAP)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			int width, height;
			glfwGetWindowSize(window, &width, &height);

			WhileLMousePressed(xpos, ypos, width, height);
			lastSpawn = currentFrame;
		}

		// Handle the entity tick event updates
		int elemToRemove = -1;
		for (int i = 0; i < entities.size(); i++)
		{
			Snow& ref = entities[i];
			// TODO: Change to some way where I don't have to pass the whole entities vector.
			// Prgogram noticeably slows down when lots of particles are present.
			// Maybe find a way to pass simple collision shapes that represent many stationary particles
			// and just update them.
			if (!ref.IsStationary()) Snow::TickEvents(window, deltaTime, ref, entities, i);
		}

		// TODO: Figure out how to properly remove old particles
		//std::vector<Snow>& ref = entities;
		//auto erased = std::erase_if(entities, [](Snow& a) { return a.remove; });


		// Specify the color of the background
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		// Extract the vertices from objects and get a big array containing all of them
		int offset = 0;
		int count = 0;
		for (int i = 0; i < entities.size(); i++)
		{
			Snow& ref = entities[i];
			Snow::GetVerticesAsArray(vertices, ref, offset);
			offset += 30;
			count += 6;
		}

		// Chuck all the vertices into the 'big' array for gpu
		VBO1.UpdateBuffer(vertices, sizeof(float) * offset);
		VAO1.Bind();

		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawArrays(GL_TRIANGLES, 0, count);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
	}
	
	// Delete all the leftovers
	VAO1.Delete();
	VBO1.Delete();
	shaderProgram.Delete();
	free(vertices);

	// Delete window before ending the program
	glfwDestroyWindow(window);

	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;
}