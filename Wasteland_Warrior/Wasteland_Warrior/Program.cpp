/*
* Program.cpp
*
*  Created on: Sep 10, 2018
*      Author: John Hall
*/

#include <iostream>
#include <string>

//**Must include glad and GLFW in this order or it breaks**
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Program.h"
#include "RenderingEngine.h"
#include "Scene.h"
#include "SceneMainMenu.h"
#include "UserInput.h"
#include "AI_Interaction.h"
#include "Physics_Controller.h"
#include "Audio_Controller.h"
#include "UI_Controller.h"
#include "Gamestate.h"
#include "Entity.h"
#include "Vehicle.h"
#include "PlayerUnit.h"
#include "EnemyUnit.h"
#include "texture.h"

#include <SDL_mixer.h>
#include <SDL.h>

/*****--- Old camera code. Delete this and handle camera in rendering. ---*****/
float oldMouseXpos;
float oldMouseYpos;
/**********/

Program::Program() {
	setupWindow();
}

Program::~Program() {
	//Must be cleaned up in the destructor because these are allocated to the heap
	delete renderingEngine;
	delete scene;
}

void Program::start() {
	//Initialization
	Gamestate* gameState = new Gamestate();
	gameState->time = 0.0;
	gameState->timeStep = 1.0 / 60.0; //60 fps
	gameState->button = "";
	gameState->UIMode = "Start";
	//gameState->UIMode = "Game";

	SDL_Init(SDL_INIT_AUDIO);
	
	UserInput usrInput = UserInput();
	AI_Interaction aiInteraction = AI_Interaction();
	Physics_Controller physicsCL = Physics_Controller(gameState);
	Audio_Controller audioCL = Audio_Controller();
	UI_Controller UICL = UI_Controller(gameState);
	
	const char* vertexFile = "../shaders/vertex.glsl";
	const char* fragmentFile = "../shaders/fragment.glsl";

	renderingEngine = new RenderingEngine(gameState, vertexFile, fragmentFile);
	
	scene = new Scene(renderingEngine);
	gameState->scene = scene; // what is the scene meaning here in the gamestate?



	//RenderingEngine* renderingEngine2 = new RenderingEngine(gameState, vertexMainFile, fragmentMainFile);
	//SceneMainMenu* mainScene2 = new SceneMainMenu(renderingEngine_MainMenu);
	//SceneMainMenu* mainScene3 = new SceneMainMenu(renderingEngine_MainMenu);
	//SceneMainMenu* mainScene4 = new SceneMainMenu(renderingEngine_MainMenu);
	//Create Entities Example

	//PlayerUnit mainCar = PlayerUnit();
	//EnemyUnit Enemy1 = EnemyUnit();
	//EnemyUnit Enemy2 = EnemyUnit();

	//gameState->Entities.push_back(mainCar);
	//gameState->Entities.push_back(Enemy1);
	//gameState->Entities.push_back(Enemy2);

	//Main render loop
	while (!glfwWindowShouldClose(window)) {

		//User Input
		usrInput.Update(gameState);

		//AI Interaction System
		if (gameState->UIMode == "Game") { 
			aiInteraction.Update(gameState);
		}

		//Physics Engine
		if (gameState->UIMode == "Game") {
			physicsCL.Update();
			//std::cout << "Box position:  X:" << gameState->cubeLocation.x << "  Y:" << gameState->cubeLocation.y << "  Z:" << gameState->cubeLocation.z << std::endl; //Test statement, delete it if you want
		}

		//Audio Engine
		audioCL.playSound(gameState);
		
		//UI System
		UICL.Update(gameState, window);

		//Render Engine
		if (gameState->UIMode == "Game") {
			printf("gamegamegamegame...\n");
			//scene = new Scene(renderingEngine);
			scene->displayScene();
			//glfwSwapBuffers(window);
			//delete scene;
		}
		
		/*****--- Old camera code. Delete this and handle camera in rendering. ---*****/
		if (UserInput::MouseXpos != oldMouseXpos) {
			gameState->camera.rotateHorizontal((oldMouseXpos - UserInput::MouseXpos) * 0.01);
			oldMouseXpos = UserInput::MouseXpos;
		}
		if (UserInput::MouseYpos != oldMouseYpos) {
			gameState->camera.rotateVertical((oldMouseYpos - UserInput::MouseYpos) * 0.01);
			oldMouseYpos = UserInput::MouseYpos;
		}
		/**********/
		glfwSwapBuffers(window);
		//glfwWaitEvents();
		glfwPollEvents();

		//Fixed Timestep
		gameState->time += gameState->timeStep;

	}

}

void Program::setupWindow() {
	//Initialize the GLFW windowing system
	if (!glfwInit()) {
		std::cout << "ERROR: GLFW failed to initialize, TERMINATING" << std::endl;
		return;
	}

	//Set the custom error callback function
	//Errors will be printed to the console
	glfwSetErrorCallback(ErrorCallback);

	//Attempt to create a window with an OpenGL 4.1 core profile context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 1280;
	int height = 960;
	window = glfwCreateWindow(width, height, "Wasteland Warrior", 0, 0);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
		glfwTerminate();
		return;
	}

	//Input Callbacks
	glfwSetKeyCallback(window, UserInput::key);
	glfwSetCursorPosCallback(window, UserInput::cursor);

	//Bring the new window to the foreground (not strictly necessary but convenient)
	glfwMakeContextCurrent(window);

	//Intialize GLAD (finds appropriate OpenGL configuration for your system)
	if (!gladLoadGL()) {
		std::cout << "GLAD init failed" << std::endl;
		return;
	}

	//Query and print out information about our OpenGL environment
	QueryGLVersion();
}

void Program::QueryGLVersion() {
	// query opengl version and renderer information
	std::string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	std::string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	std::string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	std::cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << std::endl;
}

void ErrorCallback(int error, const char* description) {
	std::cout << "GLFW ERROR " << error << ":" << std::endl;
	std::cout << description << std::endl;
}