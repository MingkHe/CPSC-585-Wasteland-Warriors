/*
* Scene.cpp
*
*  Created on: Sep 10, 2018
*  Author: John Hall
*/

#include "SceneMainMenu.h"

#include "RenderingEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include "glad/glad.h"
#include <GLFW/glfw3.h>



SceneMainMenu::SceneMainMenu(RenderingEngine* renderer) : renderer(renderer) {
}


void SceneMainMenu::generateRectPrism(float length, float width, float height) {
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

	for (int i = 0; i < box.verts.size(); i++) {
		box.colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	}
	box.transform = glm::mat4(
		4.f, 0.f, 0.f, 0.f,
		0.f, 4.f, 0.f, 0.f,
		0.f, 0.f, 4.f, 0.f,
		1.f, 0.f, 0.f, 1.f
	);
	box.drawMode = GL_TRIANGLE_STRIP;
	RenderingEngine::assignBuffers(box);
	RenderingEngine::setBufferData(box);
	objects.push_back(box);
}


SceneMainMenu::~SceneMainMenu() {

}

void SceneMainMenu::displayScene() {
	renderer->RenderScene(objects);
}

void SceneMainMenu::displayMenuScene() {
	renderer->RenderMenuScene(objects);
}

void SceneMainMenu::displayTexture(MyTexture &texture, std::vector<glm::vec3> pos) {

	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(renderer->shaderProgram);
	//Set which texture unit the texture is bound to
	glActiveTexture(GL_TEXTURE0);
	//Bind the texture to GL_TEXTURE0
	glBindTexture(GL_TEXTURE_RECTANGLE, texture.textureID);
	//Get identifier for uniform
	GLuint uniformLocation = glGetUniformLocation(renderer->shaderProgram, "ImageTexture");
	//Load texture unit number into uniform
	glUniform1i(uniformLocation, 0);

	rectangle.verts.push_back(pos[0]);
	rectangle.verts.push_back(pos[1]);
	rectangle.verts.push_back(pos[2]);
	rectangle.verts.push_back(pos[0]);
	rectangle.verts.push_back(pos[2]);
	rectangle.verts.push_back(pos[3]);

	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), 0.f));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(0.0f, float(texture.height)));

	rectangle.drawMode = GL_TRIANGLES;

	//Construct vao and vbos for the triangle
	RenderingEngine::assignBuffers(rectangle);

	//Send the triangle data to the GPU
	//Must be done every time the triangle is modified in any way, ex. verts, colors, normals, uvs, etc.
	RenderingEngine::setBufferData(rectangle);

	//Add the triangle to the scene objects
	objects.push_back(rectangle);
	//printf("size: %d\n", objects.size());
	displayMenuScene();

	//free memory, more stuff to care about?
	RenderingEngine::deleteBufferData(rectangle);
	objects.clear();

	//printf("size: %d\n",objects.size());
}

void SceneMainMenu::displayTextureClear(MyTexture &texture, std::vector<glm::vec3> pos) {
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(renderer->shaderProgram);
	//Set which texture unit the texture is bound to
	glActiveTexture(GL_TEXTURE0);
	//Bind the texture to GL_TEXTURE0
	glBindTexture(GL_TEXTURE_RECTANGLE, texture.textureID);
	//Get identifier for uniform
	GLuint uniformLocation = glGetUniformLocation(renderer->shaderProgram, "ImageTexture");
	//Load texture unit number into uniform
	glUniform1i(uniformLocation, 0);

	rectangle.verts.push_back(pos[0]);
	rectangle.verts.push_back(pos[1]);
	rectangle.verts.push_back(pos[2]);
	rectangle.verts.push_back(pos[0]);
	rectangle.verts.push_back(pos[2]);
	rectangle.verts.push_back(pos[3]);

	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), 0.f));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(0.0f, float(texture.height)));

	rectangle.drawMode = GL_TRIANGLES;

	//Construct vao and vbos for the triangle
	RenderingEngine::assignBuffers(rectangle);

	//Send the triangle data to the GPU
	//Must be done every time the triangle is modified in any way, ex. verts, colors, normals, uvs, etc.
	RenderingEngine::setBufferData(rectangle);

	//Add the triangle to the scene objects
	objects.push_back(rectangle);
	//printf("size: %d\n", objects.size());
	renderer->RenderMenuSceneClear(objects);

	//free memory, more stuff to care about?
	RenderingEngine::deleteBufferData(rectangle);
	objects.clear();
}