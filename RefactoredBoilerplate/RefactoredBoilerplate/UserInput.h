#include "Gamestate.h"

#include <iostream>
#include <GLFW/glfw3.h>

class UserInput
{
private:

public:
	UserInput();
	~UserInput();

	int Update(Gamestate gameState);
	static void key(GLFWwindow * window, int key, int scancode, int action, int mods);
};