#include "UserInput.h"
#include "Program.h"

#include <iostream>
#include <string>
#include <queue>

std::queue<std::string> UserInput::inputBuffer;

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

	//Update state
	gameState->UIMode = "InGame";

	//Update camera
	float cameraSensitivity = 0.01;
	gameState->camera.rotateHorizontal(gameState->leftStickX * cameraSensitivity);
	gameState->camera.rotateVertical(gameState->leftStickY * cameraSensitivity);
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
		case GLFW_KEY_W:
			UserInput::inputBuffer.push("W");
			break;
		case GLFW_KEY_A:
			UserInput::inputBuffer.push("A");
			break;
		case GLFW_KEY_S:
			UserInput::inputBuffer.push("S");
			break;
		case GLFW_KEY_D:
			UserInput::inputBuffer.push("D");
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
		case GLFW_KEY_SPACE:
			UserInput::inputBuffer.push("SPACE");
			break;
		case GLFW_KEY_ESCAPE:
			UserInput::inputBuffer.push("ESCAPE");
			break;
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
		break;

	case GLFW_RELEASE:
		break;
	}
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

		if (GLFW_PRESS == buttons[0]) { UserInput::inputBuffer.push("A"); };//A
		if (GLFW_PRESS == buttons[1]) { UserInput::inputBuffer.push("B"); };//B
		if (GLFW_PRESS == buttons[2]) { UserInput::inputBuffer.push("X"); };//X
		if (GLFW_PRESS == buttons[3]) { UserInput::inputBuffer.push("Y"); };//Y
		if (GLFW_PRESS == buttons[4]) { UserInput::inputBuffer.push("LB"); };//LB
		if (GLFW_PRESS == buttons[5]) { UserInput::inputBuffer.push("RB"); };//RB
		if (GLFW_PRESS == buttons[6]) { UserInput::inputBuffer.push("LO"); };//left option
		if (GLFW_PRESS == buttons[7]) { UserInput::inputBuffer.push("RO"); };//right option
		if (GLFW_PRESS == buttons[8]) { UserInput::inputBuffer.push("LS"); };//left joy
		if (GLFW_PRESS == buttons[9]) { UserInput::inputBuffer.push("RS"); };//right joy
		if (GLFW_PRESS == buttons[10]) { UserInput::inputBuffer.push("UP"); };//up
		if (GLFW_PRESS == buttons[11]) { UserInput::inputBuffer.push("RIGHT"); };//right
		if (GLFW_PRESS == buttons[12]) { UserInput::inputBuffer.push("DOWN"); };//down
		if (GLFW_PRESS == buttons[13]) { UserInput::inputBuffer.push("LEFT"); };//left

	}
}