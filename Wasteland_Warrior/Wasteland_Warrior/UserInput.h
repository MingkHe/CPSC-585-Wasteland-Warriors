#include "Gamestate.h"

#include <iostream>
#include <string>
#include <queue>

#include <GLFW/glfw3.h>

class UserInput
{
public:
	//Joysticks
	float leftStickX;
	float leftStickY;
	float rightStickX;
	float rightStickY;

	//Triggers
	float leftTrigger;
	float rightTrigger;
		
	UserInput();
	~UserInput();

	static std::queue<std::string> inputBuffer;

	void Update(Gamestate gameState);
	static void key(GLFWwindow * window, int key, int scancode, int action, int mods);
	void gamepad(int joystick);
};