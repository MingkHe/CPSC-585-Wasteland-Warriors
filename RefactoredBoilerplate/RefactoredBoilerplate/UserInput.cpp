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
	//Update state based on menu
	return 0;
}

// Callback for key presses
void UserInput::key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {

		//This closes game. We may want to add aditional functionality to the escape key.
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	switch (action) {

	case GLFW_PRESS:

		switch (key) {
		
			//WASD
		case GLFW_KEY_W: std::cout << 'W';
			break;
		case GLFW_KEY_A: std::cout << 'A';
			break;
		case GLFW_KEY_S: std::cout << 'S';
			break;
		case GLFW_KEY_D: std::cout << 'D';
			break;

			//Arrows
		case GLFW_KEY_RIGHT: std::cout << "RIGHT";
			break;
		case GLFW_KEY_LEFT: std::cout << "LEFT";
			break;
		case GLFW_KEY_DOWN: std::cout << "DOWN";
			break;
		case GLFW_KEY_UP: std::cout << "UP";
			break;

			//Controls
		case GLFW_KEY_SPACE: std::cout << "SPACE";
			break;
		case GLFW_KEY_ESCAPE: std::cout << "ESCAPE";
			break;
		case GLFW_KEY_ENTER: std::cout << "ENTER";
			break;
		case GLFW_KEY_LEFT_SHIFT: std::cout << "LSHIFT";
			break;
		case GLFW_KEY_RIGHT_SHIFT: std::cout << "RSHIFT";
			break;
		}
	break;

	case GLFW_REPEAT:
		break;
	case GLFW_RELEASE:
		break;
	}
}