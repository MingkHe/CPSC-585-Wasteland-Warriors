#include "Gamestate.h"
#include "RenderingEngine.h"
#include "SceneMainMenu.h"

class UI_Controller
{
public:
	UI_Controller(Gamestate* gameState, RenderingEngine* render);
	~UI_Controller();

	RenderingEngine* renderingEngine;

	SceneMainMenu* mainScene_bg;
	SceneMainMenu* mainScene_start;
	SceneMainMenu* mainScene_quit;
	SceneMainMenu* mainScene_pointer;

	SceneMainMenu* pauseScene_bg;
	SceneMainMenu* pauseScene_restart;
	SceneMainMenu* pauseScene_resume;
	SceneMainMenu* pauseScene_menu;

	std::vector<MyTexture> textureArray;
	std::vector<glm::vec3> position;

	glm::vec3 pointerUp;
	glm::vec3 pointerDown;

	int pointerState=0; // 0 means the pointer points to the start and 1 means the pointer points to the quit

	int pausePointerState = 0;



	void Update(Gamestate* GameState, GLFWwindow* window);
};

