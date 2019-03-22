#include "Gamestate.h"

#include <iostream>
#include <string>
#include <queue>

#include <GLFW/glfw3.h>

class UserInput
{
private:
	bool up;
	bool down;
	bool view;

public:
	UserInput();
	~UserInput();

	//Input Buffer
	static std::queue<std::string> inputBuffer;
	
	//Mouse
	static double MouseXpos;
	static double MouseYpos;
	float oldMouseXpos;
	float oldMouseYpos;
	static bool MouseLeft;
	static bool MouseRight;

	//WASD
	static bool WKey;
	static bool AKey;
	static bool SKey;
	static bool DKey;
	static bool SPACEKey;

	void Update(Gamestate* gameState);
	static void key(GLFWwindow * window, int key, int scancode, int action, int mods);
	static void cursor(GLFWwindow* window, double xpos, double ypos);
	static void mouseButton(GLFWwindow* window, int button, int action, int mods);
	void gamepad(int joystick, Gamestate* gameState);
};