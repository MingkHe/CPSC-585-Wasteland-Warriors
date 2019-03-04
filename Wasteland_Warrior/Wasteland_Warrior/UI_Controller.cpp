#include "UI_Controller.h"


UI_Controller::UI_Controller(Gamestate* gameState, RenderingEngine* render)
{
	const char* vertexMainFile = "../shaders/vertexMainMenu.glsl";
	const char* fragmentMainFile = "../shaders/fragmentMainMenu.glsl";
	//renderingEngine = new RenderingEngine(gameState, vertexMainFile, fragmentMainFile);
	renderingEngine = render;

	MyTexture texture;
	//0
	InitializeTexture(&texture, "Image/bg.jpg", GL_TEXTURE_RECTANGLE);
	textureArray.push_back(texture);

	//1
	InitializeTexture(&texture, "Image/start4.png", GL_TEXTURE_RECTANGLE);
	textureArray.push_back(texture);

	//2
	InitializeTexture(&texture, "Image/quit1.png", GL_TEXTURE_RECTANGLE);
	textureArray.push_back(texture);

	//3
	InitializeTexture(&texture, "Image/pointer1.png", GL_TEXTURE_RECTANGLE);
	textureArray.push_back(texture);
	
	//4
	InitializeTexture(&texture, "Image/restart.png", GL_TEXTURE_RECTANGLE);
	textureArray.push_back(texture);

	//5
	InitializeTexture(&texture, "Image/resume.png", GL_TEXTURE_RECTANGLE);
	textureArray.push_back(texture);

	//6
	InitializeTexture(&texture, "Image/menu.png", GL_TEXTURE_RECTANGLE);
	textureArray.push_back(texture);

	//7
	InitializeTexture(&texture, "Image/bg_pause.jpg", GL_TEXTURE_RECTANGLE);
	textureArray.push_back(texture);

	mainScene_bg = new SceneMainMenu(renderingEngine);
	mainScene_start = new SceneMainMenu(renderingEngine);
	mainScene_quit = new SceneMainMenu(renderingEngine);

	pauseScene_bg = new SceneMainMenu(renderingEngine);
	pauseScene_restart = new SceneMainMenu(renderingEngine);
	pauseScene_resume = new SceneMainMenu(renderingEngine);
	pauseScene_menu = new SceneMainMenu(renderingEngine);
}

UI_Controller::~UI_Controller()
{
}

void UI_Controller::Update(Gamestate* GameState, GLFWwindow* window)
{
	//In Game UI
	if (GameState->UIMode == "Game") 
	{
		std::string input = GameState->button;

		if (input == "M")
		{
			GameState->UIMode = "Pause";
		}
		//render UI elements over already rendered scene.
		//Based on info from GameState.

	//Pause Game Menu
	} 
	else if (GameState->UIMode == "Pause") 
	{

		//render pause game image. 
		//Should update based on selected menu item.
		
		//todo: pause menu sound effect
		GameState->ui_pauseMenu = true;

		std::string input = GameState->button;

		mainScene_pointer = new SceneMainMenu(renderingEngine);

		//Background Image
		position.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
		position.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
		position.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		position.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));

		pauseScene_bg->displayTexture(textureArray[7], position);
		position.clear();

		//Resume Image
		position.push_back(glm::vec3(-.4f, .2f, 1.0f));
		position.push_back(glm::vec3(.4f, .2f, 1.0f));
		position.push_back(glm::vec3(.4f, .4f, 1.0f));
		position.push_back(glm::vec3(-.4f, .4f, 1.0f));

		pauseScene_resume->displayTexture(textureArray[5], position);
		position.clear();

		//Restart Image
		position.push_back(glm::vec3(-.4f, -.1f, 1.0f));
		position.push_back(glm::vec3(.4f, -.1f, 1.0f));
		position.push_back(glm::vec3(.4f, .1f, 1.0f));
		position.push_back(glm::vec3(-.4f, .1f, 1.0f));

		pauseScene_restart->displayTexture(textureArray[4], position);
		position.clear();

		//Menu Image
		position.push_back(glm::vec3(-.3f, -.4f, 1.0f));
		position.push_back(glm::vec3(.3f, -.4f, 1.0f));
		position.push_back(glm::vec3(.3f, -.2f, 1.0f));
		position.push_back(glm::vec3(-.3f, -.2f, 1.0f));

		pauseScene_menu->displayTexture(textureArray[6], position);
		position.clear();

		//Pointer Image
		if (input == "UP") {
			pausePointerState = (pausePointerState - 1) % 3;
			GameState->ui_switch = true;
		}

		if (input == "DOWN") {
			pausePointerState = (pausePointerState + 1) % 3;
			GameState->ui_switch = true;
		}

		if (pausePointerState == 0) {
			position.push_back(glm::vec3(-.63f, .2f, 1.0f));
			position.push_back(glm::vec3(-.5f, .2f, 1.0f));
			position.push_back(glm::vec3(-.5f, .4f, 1.0f));
			position.push_back(glm::vec3(-.63f, .4f, 1.0f));
		}
		else if(pausePointerState ==1)
		{
			position.push_back(glm::vec3(-.63f, -.1f, 1.0f));
			position.push_back(glm::vec3(-.5f, -.1f, 1.0f));
			position.push_back(glm::vec3(-.5f, .1f, 1.0f));
			position.push_back(glm::vec3(-.63f, .1f, 1.0f));
		}
		else
		{
			position.push_back(glm::vec3(-.63f, -.4f, 1.0f));
			position.push_back(glm::vec3(-.5f, -.4f, 1.0f));
			position.push_back(glm::vec3(-.5f, -.2f, 1.0f));
			position.push_back(glm::vec3(-.63f, -.2f, 1.0f));
		}
		mainScene_pointer->displayTexture(textureArray[3], position);
		position.clear();

		//delete mainScene_bg;
		//delete mainScene_start;
		//delete mainScene_quit;
		delete mainScene_pointer;

		if (pausePointerState == 0 && input == "ENTER") {
			GameState->UIMode = "Game";
			GameState->ui_enter = true;
			GameState->ui_pauseMenu = false;
		}
		else if (pausePointerState == 1 && input == "ENTER") {
			//GameState->UIMode = "Game";

			//todo unknow sound effect bug when the next line call
			GameState->ui_enter = true;

			//GameState->ui_menu = false;
		}
		else if (pausePointerState == 2 && input == "ENTER"){
			GameState->UIMode = "Start";
			GameState->ui_enter = true;
			GameState->ui_pauseMenu = false;
		}
		//glfwSwapBuffers(window);
	}

	//Start Menu
	else if (GameState->UIMode == "Start") {
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
		position.push_back(glm::vec3(-.4f, -.1f, 1.0f));
		position.push_back(glm::vec3(.4f, -.1f, 1.0f));
		position.push_back(glm::vec3(.4f, .1f, 1.0f));
		position.push_back(glm::vec3(-.4f, .1f, 1.0f));

		mainScene_start->displayTexture(textureArray[1], position);
		position.clear();

		//Quit Image
		position.push_back(glm::vec3(-.3f, -.4f, 1.0f));
		position.push_back(glm::vec3(.3f, -.4f, 1.0f));
		position.push_back(glm::vec3(.3f, -.2f, 1.0f));
		position.push_back(glm::vec3(-.3f, -.2f, 1.0f));

		mainScene_quit->displayTexture(textureArray[2], position);
		position.clear();

		//Pointer Image
		//

		if (input == "UP" || input == "DOWN") {
			pointerState = (pointerState + 1) % 2;
			GameState->ui_switch = true;
		}



		if (pointerState == 0) {
			position.push_back(glm::vec3(-.63f, -.1f, 1.0f));
			position.push_back(glm::vec3(-.5f, -.1f, 1.0f));
			position.push_back(glm::vec3(-.5f, .1f, 1.0f));
			position.push_back(glm::vec3(-.63f, .1f, 1.0f));
		}
		else {
			position.push_back(glm::vec3(-.63f, -.4f, 1.0f));
			position.push_back(glm::vec3(-.5f, -.4f, 1.0f));
			position.push_back(glm::vec3(-.5f, -.2f, 1.0f));
			position.push_back(glm::vec3(-.63f, -.2f, 1.0f));
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
		} else if (input == "ENTER") {
			glfwSetWindowShouldClose(window, GL_TRUE);
			printf("quit\n");
		}

		//glfwSwapBuffers(window);
	} 

	//Win scene
	else if (GameState->UIMode == "Win")
	{

	//Display a win screen

	if (GameState->button == "ENTER") {
		GameState->UIMode = "Start";
	}
	}

	//Lose scene
	else if (GameState->UIMode == "Lose"){
	}
}