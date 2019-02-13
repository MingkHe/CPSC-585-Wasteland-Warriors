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
	
	static float MouseXpos;
	static float MouseYpos;
	float oldMouseXpos;
	float oldMouseYpos;

	//WASD
	static bool WKey;
	static bool AKey;
	static bool SKey;
	static bool DKey;
	static bool SPACEKey;

	float cameraSensitivity = 0.01;

	void Update(Gamestate* gameState);
	static void key(GLFWwindow * window, int key, int scancode, int action, int mods);
	static void cursor(GLFWwindow* window, double xpos, double ypos);
	void gamepad(int joystick, Gamestate* gameState);
};