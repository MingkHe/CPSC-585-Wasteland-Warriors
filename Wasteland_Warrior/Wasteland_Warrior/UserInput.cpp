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
	leftStickX = 0.0;
	leftStickY = 0.0;
	rightStickX = 0.0;
	rightStickY = 0.0;
	leftTrigger = 0.0;
	rightTrigger = 0.0;
	gamepad(glfwJoystickPresent(GLFW_JOYSTICK_1));

	//Get input from buffer
	if (UserInput::inputBuffer.size() > 0) {
		gameState->button = UserInput::inputBuffer.front();
		UserInput::inputBuffer.pop();
	}

	//Update state
	gameState->state = "inGame";
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

void UserInput::gamepad(int controller) {

	//Controller 1
	if (controller == 1) {

		//Gamepad joystick and triggers 
		int axesCount;
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

		//Joysticks
		leftStickX = axes[0];
		leftStickY = axes[1];
		rightStickX = axes[2];
		rightStickY = axes[3];

		//Triggers
		leftTrigger = axes[4];
		rightTrigger = axes[5];

		//Gamepad buttons
		int buttonCount;
		const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

		if (GLFW_PRESS == buttons[0]) { UserInput::inputBuffer.push("S"); };//A
		if (GLFW_PRESS == buttons[1]) { UserInput::inputBuffer.push("D"); };//B
		if (GLFW_PRESS == buttons[2]) { UserInput::inputBuffer.push("A"); };//X
		if (GLFW_PRESS == buttons[3]) { UserInput::inputBuffer.push("W"); };//Y
		if (GLFW_PRESS == buttons[4]) { UserInput::inputBuffer.push("LSHIFT"); };//LB
		if (GLFW_PRESS == buttons[5]) { UserInput::inputBuffer.push("RSHIFT"); };//RB
		if (GLFW_PRESS == buttons[6]) { UserInput::inputBuffer.push(""); };//left option
		if (GLFW_PRESS == buttons[7]) { UserInput::inputBuffer.push(""); };//right option
		if (GLFW_PRESS == buttons[8]) { UserInput::inputBuffer.push(""); };//left joy
		if (GLFW_PRESS == buttons[9]) { UserInput::inputBuffer.push(""); };//right joy
		if (GLFW_PRESS == buttons[10]) { UserInput::inputBuffer.push("UP"); };//up
		if (GLFW_PRESS == buttons[11]) { UserInput::inputBuffer.push("RIGHT"); };//right
		if (GLFW_PRESS == buttons[12]) { UserInput::inputBuffer.push("DOWN"); };//down
		if (GLFW_PRESS == buttons[13]) { UserInput::inputBuffer.push("LEFT"); };//left

	}
}