#include "UserInput.h"
#include "Program.h"

#include <iostream>
#include <string>
#include <queue>

//Input Buffer
std::queue<std::string> UserInput::inputBuffer;

//Mouse
float UserInput::MouseXpos;
float UserInput::MouseYpos;
bool UserInput::MouseLeft;
bool UserInput::MouseRight;

//WASD
bool UserInput::WKey;
bool UserInput::AKey;
bool UserInput::SKey;
bool UserInput::DKey;
bool UserInput::SPACEKey;

bool UserInput::Reverse;

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

	//Reset Orientation
	if (gameState->button == "R" || gameState->button == "Y") {
		gameState->resetOrientation();
	}

	//Camera view
	if (gameState->button == "VIEW") {
		if (gameState->view == 2) {
			gameState->view = 0;
		}
		else {
			gameState->view++;
		}
	}

	//Mouse Input
	if (UserInput::MouseLeft) {
		if (UserInput::MouseXpos != oldMouseXpos) {
			float angle = gameState->cameraAngle + (oldMouseXpos - UserInput::MouseXpos) * 0.01f;
			if (angle < 1.5 && angle > -1.5) {
				gameState->cameraAngle = angle;
			}
			oldMouseXpos = UserInput::MouseXpos;
		}
	}
	else {
		if (gameState->cameraAngle > 0.05){
			gameState->cameraAngle = gameState->cameraAngle - 0.02f;
		}
		else if (gameState->cameraAngle < -0.05) {
			gameState->cameraAngle = gameState->cameraAngle + 0.02f;
		}
		else {
				gameState->cameraAngle = 0;
		}
	}
	if (UserInput::MouseRight) {
		gameState->mouseRight = true;
	}
	else {
		gameState->mouseRight = false;
	}

	if (UserInput::Reverse) {
		UserInput::inputBuffer.push("REVERSE");
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

			//Pause Menu
		case GLFW_KEY_M:
			UserInput::inputBuffer.push("MENU");
			break;

			//Change View
		case GLFW_KEY_V:
			UserInput::inputBuffer.push("VIEW");
			break;

			//Reverse Cam
		case GLFW_KEY_E:
			UserInput::Reverse = true;
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

			//Testing Input
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
		case GLFW_KEY_N:
			UserInput::inputBuffer.push("N");
			break;
		case GLFW_KEY_R:
			UserInput::inputBuffer.push("R");
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

		case GLFW_KEY_E:
			UserInput::Reverse = false;
			break;
		}
	}
		break;
	}
}

void UserInput::cursor(GLFWwindow* window, double xpos, double ypos)
{
	UserInput::MouseXpos = (float)xpos;
	UserInput::MouseYpos = (float)ypos;
}

void UserInput::mouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		UserInput::MouseLeft = true;
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		UserInput::MouseLeft = false;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		UserInput::MouseRight = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		UserInput::MouseRight = false;
	}
}

void UserInput::gamepad(int controller, Gamestate* gameState) {

	//Controller is connected
	if (controller == 1) {
		gameState->controller = true;

		//Generic Controller Support
		bool Generic = false;
		std::string name = glfwGetJoystickName(GLFW_JOYSTICK_1);
		if (name == "Wireless Controller") { Generic = true; }

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
			if(Generic == true){
				UserInput::inputBuffer.push("A");
			}
			else {
				UserInput::inputBuffer.push("B");
			}
		};
		if (GLFW_PRESS == buttons[2]) {
			if (Generic == true) {
				UserInput::inputBuffer.push("B");
			}
			else {
				UserInput::inputBuffer.push("X");
			}
		};
		if (GLFW_PRESS == buttons[3]) { 
			UserInput::inputBuffer.push("Y");
		};
		if (GLFW_PRESS == buttons[4]) { 
			UserInput::inputBuffer.push("REVERSE");
		};
		if (GLFW_PRESS == buttons[5]) {
			if (view == true) {
				UserInput::inputBuffer.push("VIEW");
			}
			view = false;
		};
		if (GLFW_RELEASE == buttons[5]) {
			view = true;
		};
		if (GLFW_PRESS == buttons[6]) { 
			UserInput::inputBuffer.push("OPTION");
		};
		if (GLFW_PRESS == buttons[7] && Generic == false) { 
			UserInput::inputBuffer.push("MENU");
		};
		if (GLFW_PRESS == buttons[8]) { 
			UserInput::inputBuffer.push("LS");
		};
		if (GLFW_PRESS == buttons[9]) { 
			if (Generic == true) {
				UserInput::inputBuffer.push("MENU");
			}
			else {
				UserInput::inputBuffer.push("RS");
			}
		};
		if (GLFW_PRESS == buttons[10] && Generic == false) {
			if (up == true) {
				UserInput::inputBuffer.push("UP");
			}
			up = false;
		};
		if (GLFW_RELEASE == buttons[10]) {
			up = true;
		};
		if (GLFW_PRESS == buttons[11]) {
			UserInput::inputBuffer.push("RIGHT");
		};
		if (GLFW_PRESS == buttons[12] && Generic == false) {
			if (down == true) {
				UserInput::inputBuffer.push("DOWN");
			}
			down = false;
		};
		if (GLFW_RELEASE == buttons[12] && Generic == false) {
			down = true;
		};
		if (GLFW_PRESS == buttons[13]) {
			UserInput::inputBuffer.push("LEFT");
		};

		//Generic Dpad
		if (GLFW_PRESS == buttons[14]) {
			if (up == true) {
				UserInput::inputBuffer.push("UP");
			}
			up = false;
		};
		if (GLFW_RELEASE == buttons[14]) {
			up = true;
		};
		if (GLFW_PRESS == buttons[15]) {
			UserInput::inputBuffer.push("RIGHT");
		};
		if (GLFW_PRESS == buttons[16]) {
			if (down == true) {
				UserInput::inputBuffer.push("DOWN");
			}
			down = false;
		};
		if (GLFW_RELEASE == buttons[16]) {
			down = true;
		};
		if (GLFW_PRESS == buttons[17]) {
			UserInput::inputBuffer.push("LEFT");
		};

		//Generic Controller Remapping
		if (Generic == true) {
			float lt = gameState->rightStickY;
			float rt = gameState->leftTrigger;
			float ry = gameState->rightTrigger;
			gameState->rightStickY = ry;
			gameState->leftTrigger = lt;
			gameState->rightTrigger = rt;
		}
	}
	else {
		gameState->controller = false;
	}
}