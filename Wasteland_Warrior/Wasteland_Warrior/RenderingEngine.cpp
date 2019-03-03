
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

	healthshaderProgram = ShaderTools::InitializeShaders("../shaders/healthvertex.glsl", "../shaders/healthfragment.glsl");
	radarshaderProgram = ShaderTools::InitializeShaders("../shaders/radarvertex.glsl", "../shaders/radarfragment.glsl");

	health.verts.push_back(glm::vec3(.5f, .8f, 0.f));
	health.verts.push_back(glm::vec3(.5f, .9f, 0.f));
	health.verts.push_back(glm::vec3(.9f, .8f, 0.f));
	health.verts.push_back(glm::vec3(.9f, .9f, 0.f));
	health.uvs.push_back(glm::vec2(0.f, 0.f));
	health.uvs.push_back(glm::vec2(0.f, 1.f));
	health.uvs.push_back(glm::vec2(1.f, 0.f));
	health.uvs.push_back(glm::vec2(1.f, 1.f));
	health.drawMode = GL_TRIANGLE_STRIP;
	assignBuffers(health);
	setBufferData(health);

	radar.verts.push_back(glm::vec3(-.6f, -.6f, 0.f));
	radar.verts.push_back(glm::vec3(-.6f, -.9f, 0.f));
	radar.verts.push_back(glm::vec3(-.9f, -.6f, 0.f));
	radar.verts.push_back(glm::vec3(-.9f, -.9f, 0.f));
	radar.uvs.push_back(glm::vec2(-1.f, -1.f));
	radar.uvs.push_back(glm::vec2(-1.f, 1.f));
	radar.uvs.push_back(glm::vec2(1.f, -1.f));
	radar.uvs.push_back(glm::vec2(1.f, 1.f));
	radar.drawMode = GL_TRIANGLE_STRIP;
	assignBuffers(radar);
	setBufferData(radar);
}

RenderingEngine::~RenderingEngine() {

}

void RenderingEngine::RenderScene(const std::vector<Geometry>& objects) {
	//Clears the screen to a dark grey background

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//sets uniforms
	GLint cameraGL = glGetUniformLocation(shaderProgram, "cameraPos");
	GLint lightGL = glGetUniformLocation(shaderProgram, "light");
	GLint shadeGL = glGetUniformLocation(shaderProgram, "shade");
	GLint transformGL = glGetUniformLocation(shaderProgram, "transform");
	glm::mat4 perspectiveMatrix = glm::perspective(PI_F*.4f, 512.f / 512.f, .1f, 200.f);
	glm::mat4 modelViewProjection = perspectiveMatrix * game_state->camera.viewMatrix();
	glm::vec4 light4 = modelViewProjection * glm::vec4(game_state->light, 1.0);
	glm::vec3 light = glm::vec3(light4.x, light4.y, light4.z);

	glUseProgram(shaderProgram);
	glUniform3fv(cameraGL, 1, &(game_state->camera.pos.x));
	glUniform3fv(lightGL, 1, &(light.x));
	glUniform1i(shadeGL, game_state->shading_model);
	GLint uniformLocation = glGetUniformLocation(shaderProgram, "modelViewProjection");
	glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(modelViewProjection));


	//set the shader uniforms
	/*shaders->setUniform("camera", gCamera.matrix());
	shaders->setUniform("model", inst.transform);
	shaders->setUniform("materialTex", 0); //set to 0 because the texture will be bound to GL_TEXTURE0
	shaders->setUniform("materialShininess", asset->shininess);
	shaders->setUniform("materialSpecularColor", asset->specularColor);
	shaders->setUniform("light.position", gLight.position);
	shaders->setUniform("light.intensities", gLight.intensities);
	shaders->setUniform("light.attenuation", gLight.attenuation);
	shaders->setUniform("light.ambientCoefficient", gLight.ambientCoefficient);
	shaders->setUniform("cameraPosition", gCamera.position());
	*/
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shaderProgram);

	for (const Geometry& g : objects) {
		//bind the texture
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, asset->texture->object());

		glUniformMatrix4fv(transformGL, 1, false, &(g.transform[0][0]));
		glBindVertexArray(g.vao);
		glDrawArrays(g.drawMode, 0, g.verts.size());

		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
	}

	//render health bar
	GLint healthGL = glGetUniformLocation(healthshaderProgram, "health");
	glUseProgram(healthshaderProgram);
	glUniform1f(healthGL, game_state->playerVehicle.health);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(health.vao);
	glDrawArrays(health.drawMode, 0, health.verts.size());

	//render radar
	glUseProgram(radarshaderProgram);
	GLint enemiesGL = glGetUniformLocation(radarshaderProgram, "enemies");
	GLint numenemiesGL = glGetUniformLocation(radarshaderProgram, "numenemies");
	GLint playerposGL = glGetUniformLocation(radarshaderProgram, "playerpos");
	std::vector<glm::vec2> enemy_locations;
	for (int i = 0; i < game_state->Enemies.size(); i++) {
		enemy_locations.push_back(glm::vec2(game_state->Enemies[i].position.x, game_state->Enemies[i].position.z));
	}
	//std::cout << enemy_locations[0].x << " " << enemy_locations[0].y << std::endl;
	glUniform2fv(enemiesGL, enemy_locations.size(), &(enemy_locations[0].x));
	glUniform2f(playerposGL, game_state->playerVehicle.position.x, game_state->playerVehicle.position.z);
	glUniform1i(numenemiesGL, enemy_locations.size());
	glBindVertexArray(radar.vao);
	glDrawArrays(radar.drawMode, 0, radar.verts.size());

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

	glGenBuffers(1, &geometry.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(3);

	glGenBuffers(1, &geometry.normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

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