
/*
* RenderingEngine.cpp
*
*  Created on: Sep 10, 2018
*      Author: John Hall
*/
#define PI_F 3.14159265359f
#include "RenderingEngine.h"

#include <iostream>

//included here because it just contains some global functions
#include "ShaderTools.h"

RenderingEngine::RenderingEngine(Gamestate *gameState) {
	game_state = gameState;
}

RenderingEngine::~RenderingEngine() {

}

void RenderingEngine::RenderScene(const std::vector<CompositeWorldObject>& objects) {
	//Clears the screen to a dark grey background

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//sets uniforms

	glm::mat4 perspectiveMatrix = glm::perspective(PI_F*.4f, 512.f / 512.f, .1f, 200.f);
	glm::mat4 modelViewProjection = perspectiveMatrix * game_state->camera.viewMatrix();

	glUseProgram(shaderProgram);
	GLint transformGL = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelViewProjection"), 1, false, glm::value_ptr(modelViewProjection));

	glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPosition"), 1, glm::value_ptr(game_state->camera.pos));
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightPosition"), 1, glm::value_ptr(game_state->light));
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightColour"), 1, glm::value_ptr(game_state->lightColor));
	glUniform1f(glGetUniformLocation(shaderProgram, "lightAttenuation"), game_state->lightAttenuation);
	glUniform1f(glGetUniformLocation(shaderProgram, "lightAmbientCoeff"), game_state->lightAmbientCoefficient);

	glUniform3fv(glGetUniformLocation(shaderProgram, "materialSpecularColor"), 1, glm::value_ptr(game_state->materialSpecularColor));
	glUniform1f(glGetUniformLocation(shaderProgram, "materialShininess"), game_state->materialShininess);
	glUniform1i(glGetUniformLocation(shaderProgram, "materialTex"), 0);
	

	for (const CompositeWorldObject& g : objects) {
		glUniformMatrix4fv(transformGL, 1, false, glm::value_ptr(g.geometry[0].transform));
		//bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g.geometry[0].texture.textureID);

		glBindVertexArray(g.geometry[0].vao);
		glDrawArrays(g.geometry[0].drawMode, 0, g.geometry[0].verts.size());

		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
	}
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

void RenderingEngine::RenderMenuScene(const std::vector<Geometry>& objects) {
	//Clears the screen to a dark grey background
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shaderProgram);

	for (const Geometry& g : objects) {
		glBindVertexArray(g.vao);
		glDrawArrays(g.drawMode, 0, g.verts.size());

		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
	}

	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

void RenderingEngine::RenderMenuSceneClear(const std::vector<Geometry>& objects) {
	//Clears the screen to a dark grey background
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shaderProgram);

	for (const Geometry& g : objects) {
		glBindVertexArray(g.vao);
		glDrawArrays(g.drawMode, 0, g.verts.size());

		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
	}

	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

void RenderingEngine::LoadShaderProgram(std::string name, const char* vertexFile, const char* fragmentFile) {

	if (shaderProgramList[name] == NULL)
	{
		//myMusic[path] = Mix_LoadMUS(path.c_str());
		shaderProgramList[name] = ShaderTools::InitializeShaders(vertexFile, fragmentFile);

		if (shaderProgramList[name] == NULL) {
			printf("Shader loading error: Program %s Error\n", name);
		}

	}

}

GLuint RenderingEngine::GetShaderProgram(std::string name) {
	if (shaderProgramList[name] == NULL)
	{
		printf("Shader haven't loaded yet. Please load shader first. Error Shader name: %s Error\n", name);
	}
	return shaderProgramList[name];
}

void RenderingEngine::SwitchShaderProgram(GLuint shader) {
	shaderProgram = shader;
}

void RenderingEngine::assignBuffers(Geometry& geometry) {
	//Generate vao for the object
	//Constant 1 means 1 vao is being generated
	glGenVertexArrays(1, &geometry.vao);
	glBindVertexArray(geometry.vao);

	//Generate vbos for the object
	//Constant 1 means 1 vbo is being generated
	glGenBuffers(1, &geometry.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &geometry.colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &geometry.normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &geometry.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(3);



}

void RenderingEngine::setBufferData(Geometry& geometry) {
	//Send geometry to the GPU
	//Must be called whenever anything is updated about the object
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.verts.size(), geometry.verts.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.colors.size(), geometry.colors.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * geometry.uvs.size(), geometry.uvs.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.normals.size(), geometry.normals.data(), GL_STATIC_DRAW);

}

void RenderingEngine::deleteBufferData(Geometry& geometry) {
	glDeleteBuffers(1, &geometry.vertexBuffer);
	glDeleteBuffers(1, &geometry.normalBuffer);
	glDeleteBuffers(1, &geometry.colorBuffer);
	glDeleteBuffers(1, &geometry.uvBuffer);
	glDeleteVertexArrays(1, &geometry.vao);
}

bool RenderingEngine::CheckGLErrors() {
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError()) {
		std::cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM" << std::endl; break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE" << std::endl; break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION" << std::endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY" << std::endl; break;
		default:
			std::cout << "[unknown error code]" << std::endl;
		}
		error = true;
	}
	return error;

}