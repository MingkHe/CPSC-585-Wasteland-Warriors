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
	int loadOBJObject(const char* filepath, const char* textureFilepath);

	RenderingEngine * renderer;

	//list of objects in the scene
	std::vector<CompositeWorldObject> allWorldCompObjects;
	std::vector<CompositeWorldObject> compObjectInstances;
	int generateRectPrism(float length, float width, float height);

	int sceneObjectIndex = -1;
	int sceneCompObjectIndex = -1;
	char previousHeader = 'v';

private:
	void createObject(const char* textureFilepath, CompositeWorldObject OBJobjectComp, Geometry OBJobject,
		std::vector< unsigned int > vertexIndices, std::vector< unsigned int > uvIndices, std::vector< unsigned int > normalIndices,
		std::vector< glm::vec3 > temp_vertices, std::vector< glm::vec2 > temp_uvs, std::vector< glm::vec3 > temp_normals);
};


#endif /* SCENE_H_ */
