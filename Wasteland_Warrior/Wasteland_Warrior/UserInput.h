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
	
	static double MouseXpos;
	static double MouseYpos;
	float oldMouseXpos;
	float oldMouseYpos;

	//WASD
	static bool WKey;
	static bool AKey;
	static bool SKey;
	static bool DKey;
	static bool RKey;
	static bool SPACEKey;

	static bool restart;

	void Update(Gamestate* gameState);
	static void key(GLFWwindow * window, int key, int scancode, int action, int mods);
	static void cursor(GLFWwindow* window, double xpos, double ypos);
	void gamepad(int joystick, Gamestate* gameState);
};