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

//Time stuff
#include <sys/timeb.h>
#include <stdio.h>
#include <errno.h>

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
	gameState->window_width = this->win_width;//glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
	gameState->window_height = this->win_height;//glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
	gameState->UIMode = "Loading";

	struct timeb currentTime;
	ftime(&currentTime);
	gameState->time = currentTime.millitm;
	int elapsed_seconds = 0;

	SDL_Init(SDL_INIT_AUDIO);

	UserInput usrInput = UserInput();
	Logic logic = Logic();
	AI_Interaction aiInteraction = AI_Interaction(gameState);
	Physics_Controller physicsCL = Physics_Controller(gameState);
	Audio_Controller audioCL = *Audio_Controller::instance();

	renderingEngine = new RenderingEngine(gameState);

	const char* vertexFile = "../shaders/vertex.glsl";
	const char* fragmentFile = "../shaders/fragment.glsl";

	const char* vertexMenuFile = "../shaders/vertexMainMenu.glsl";
	const char* fragmentMenuFile = "../shaders/fragmentMainMenu.glsl";

	renderingEngine->LoadShaderProgram("gamePlayShader", vertexFile, fragmentFile);

	renderingEngine->LoadShaderProgram("menuShader", vertexMenuFile, fragmentMenuFile);

	

	UI_Controller UICL = UI_Controller(gameState,renderingEngine);

	scene = new Scene(renderingEngine, gameState);

	UICL.Update(gameState,window);
	glfwSwapBuffers(window);

	printf("finished loading\n");
	//gameState->InstantiateAllMeshes_Textures();

	//Mesh loading
	gameState->InstantiateAllMeshes_Textures_Map();
	for (int i = 1; i <= 25; i++) {
		//Sleep(100);
		gameState->loadingPercentage = i;
		UICL.Update(gameState, window);
		glfwSwapBuffers(window);
	}
	gameState->loadingPercentage = 25;

	gameState->InstantiateAllMeshes_Textures_Static();

	for (int i = 26; i <= 50; i++) {
		//Sleep(100);
		gameState->loadingPercentage = i;
		UICL.Update(gameState, window);
		glfwSwapBuffers(window);
	}

	//Sleep(1000);

	gameState->InstantiateAllMeshes_Textures_Dynamic();

	for (int i = 51; i <= 78; i++) {
		//Sleep(100);
		gameState->loadingPercentage = i;
		UICL.Update(gameState, window);
		glfwSwapBuffers(window);
	}

	gameState->InstantiateAllMeshes_Textures_Vehicle();

	//Spawn Map, Skybox and Canyon Walls
	gameState->SpawnMap();
	gameState->SpawnStaticObject(0, 0, 0, 0, 0, 0, 0);
	gameState->SpawnStaticObject(6, 0, 0, 0, 0, 0, 0);
	
	//Spawn Various Static Objects
	gameState->SpawnStaticObject(2, 93, -0.75, -45, 0, 0, 0);

	gameState->SpawnStaticObject(3, 63, 0, -25, 0, 0, 0);
	gameState->SpawnStaticObject(2, 123, -6.7, -95, 0, 0, 0);
	gameState->SpawnStaticObject(3, 148, -6.2, -55, 0, 0, 0);
	gameState->SpawnStaticObject(2, 143, -6.7, -125, 0, 0, 0);
	gameState->SpawnStaticObject(3, 73, 0, -125, 0, 0, 0);

	
	gameState->SpawnStaticObject(7, 0, 0, 173, 0, 0, 0);
	//Tunnels
	gameState->SpawnStaticObject(8, 0, 0, 190, 0, 180, 0);
	gameState->SpawnStaticObject(8, 100, 0, -180, 0, 0, 0);

	//Buildings & Ruined Buildings
	gameState->SpawnStaticObject(1, 20, 0, 20, 0, 0, 0);
	gameState->SpawnStaticObject(1, -40, 0, -30, 0, 0, 0);
	gameState->SpawnStaticObject(1, 60, 0, -13, 0, 0, 0);
	gameState->SpawnStaticObject(1, -80, 0, 53, 0, 0, 0);
	gameState->SpawnStaticObject(1, 100, 0, 93, 0, 0, 0);
	gameState->SpawnStaticObject(1, -120, 0, -43, 0, 0, 0);
	gameState->SpawnStaticObject(1, 140, 0, 73, 0, 0, 0);
	gameState->SpawnStaticObject(1, -100, 0, -53, 0, 0, 0);

	gameState->SpawnStaticObject(1, 20, 0, -123, 0, 0, 0);
	gameState->SpawnStaticObject(1, -30, 0, -145, 0, 0, 0);

	gameState->SpawnStaticObject(9, -20, 0, -100, 0, 0, 0);
	gameState->SpawnStaticObject(4, 0, 0, -100, 0, 0, 0);
	gameState->SpawnStaticObject(9, 40, 0, -100, 0, 0, 0);
	gameState->SpawnStaticObject(10, 20, 0, -100, 0, 0, 0);

	//Spawn Player
	gameState->SpawnPlayer(0, 0, 0, 0, 0, 0);

	for (int i = 79; i <= 100; i++) {
		//Sleep(100);
		gameState->loadingPercentage = i;
		UICL.Update(gameState, window);
		glfwSwapBuffers(window);
	}

	gameState->UIMode = "Start";
	//Sleep(1000);

	/*
	while (true) {
		usrInput.Update(gameState);
		if (gameState->button == "Enter") {
			break;
		}
	}
	*/

	

	//Main render loop
	while (!glfwWindowShouldClose(window)) {
		//std::cout << "New execution loop started on: " << currentTime.time << "." << currentTime.millitm << std::endl;
		//User Input
		usrInput.Update(gameState);

		//Game Rules
		if (gameState->UIMode == "Game") {
			logic.Update(gameState);
		}

		//AI Interaction System
		if (gameState->UIMode == "Game") {
			aiInteraction.Update();
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
		while (elapsed_seconds < gameState->timeStep){
			ftime(&currentTime);
			//std::cout << "Elapsed time: " << elapsed_seconds << std::endl;
			//std::cout << "Last time was: " << gameState->time << "   Current time is: " << currentTime.millitm << std::endl;
			
			if (currentTime.millitm > gameState->time) { elapsed_seconds = currentTime.millitm - gameState->time; }
			else { elapsed_seconds = (1000 - currentTime.millitm) + gameState->time; }
		}
		//std::cout << "Frame"  << std::endl;
		elapsed_seconds = 0;
		gameState->time = currentTime.millitm;
	}
	SDL_CloseAudio();
	SDL_Quit();
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
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	//int width = mode->width;//1280; //640
	//int height = mode->height;//960; //480
	int width = 640;
	int height = 480;
	this->win_height = height;
	this->win_width = width;

	window = glfwCreateWindow(width, height, "Wasteland Warrior", NULL, NULL);
	//window = glfwCreateWindow(width, height, "Wasteland Warrior", glfwGetPrimaryMonitor(), NULL);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
		glfwTerminate();
		return;
	}

	//Input Callbacks
	glfwSetKeyCallback(window, UserInput::key);
	glfwSetCursorPosCallback(window, UserInput::cursor);
	glfwSetMouseButtonCallback(window, UserInput::mouseButton);

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
