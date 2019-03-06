#include "UserInput.h"
#include "Program.h"

#include <iostream>
#include <string>
#include <queue>

std::queue<std::string> UserInput::inputBuffer;

double UserInput::MouseXpos;
double UserInput::MouseYpos;
bool UserInput::MousePressed;

//WASD
bool UserInput::WKey;
bool UserInput::AKey;
bool UserInput::SKey;
bool UserInput::DKey;
bool UserInput::SPACEKey;

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

	//WASD car control
	if (UserInput::WKey == true) {
		gameState->WKey = true;
	}
	else {
		gameState->WKey = false;
	}
	if (UserInput::AKey == true) {
		gameState->AKey = true;
	}
	else {
		gameState->AKey = false;
	}
	if (UserInput::SKey == true) {
		gameState->SKey = true;
	}
	else{
		gameState->SKey = false;
	}
	if (UserInput::DKey == true) {
		gameState->DKey = true;
	}
	else {
		gameState->DKey = false;
	}
	if (UserInput::SPACEKey == true) {
		gameState->SPACEKey = true;
	}
	else {
		gameState->SPACEKey = false;
	}

	//reset orentation
	if (gameState->button == "R" || gameState->button == "Y") {
		gameState->resetOrientation();
	}

	if (gameState->leftStickY == 1) {
		UserInput::inputBuffer.push("UP");
	}
	if (gameState->leftStickY == -1) {
		UserInput::inputBuffer.push("DOWN");
	}

	if (UserInput::MouseXpos != oldMouseXpos) {
		if (UserInput::MousePressed) {
			float angle = gameState->cameraAngle + (oldMouseXpos - UserInput::MouseXpos) * 0.01;
			if (angle < 1.5 && angle > -1.5) {
				gameState->cameraAngle = angle;
			}
			oldMouseXpos = UserInput::MouseXpos;
		}
		else {
			if (gameState->cameraAngle > 0.05){
				gameState->cameraAngle = gameState->cameraAngle - 0.02;
		}
		else if (gameState->cameraAngle < -0.05) {
				gameState->cameraAngle = gameState->cameraAngle + 0.02;
			}
		else {
				gameState->cameraAngle = 0;
			}
		}
	}
}

// Callback for key presses
void UserInput::key(GLFWwindow* window, int key, int scancode, int action, int mods) {

	switch (action) {

	case GLFW_PRESS:

		switch (key) {

			//Car Controls
		case GLFW_KEY_W:
			UserInput::WKey = true;
			break;
		case GLFW_KEY_A:
			UserInput::AKey = true;
			break;
		case GLFW_KEY_S:
			UserInput::SKey = true;
			break;
		case GLFW_KEY_D:
			UserInput::DKey = true;
			break;
		case GLFW_KEY_SPACE:
			UserInput::SPACEKey = true;
			break;

			//Escape
		case GLFW_KEY_ESCAPE:
			UserInput::inputBuffer.push("ESC");
			break;

			//Testing Letters
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
		case GLFW_KEY_R:
			UserInput::inputBuffer.push("R");
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

	case GLFW_RELEASE:
	{
		switch (key) {
			//Car Controls
		case GLFW_KEY_W:
			UserInput::WKey = false;
			break;
		case GLFW_KEY_A:
			UserInput::AKey = false;
			break;
		case GLFW_KEY_S:
			UserInput::SKey = false;
			break;
		case GLFW_KEY_D:
			UserInput::DKey = false;
			break;
		case GLFW_KEY_SPACE:
			UserInput::SPACEKey = false;
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

void UserInput::mouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		UserInput::MousePressed = true;
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		UserInput::MousePressed = false;
	}
}

void UserInput::gamepad(int controller, Gamestate* gameState) {

	//Controller 1
	if (controller == 1) {


		//get Joystick for Mapping
		const char* name = glfwGetJoystickName(GLFW_JOYSTICK_1);
		//std::cout << name << std::endl;

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
			UserInput::inputBuffer.push("ENTER");
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
			UserInput::inputBuffer.push("M");
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

		//PS4 Controller mapping
		/*if (name == "Wireless Controller") {
			float lt = gameState->rightStickY;
			float rt = gameState->leftTrigger;
			float ry = gameState->rightTrigger;

			gameState->rightStickY = ry;
			gameState->leftTrigger = lt;
			gameState->rightTrigger = rt;
		}*/
	}
}