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
	

	RenderingEngine * renderer;

	//list of objects in the scene
	std::vector<Geometry> objects;
	int generateRectPrism(float length, float width, float height);

	int sceneObjectIndex = -1;

private:
};


#endif /* SCENE_H_ */
