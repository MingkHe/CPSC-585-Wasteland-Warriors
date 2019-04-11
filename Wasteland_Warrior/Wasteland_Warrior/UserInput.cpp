#pragma comment(lib, "LogitechSteeringWheelLib.lib") 

#include "UserInput.h"
#include "Program.h"
#include "LogitechSteeringWheelLib.h"

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

//Driving controls
bool UserInput::WKey;
bool UserInput::AKey;
bool UserInput::SKey;
bool UserInput::DKey;

//Handbrake
bool UserInput::Handbrake;

//Reverse Cam
bool UserInput::Reverse;

UserInput::UserInput()
{
}

UserInput::~UserInput()
{
}

void UserInput::Update(Gamestate* gameState)
{
	//Reset Gamepad
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

	//Car control
	if (UserInput::WKey) {
		gameState->WKey = true;
	}
	else {
		gameState->WKey = false;
	}
	if (UserInput::AKey) {
		gameState->AKey = true;
	}
	else {
		gameState->AKey = false;
	}
	if (UserInput::SKey) {
		gameState->SKey = true;
	}
	else{
		gameState->SKey = false;
	}
	if (UserInput::DKey) {
		gameState->DKey = true;
	}
	else {
		gameState->DKey = false;
	}
	if (UserInput::Handbrake) {
		gameState->Handbrake = true;
	}
	else {
		gameState->Handbrake = false;
	}
	if (UserInput::Reverse) {
		UserInput::inputBuffer.push("REVERSE");
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

			//Selection
		case GLFW_KEY_ENTER:
			UserInput::inputBuffer.push("ENTER");
			break;

			//Handbrake
		case GLFW_KEY_SPACE:
			UserInput::Handbrake = true;
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

			//Handbrake
		case GLFW_KEY_SPACE:
			UserInput::Handbrake = false;
			break;

			//Reverse Cam
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
	const float *axesTest = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCountTest); //This is for checking if its a wireless controller or haptics steering wheel
	if ((controller == 1) && (axesCountTest != 4)) {
		gameState->controller = true;

		gameState->updateHapticWheelState = false;
		std::string name = glfwGetJoystickName(GLFW_JOYSTICK_1);
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

		if (name == "Wireless Controller") { 

			//Joysticks
			gameState->leftStickX = axes[0];
			gameState->leftStickY = axes[1];
			gameState->rightStickX = axes[2];
			gameState->rightStickY = axes[5];

			//Triggers
			gameState->leftTrigger = axes[3];
			gameState->rightTrigger = axes[4];

			//Buttons
			if (GLFW_PRESS == buttons[0]) {
				UserInput::inputBuffer.push("X");
			};
			if (GLFW_PRESS == buttons[1]) {
				if (select == true) {
					UserInput::inputBuffer.push("A");
				}
				select = false;
			};
			if (GLFW_RELEASE == buttons[1]) {
				select = true;
			};
			if (GLFW_PRESS == buttons[2]) {
				UserInput::inputBuffer.push("B");
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
			if (GLFW_PRESS == buttons[7]) {
				//Right trigger as button
			};
			if (GLFW_PRESS == buttons[8]) {
				//Left trigger as button
			};
			if (GLFW_PRESS == buttons[9]) {
				UserInput::inputBuffer.push("MENU");
			};
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
		}
		else {
			//Joysticks
			gameState->leftStickX = axes[0];
			gameState->leftStickY = axes[1];
			gameState->rightStickX = axes[2];
			gameState->rightStickY = axes[3];

			//Triggers
			gameState->leftTrigger = axes[4];
			gameState->rightTrigger = axes[5];

			//Buttons
			if (GLFW_PRESS == buttons[0]) {
				if (select == true) {
					UserInput::inputBuffer.push("A");
				}
				select = false;
			};
			if (GLFW_RELEASE == buttons[0]) {
				select = true;
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
			if (GLFW_PRESS == buttons[7]) {
				UserInput::inputBuffer.push("MENU");
			};
			if (GLFW_PRESS == buttons[8]) {
				UserInput::inputBuffer.push("LS");
			};
			if (GLFW_PRESS == buttons[9]) {
				UserInput::inputBuffer.push("RS");
			};
			if (GLFW_PRESS == buttons[10]) {
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
			if (GLFW_PRESS == buttons[12]) {
				if (down == true) {
					UserInput::inputBuffer.push("DOWN");
				}
				down = false;
			};
			if (GLFW_RELEASE == buttons[12]) {
				down = true;
			};
			if (GLFW_PRESS == buttons[13]) {
				UserInput::inputBuffer.push("LEFT");
			};
		}
	}
	//If haptic steering wheel is connected
	else if ((controller == 1) && (axesCountTest == 4)) {

		gameState->hapticWheel = true;

		//Haptic Wheel & Pedals
		int axesCount; //4
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
	
		//all go -1 to 1 (-1 for left and down, 1 for right and up)
		gameState->leftStickX = axes[0]; //Wheel Steering
		gameState->rightTrigger = -axes[1]; //Acceleration
		gameState->leftTrigger = -axes[2]; // Braking
		gameState->rightStickX = -axes[3];  //Clutch

		//Haptic Wheel buttons
		int buttonCount; //22
		const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		//std::cout << buttonCount << std::endl;
		if (GLFW_PRESS == buttons[0]) {
			UserInput::inputBuffer.push("A");
		};
		if (GLFW_PRESS == buttons[4]) {
			UserInput::inputBuffer.push("B");
		};
		if (GLFW_PRESS == buttons[8]) {
			if (vehicleReset == true) {
				UserInput::inputBuffer.push("Y");
			}
			vehicleReset = false;
		};
		if (GLFW_RELEASE == buttons[8]) {
			vehicleReset = true;
		};
		if (GLFW_PRESS == buttons[3]) {
			UserInput::inputBuffer.push("X");
		};
		if (GLFW_PRESS == buttons[5]) {
			UserInput::inputBuffer.push("REVERSE");
		};
		if (GLFW_PRESS == buttons[9]) {
			if (view == true) {
				UserInput::inputBuffer.push("VIEW");
			}
			view = false;
		};
		if (GLFW_RELEASE == buttons[9]) {
			view = true;
		};
		if (GLFW_PRESS == buttons[6]) {
			UserInput::inputBuffer.push("OPTION");
		};
		if (GLFW_PRESS == buttons[10]) {
			UserInput::inputBuffer.push("MENU");
		};
		if (GLFW_PRESS == buttons[1]) {
			UserInput::inputBuffer.push("LS");
		};
		if (GLFW_PRESS == buttons[9]) {
			UserInput::inputBuffer.push("RS");
		};

		//Haptic Wheel's Dpad
		if (GLFW_PRESS == buttons[18]) {
			if (up == true) {
				UserInput::inputBuffer.push("UP");
			}
			up = false;
		};
		if (GLFW_RELEASE == buttons[18]) {
			up = true;
		};
		if (GLFW_PRESS == buttons[19]) {
			UserInput::inputBuffer.push("RIGHT");
		};
		if (GLFW_PRESS == buttons[20]) {
			if (down == true) {
				UserInput::inputBuffer.push("DOWN");
			}
			down = false;
		};
		if (GLFW_RELEASE == buttons[20]) {
			down = true;
		};
		if (GLFW_PRESS == buttons[21]) {
			UserInput::inputBuffer.push("LEFT");
		};

		//
		if (LogiUpdate()) {
			gameState->updateHapticWheelState = true;
		}

	}
	else {
		gameState->updateHapticWheelState = false;
		gameState->controller = false;
		gameState->hapticWheel = false;
	}
}