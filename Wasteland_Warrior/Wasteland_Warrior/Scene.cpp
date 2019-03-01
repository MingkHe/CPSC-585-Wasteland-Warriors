/*
* Scene.cpp
*
*  Created on: Sep 10, 2018
*  Author: John Hall
*/

#include "Scene.h"
#include "Gamestate.h"

#include "RenderingEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include "glad/glad.h"
#include <GLFW/glfw3.h>

Scene::Scene(RenderingEngine* renderer, Gamestate* newGamestate) : renderer(renderer) {
	gameState = newGamestate;
	gameState->scene = this;

	Geometry ground;
	ground.verts.push_back(glm::vec3(-250.f, 0.0f, -250.f));
	ground.verts.push_back(glm::vec3(250.f, 0.0f, -250.f));
	ground.verts.push_back(glm::vec3(-250.f, 0.0f, 250.f));
	ground.verts.push_back(glm::vec3(250.f, 0.0f, 250.f));
	for (int i = 0; i < 4; i++) {
		ground.colors.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
		ground.normals.push_back(glm::vec3(0.f, 1.f, 0.f));
	}

	ground.transform = glm::mat4(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
	);

	ground.drawMode = GL_TRIANGLE_STRIP;
	RenderingEngine::assignBuffers(ground);
	RenderingEngine::setBufferData(ground);
	objects.push_back(ground);


	//generateRectPrism(5.0, 3.0, 2.0);
}

void Scene::setGamestate(Gamestate* newGamestate) {
	gameState = newGamestate;
}

int Scene::generateRectPrism(float length, float width, float height) {
	Geometry box;
	float carL = width; // Actually is width
	float carW = length; //Actually is length
	float carH = height;


	//Front Side
	box.verts.push_back({ carL, carH, carW });
	box.verts.push_back({ -carL, carH, carW });
	box.verts.push_back({ carL, -carH, carW });
	box.verts.push_back({ -carL, -carH, carW });
	for (int i = 0; i < 4; i++) {
		box.normals.push_back(glm::vec3(1.f, 0.f, 0.f));
	}

	//Back
	box.verts.push_back({ carL, carH, -carW });
	box.verts.push_back({ -carL, carH, -carW });
	box.verts.push_back({ carL, -carH, -carW });
	box.verts.push_back({ -carL, -carH, -carW });
	for (int i = 0; i < 4; i++) {
		box.normals.push_back(glm::vec3(-1.f, 0.f, 0.f));
	}

	//Left
	box.verts.push_back({ carL, carH, carW });
	box.verts.push_back({ carL, -carH, carW });
	box.verts.push_back({ carL, carH, -carW });
	box.verts.push_back({ carL, -carH, -carW });
	for (int i = 0; i < 4; i++) {
		box.normals.push_back(glm::vec3(0.f, 0.f, -1.f));
	}

	//Right
	box.verts.push_back({ -carL, carH, carW });
	box.verts.push_back({ -carL, -carH, carW });
	box.verts.push_back({ -carL, carH, -carW });
	box.verts.push_back({ -carL, -carH, -carW });

	for (int i = 0; i < 4; i++) {
		box.normals.push_back(glm::vec3(0.f, 0.f, 1.f));
	}

	//Top
	box.verts.push_back({ carL, carH, carW });
	box.verts.push_back({ -carL, carH, carW });
	box.verts.push_back({ carL, carH, -carW });
	box.verts.push_back({ -carL, carH, -carW });
	for (int i = 0; i < 4; i++) {
		box.normals.push_back(glm::vec3(0.f, 1.f, 0.f));
	}

	//Bottom
	box.verts.push_back({ carL, -carH, carW });
	box.verts.push_back({ -carL, -carH, carW });
	box.verts.push_back({ carL, -carH, -carW });
	box.verts.push_back({ -carL, -carH, -carW });

	for (int i = 0; i < 4; i++) {
		box.normals.push_back(glm::vec3(0.f, -1.f, 0.f));
	}

	//Casting to signed int to resolve warning of signed/unsigned comparison
	for (int i = 0; i < (signed int)box.verts.size(); i++) {
		box.colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	}


	box.drawMode = GL_TRIANGLE_STRIP;
	RenderingEngine::assignBuffers(box);
	RenderingEngine::setBufferData(box);
	objects.push_back(box);

	sceneObjectIndex++;
	return(sceneObjectIndex);
}


Scene::~Scene() {

}

void Scene::displayScene() {
<<<<<<< HEAD
	//glUseProgram(renderer->shaderProgramList[0]);
	GLuint shader = renderer->GetShaderProgram("gamePlayShader");
	renderer->SwitchShaderProgram(shader);
=======
	for (int i = 1; i <= sceneObjectIndex; i++) {
		objects[i].transform = gameState->getEntityTransformation(i);
	}
	//objects[1].transform = gameState->playerVehicle.transformationMatrix;

>>>>>>> master
	renderer->RenderScene(objects);
}
