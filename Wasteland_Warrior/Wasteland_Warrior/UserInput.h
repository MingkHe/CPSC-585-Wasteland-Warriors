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
	bool select;

	int axesCount;
	int axesCountTest;
	int buttonCount;

	bool reverseCameraOn;
	bool vehicleReset;

public:
	UserInput();
	~UserInput();

	//Input Buffer
	static std::queue<std::string> inputBuffer;
	
	//Mouse
	static float MouseXpos;
	static float MouseYpos;
	float oldMouseXpos;
	float oldMouseYpos;
	static bool MouseLeft;
	static bool MouseRight;


	//Car controls
	static bool WKey;
	static bool AKey;
	static bool SKey;
	static bool DKey;

	//Handbrake
	static bool Handbrake;

	//Reverse Cam
	static bool Reverse;

	void Update(Gamestate* gameState);
	static void key(GLFWwindow * window, int key, int scancode, int action, int mods);
	static void cursor(GLFWwindow* window, double xpos, double ypos);
	static void mouseButton(GLFWwindow* window, int button, int action, int mods);
	void gamepad(int joystick, Gamestate* gameState);
};