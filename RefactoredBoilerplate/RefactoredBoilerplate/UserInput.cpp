#include "UserInput.h"
#include "Program.h"

#include <iostream>

UserInput::UserInput()
{
}


UserInput::~UserInput()
{
}

int UserInput::Update(Gamestate gameState)
{
	//Implementation
	return 0;
}

// Callback for key presses
void UserInput::key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	switch (key) {
	case GLFW_KEY_W: std::cout << 'W';
		break;
	case GLFW_KEY_A: std::cout << 'A';
		break;
	case GLFW_KEY_S: std::cout << 'S';
		break;
	case GLFW_KEY_D: std::cout << 'D';
		break;
	}
}