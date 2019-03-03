/*
* Scene.h
*	Class for storing objects in a scene
*  Created on: Sep 10, 2018
*      Author: cb-ha
*/

#ifndef SCENEMAINMENU_H_
#define SCENEMAINMENU_H_

#include <vector>

#include "Geometry.h"
#include "texture.h"

//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
class RenderingEngine;

class SceneMainMenu {
public:
	SceneMainMenu(RenderingEngine* renderer);
	virtual ~SceneMainMenu();

	//Send geometry to the renderer
	void displayScene();
	void displayMenuScene();
	void displayTexture(MyTexture &texture, std::vector<glm::vec3> pos);
	void displayTextureClear(MyTexture &texture, std::vector<glm::vec3> pos);

	RenderingEngine * renderer;

	//list of objects in the scene
	std::vector<Geometry> objects;
	Geometry rectangle;

private:
	void generateRectPrism(float length, float width, float height);
};


#endif /* SCENEMAINMENU_H_ */
