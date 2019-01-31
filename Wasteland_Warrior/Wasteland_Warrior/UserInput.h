#include "Gamestate.h"

#include <iostream>
#include <string>
#include <queue>

#include <GLFW/glfw3.h>

class UserInput
{
public:
	UserInput();
	~UserInput();

	static std::queue<std::string> inputBuffer;

	int Update(Gamestate gameState);
	void gamepad(int joystick);
	static void key(GLFWwindow * window, int key, int scancode, int action, int mods);
};