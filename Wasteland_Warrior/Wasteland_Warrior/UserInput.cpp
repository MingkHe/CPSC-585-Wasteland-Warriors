#include "UserInput.h"
#include "Program.h"

#include <iostream>
#include <string>
#include <queue>

std::queue<std::string> UserInput::inputBuffer;

float UserInput::MouseXpos;
float UserInput::MouseYpos;

//WASD
bool UserInput::W;
bool UserInput::A;
bool UserInput::S;
bool UserInput::D;
bool UserInput::SPACE;

UserInput::UserInput()
{
}


UserInput::~UserInput()
{
}

void UserInput::Update(Gamestate* gameState)
{
	//Gamepad input
	gameState->leftStickX = 0.0;
	gameState->leftStickY = 0.0;
	gameState->rightStickX = 0.0;
	gameState->rightStickY = 0.0;
	gameState->leftTrigger = 0.0;
	gameState->rightTrigger = 0.0;
	gamepad(glfwJoystickPresent(GLFW_JOYSTICK_1), gameState);

	//Get input from buffer
	if (UserInput::inputBuffer.size() > 0) {
		gameState->button = UserInput::inputBuffer.front();
		UserInput::inputBuffer.pop();
	}
	else {
		gameState->button = "";
	}

	//WASD
	if (UserInput::W == true) {
		gameState->W = true;
	}
	else {
		gameState->W = false;
	}
	if (UserInput::A == true) {
		gameState->A = true;
	}
	else {
		gameState->A = false;
	}
	if (UserInput::S == true) {
		gameState->S = true;
	}
	else{
		gameState->S = false;
	}
	if (UserInput::D == true) {
		gameState->D = true;
	}
	else {
		gameState->D = false;
	}
	if (UserInput::SPACE == true) {
		gameState->SPACE = true;
	}
	else {
		gameState->SPACE = false;
	}


	//Update state
	gameState->UIMode = "InGame";

	//Update camera
	gameState->camera.rotateHorizontal(gameState->leftStickX * cameraSensitivity);
	gameState->camera.rotateVertical(gameState->leftStickY * cameraSensitivity);

	if (UserInput::MouseXpos != oldMouseXpos) {
		gameState->camera.rotateHorizontal((oldMouseXpos - UserInput::MouseXpos) * cameraSensitivity);
		oldMouseXpos = UserInput::MouseXpos;
	}
	if (UserInput::MouseYpos != oldMouseYpos) {
		gameState->camera.rotateVertical((oldMouseYpos - UserInput::MouseYpos) * cameraSensitivity);
		oldMouseYpos = UserInput::MouseYpos;
	}

}

// Callback for key presses
void UserInput::key(GLFWwindow* window, int key, int scancode, int action, int mods) {

	switch (action) {

	case GLFW_PRESS:

		switch (key) {

			//Escape
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		case GLFW_KEY_T:
			UserInput::inputBuffer.push("T");
			break;
		case GLFW_KEY_F:
			UserInput::inputBuffer.push("F");
			break;
		case GLFW_KEY_G:
			UserInput::inputBuffer.push("G");
			break;
		case GLFW_KEY_H:
			UserInput::inputBuffer.push("H");
			break;
		case GLFW_KEY_M:
			UserInput::inputBuffer.push("M");
			break;
		case GLFW_KEY_N:
			UserInput::inputBuffer.push("N");
			break;

			//Arrows
		case GLFW_KEY_RIGHT:
			UserInput::inputBuffer.push("RIGHT");
			break;
		case GLFW_KEY_LEFT:
			UserInput::inputBuffer.push("LEFT");
			break;
		case GLFW_KEY_DOWN:
			UserInput::inputBuffer.push("DOWN");
			break;
		case GLFW_KEY_UP:
			UserInput::inputBuffer.push("UP");
			break;

			//Controls
		case GLFW_KEY_ENTER:
			UserInput::inputBuffer.push("ENTER");
			break;
		case GLFW_KEY_LEFT_SHIFT:
			UserInput::inputBuffer.push("LSHIFT");
			break;
		case GLFW_KEY_RIGHT_SHIFT:
			UserInput::inputBuffer.push("RSHIFT");
			break;
		}
		break;

	case GLFW_REPEAT:
	{
		switch (key) {
			//Car Controls
		case GLFW_KEY_W:
			UserInput::W = true;
			break;
		case GLFW_KEY_A:
			UserInput::A = true;
			break;
		case GLFW_KEY_S:
			UserInput::S = true;
			break;
		case GLFW_KEY_D:
			UserInput::D = true;
			break;
		case GLFW_KEY_SPACE:
			UserInput::SPACE = true;
			break;
		}
	}
		break;

	case GLFW_RELEASE:
	{
		switch (key) {
			//Car Controls
		case GLFW_KEY_W:
			UserInput::W = false;
			break;
		case GLFW_KEY_A:
			UserInput::A = false;
			break;
		case GLFW_KEY_S:
			UserInput::S = false;
			break;
		case GLFW_KEY_D:
			UserInput::D = false;
			break;
		case GLFW_KEY_SPACE:
			UserInput::SPACE = false;
			break;
		}
	}
		break;
	}
}

void UserInput::cursor(GLFWwindow* window, double xpos, double ypos)
{
	UserInput::MouseXpos = xpos;
	UserInput::MouseYpos = ypos;
}

void UserInput::gamepad(int controller, Gamestate* gameState) {

	//Controller 1
	if (controller == 1) {

		//Gamepad joystick and triggers 
		int axesCount;
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

		//Joysticks
		gameState->leftStickX = axes[0];
		gameState->leftStickY = axes[1];
		gameState->rightStickX = axes[2];
		gameState->rightStickY = axes[3];

		//Triggers
		gameState->leftTrigger = axes[4];
		gameState->rightTrigger = axes[5];

		//Gamepad buttons
		int buttonCount;
		const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

		if (GLFW_PRESS == buttons[0]) { 
			UserInput::inputBuffer.push("A"); 
		};
		if (GLFW_PRESS == buttons[1]) { 
			UserInput::inputBuffer.push("B"); 
		};
		if (GLFW_PRESS == buttons[2]) {
			UserInput::inputBuffer.push("X"); 
		};
		if (GLFW_PRESS == buttons[3]) { 
			UserInput::inputBuffer.push("Y"); 
		};
		if (GLFW_PRESS == buttons[4]) { 
			UserInput::inputBuffer.push("LB"); 
		};
		if (GLFW_PRESS == buttons[5]) { 
			UserInput::inputBuffer.push("RB"); 
		};
		if (GLFW_PRESS == buttons[6]) { 
			UserInput::inputBuffer.push("LO"); 
		};
		if (GLFW_PRESS == buttons[7]) { 
			UserInput::inputBuffer.push("RO");
		};
		if (GLFW_PRESS == buttons[8]) { 
			UserInput::inputBuffer.push("LS");
		};
		if (GLFW_PRESS == buttons[9]) { 
			UserInput::inputBuffer.push("RS");
		};
		if (GLFW_PRESS == buttons[10]) { 
			UserInput::inputBuffer.push("UP");
		};
		if (GLFW_PRESS == buttons[11]) { 
			UserInput::inputBuffer.push("RIGHT");
		};
		if (GLFW_PRESS == buttons[12]) { 
			UserInput::inputBuffer.push("DOWN");
		};
		if (GLFW_PRESS == buttons[13]) { 
			UserInput::inputBuffer.push("LEFT");
		};

	}
}