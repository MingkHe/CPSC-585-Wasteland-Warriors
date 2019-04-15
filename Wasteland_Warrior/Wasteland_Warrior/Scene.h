/*
* Scene.h
*	Class for storing objects in a scene
*  Created on: Sep 10, 2018
*      Author: cb-ha
*/

#ifndef SCENE_H_
#define SCENE_H_
#pragma once

#include <vector>

#include "Geometry.h"
#include "texture.h"
#include "CompositeWorldObject.h"


class Gamestate;

//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
class RenderingEngine;

class Scene {
public:
	Scene(RenderingEngine* renderer, Gamestate* newGamestate);
	virtual ~Scene();
	void setGamestate(Gamestate* newGamestate);
	Gamestate* gameState;


	//Send geometry to the renderer
	void displayScene();
	int loadOBJObjectInstance(std::vector<const char*> filepath, std::vector<const char*> textureFilepath);

	RenderingEngine * renderer;

	//list of objects in the scene
	std::vector<CompositeWorldObject> allWorldCompObjects;
	std::vector<CompositeWorldObject> compObjectInstances;

	std::vector<CompositeWorldObject> worldGhostObjects;
	int generateRectPrism(float length, float width, float height);

	int sceneObjectIndex = -1;
	int sceneCompObjectIndex = -1;
	int objectInstanceIndex = -1;
	char previousHeader = 'v';
	int playerWheelRender;
	int playerWheelIndices[4];

	int loadCompObjectInstance(int compObjIndex);

	glm::mat4 identityMatrix = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
private:

	/*CompositeWorldObject createObjectInstance(const char* textureFilepath, CompositeWorldObject OBJobjectComp,
		std::vector< unsigned int > vertexIndices, std::vector< unsigned int > uvIndices, std::vector< unsigned int > normalIndices,
		std::vector< glm::vec3 > temp_vertices, std::vector< glm::vec2 > temp_uvs, std::vector< glm::vec3 > temp_normals, int textureIndex);*/

	Geometry createObjectInstance(const char* textureFilepath, 
		std::vector< unsigned int > vertexIndices, std::vector< unsigned int > uvIndices, std::vector< unsigned int > normalIndices,
		std::vector< glm::vec3 > temp_vertices, std::vector< glm::vec2 > temp_uvs, std::vector< glm::vec3 > temp_normals, int materialIndex);
};


#endif /* SCENE_H_ */
