#include "UI_Controller.h"


UI_Controller::UI_Controller(Gamestate* gameState, RenderingEngine* render)
{
	const char* vertexMainFile = "../shaders/vertexMainMenu.glsl";
	const char* fragmentMainFile = "../shaders/fragmentMainMenu.glsl";
	//renderingEngine = new RenderingEngine(gameState, vertexMainFile, fragmentMainFile);
	renderingEngine = render;

	MyTexture texture;
	InitializeTexture(&texture, "Image/bg.jpg", GL_TEXTURE_RECTANGLE);
	textureArray.push_back(texture);

	InitializeTexture(&texture, "Image/start.png", GL_TEXTURE_RECTANGLE);
	textureArray.push_back(texture);

	InitializeTexture(&texture, "Image/quit.png", GL_TEXTURE_RECTANGLE);
	textureArray.push_back(texture);

	InitializeTexture(&texture, "Image/pointer.png", GL_TEXTURE_RECTANGLE);
	textureArray.push_back(texture);

	mainScene_bg = new SceneMainMenu(renderingEngine);
	mainScene_start = new SceneMainMenu(renderingEngine);
	mainScene_quit = new SceneMainMenu(renderingEngine);
	
}

UI_Controller::~UI_Controller()
{
}

void UI_Controller::Update(Gamestate* GameState, GLFWwindow* window)
{
	//In Game UI
	if (GameState->UIMode == "Game") {

		//render UI elements over already rendered scene.
		//Based on info from GameState.

	//Pause Game Menu
	} else if (GameState->UIMode == "Pause") {

		//render pause game image. 
		//Should update based on selected menu item.
	
	//Start Menu
	} else if (GameState->UIMode == "Start") {
		GameState->ui_menu = true;
		//render start screen image
		//Should update based on selected menu item.


		std::string input = GameState->button;

	
		mainScene_pointer = new SceneMainMenu(renderingEngine);


		//Background Image
		position.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
		position.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
		position.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		position.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));

		mainScene_bg->displayTexture(textureArray[0], position);
		position.clear();

		//Start Image
		position.push_back(glm::vec3(-.4f, -.08f, 1.0f));
		position.push_back(glm::vec3(.4f, -.08f, 1.0f));
		position.push_back(glm::vec3(.4f, .08f, 1.0f));
		position.push_back(glm::vec3(-.4f, .08f, 1.0f));

		mainScene_start->displayTexture(textureArray[1], position);
		position.clear();

		//Quit Image
		position.push_back(glm::vec3(-.3f, -.32f, 1.0f));
		position.push_back(glm::vec3(.3f, -.32f, 1.0f));
		position.push_back(glm::vec3(.3f, -.16f, 1.0f));
		position.push_back(glm::vec3(-.3f, -.16f, 1.0f));

		mainScene_quit->displayTexture(textureArray[2], position);
		position.clear();

		//Pointer Image
		//

		if (input == "UP" || input == "DOWN") {
			pointerState = (pointerState + 1) % 2;
			GameState->ui_switch = true;
		}



		if (pointerState == 0) {
			position.push_back(glm::vec3(-.63f, -.08f, 1.0f));
			position.push_back(glm::vec3(-.5f, -.08f, 1.0f));
			position.push_back(glm::vec3(-.5f, .08f, 1.0f));
			position.push_back(glm::vec3(-.63f, .08f, 1.0f));
		}
		else {
			position.push_back(glm::vec3(-.63f, -.32f, 1.0f));
			position.push_back(glm::vec3(-.5f, -.32f, 1.0f));
			position.push_back(glm::vec3(-.5f, -.16f, 1.0f));
			position.push_back(glm::vec3(-.63f, -.16f, 1.0f));
		}
		mainScene_pointer->displayTexture(textureArray[3], position);
		position.clear();

		//delete mainScene_bg;
		//delete mainScene_start;
		//delete mainScene_quit;
		delete mainScene_pointer;

		if (pointerState == 0 && input == "ENTER") {
			GameState->UIMode = "Game";
			GameState->ui_enter = true;
			GameState->ui_menu = false;
			printf("switch success!\n");
		}

		//glfwSwapBuffers(window);
	} else if (GameState->UIMode == "Win") {
		
		//Display a win screen

		if (GameState->button == "ENTER") {
			GameState->UIMode = "Start";
		}
	} else if (GameState->UIMode == "Lose") {

		//Display a lose screen 

		if (GameState->button == "ENTER") {
			GameState->UIMode = "Start";
		}
	}
}