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


SceneMainMenu::~SceneMainMenu() {

}

void SceneMainMenu::displayScene() {
	//renderer->RenderScene(objects);
}

void SceneMainMenu::displayMenuScene() {
	renderer->RenderMenuScene(objects);
}

void SceneMainMenu::displayTexture(MyTexture &texture, std::vector<glm::vec3> pos) {

	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint shader = renderer->GetShaderProgram("menuShader");
	renderer->SwitchShaderProgram(shader);
	
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
	
	glDisable(GL_BLEND);
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