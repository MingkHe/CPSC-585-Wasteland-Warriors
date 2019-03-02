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
#include <chrono>

#include "Program.h"
#include "RenderingEngine.h"
#include "Scene.h"
#include "SceneMainMenu.h"
#include "UserInput.h"
#include "Logic.h"
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

//https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c

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

	auto currentTime = std::chrono::system_clock::now();
	gameState->time = currentTime;
	std::chrono::duration<double> elapsed_seconds = currentTime - currentTime;

	gameState->timeStep = 1.0 / 60.0; //60 fps
	gameState->button = "";
	gameState->UIMode = "Start";
	//gameState->UIMode = "Game";

	SDL_Init(SDL_INIT_AUDIO);
	
	UserInput usrInput = UserInput();
	Logic logic = Logic();
	AI_Interaction aiInteraction = AI_Interaction();
	Physics_Controller physicsCL = Physics_Controller(gameState);
	Audio_Controller audioCL = Audio_Controller();
	
	renderingEngine = new RenderingEngine(gameState);

	const char* vertexFile = "../shaders/vertex.glsl";
	const char* fragmentFile = "../shaders/fragment.glsl";

	const char* vertexMenuFile = "../shaders/vertexMainMenu.glsl";
	const char* fragmentMenuFile = "../shaders/fragmentMainMenu.glsl";

	renderingEngine->LoadShaderProgram("gamePlayShader", vertexFile, fragmentFile);

	renderingEngine->LoadShaderProgram("menuShader", vertexMenuFile, fragmentMenuFile);
	
	//scene = new Scene(renderingEngine);
	gameState->scene = scene; // what is the scene meaning here in the gamestate?

	UI_Controller UICL = UI_Controller(gameState,renderingEngine);

	//RenderingEngine* renderingEngine2 = new RenderingEngine(gameState, vertexMainFile, fragmentMainFile);
	//SceneMainMenu* mainScene2 = new SceneMainMenu(renderingEngine_MainMenu);
	//SceneMainMenu* mainScene3 = new SceneMainMenu(renderingEngine_MainMenu);
	//SceneMainMenu* mainScene4 = new SceneMainMenu(renderingEngine_MainMenu);
	//Create Entities Example

	scene = new Scene(renderingEngine, gameState);


	gameState->SpawnPlayer(0, 0);

	gameState->SpawnEnemy(15, 0);
	gameState->SpawnEnemy(-15, 0);
	gameState->SpawnEnemy(25, 0);


	
	//gameState->SpawnEnemy(25, 0);




	//Test creation
	//physicsCL.createVehicle();
	//physicsCL.setPosition(1, { 0.0f, 4.0f,  5.0f });

	//Main render loop
	while (!glfwWindowShouldClose(window)) {

		//User Input
		usrInput.Update(gameState);

		//Game Rules
		if (gameState->UIMode == "Game") {
			logic.Update(gameState);
		}

		//AI Interaction System
		if (gameState->UIMode == "Game") { 
			aiInteraction.Update(gameState);
		}

		//Physics Engine
		if (gameState->UIMode == "Game") {
			physicsCL.Update();
		}

		//Audio Engine
		audioCL.playSound(gameState);
		
		//UI System
		UICL.Update(gameState, window);

		//Render Engine
		if (gameState->UIMode == "Game") {
			//scene = new Scene(renderingEngine);
			scene->displayScene();
			//glfwSwapBuffers(window);
			//delete scene;
		}

		glfwSwapBuffers(window);

		//glfwWaitEvents();
		glfwPollEvents();

		//Fixed Timestep

		while (elapsed_seconds.count() < gameState->timeStep){
			currentTime = std::chrono::system_clock::now();
			elapsed_seconds = currentTime - gameState->time;

			//std::cout << "Time elapsed: " << elapsed_seconds.count() << std::endl; //Test statement, delete it if you want
			if (elapsed_seconds.count() >= (gameState->timeStep * 2)) {
				std::cout << "Frame lost" << std::endl; //Test statement, delete it if you want
			}
		}
		elapsed_seconds = currentTime-currentTime;
		gameState->time = currentTime;
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