
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
	basicshaderProgram = ShaderTools::InitializeShaders("../shaders/basicvertex.glsl", "../shaders/basicfragment.glsl");
	needleshaderProgram = ShaderTools::InitializeShaders("../shaders/needlevertex.glsl", "../shaders/needlefragment.glsl");
	shadowshaderProgram = ShaderTools::InitializeShaders("../shaders/shadowMapVertex.glsl", "../shaders/shadowMapFragment.glsl");
	imageShaderProgram = ShaderTools::InitializeShaders("../shaders/vertexMainMenu.glsl", "../shaders/fragmentMainMenu.glsl");
	lineShaderProgram = ShaderTools::InitializeShaders("../shaders/basicVer.glsl", "../shaders/basicFrag.glsl");
	
	textShaderProgram = ShaderTools::InitializeShaders("../shaders/texVertex.glsl", "../shaders/texFragment.glsl");
	float aspect_ratio = game_state->window_height / game_state->window_width;
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

	float increment = (2.f*3.1415926536f) / 50.f;
	glm::vec3 center = glm::vec3(.75f, -.75f, 0.f);
	glm::vec3 scale = glm::vec3(.125f);

	for (float i = 0; i < 2.f*3.1415926536; i += increment) {
		speedo.verts.push_back(glm::vec3(std::cos(i), std::sin(i), 0.f)*scale + center);
		speedo.verts.push_back(glm::vec3(std::cos(i + increment), std::sin(i + increment), 0.f)*scale + center);
		speedo.verts.push_back(center);

		speedo.uvs.push_back(.5f*glm::vec2(std::cos(i), std::sin(i)) + glm::vec2(.5f, .5f));
		speedo.uvs.push_back(.5f*glm::vec2(std::cos(i + increment), std::sin(i + increment)) + glm::vec2(.5f, .5f));
		speedo.uvs.push_back(glm::vec2(.5f, .5f));
	}
	speedo.drawMode = GL_TRIANGLES;
	InitializeTexture(&speedo.texture, "Image/speedo.png");
	assignBuffers(speedo);
	setBufferData(speedo);

	needle.verts.push_back(glm::vec3(0, 1, 0.f));
	needle.verts.push_back(glm::vec3(1, 0, 0.f)*.0625f);
	needle.verts.push_back(glm::vec3(-1, 0, 0.f)*.0625f);
	needle.colors.push_back(glm::vec3(1.f, 0.f, 0.f));
	needle.colors.push_back(glm::vec3(1.f, 0.f, 0.f));
	needle.colors.push_back(glm::vec3(1.f, 0.f, 0.f));
	needle.drawMode = GL_TRIANGLES;
	assignBuffers(needle);
	setBufferData(needle);

	aimBeam.verts.push_back(glm::vec3(gameState->playerVehicle.position.x + 3 * gameState->playerVehicle.direction.x, gameState->playerVehicle.position.y + 3 * gameState->playerVehicle.direction.y, gameState->playerVehicle.position.z + 3 * gameState->playerVehicle.direction.z));
	aimBeam.verts.push_back(glm::vec3(gameState->playerVehicle.position.x + 13 * gameState->playerVehicle.direction.x, gameState->playerVehicle.position.y + 13 * gameState->playerVehicle.direction.y, gameState->playerVehicle.position.z + 13 * gameState->playerVehicle.direction.z));
	aimBeam.colors.push_back(glm::vec3(.0f,.0f,1.0f));
	aimBeam.colors.push_back(glm::vec3(.0f, .0f, 1.0f));
	aimBeam.drawMode = GL_LINES;
	assignBuffers(aimBeam);
	setBufferData(aimBeam);

	/*mirror.verts.push_back(glm::vec3(-1.f, -1.f, 0.f));
	mirror.verts.push_back(glm::vec3(-1.f, 1.f, 0.f));
	mirror.verts.push_back(glm::vec3(1.f, -1.f, 0.f));
	mirror.verts.push_back(glm::vec3(1.f, 1.f, 0.f));
	mirror.uvs.push_back(glm::vec2(0.f, 0.f));
	mirror.uvs.push_back(glm::vec2(0.f, 1.f));
	mirror.uvs.push_back(glm::vec2(1.f, 0.f));
	mirror.uvs.push_back(glm::vec2(1.f, 1.f));*/
	mirror.verts.push_back(glm::vec3(-.4f, .75f, 0.f));
	mirror.verts.push_back(glm::vec3(-.4f, .95f, 0.f));
	mirror.verts.push_back(glm::vec3(.4f, .75f, 0.f));
	mirror.verts.push_back(glm::vec3(.4f, .95f, 0.f));
	mirror.uvs.push_back(glm::vec2(1.f, .4f));
	mirror.uvs.push_back(glm::vec2(1.f, .85f));
	mirror.uvs.push_back(glm::vec2(0.f, .4f));
	mirror.uvs.push_back(glm::vec2(0.f, .85f));
	mirror.drawMode = GL_TRIANGLE_STRIP;
	assignBuffers(mirror);
	setBufferData(mirror);

	aim.verts.push_back(glm::vec3(-0.1f,-0.1f,.0f));
	aim.verts.push_back(glm::vec3(0.1f, -0.1f, .0f));
	aim.verts.push_back(glm::vec3(0.1f, 0.1f, .0f));
	aim.verts.push_back(glm::vec3(-0.1f, -0.1f, .0f));
	aim.verts.push_back(glm::vec3(0.1f, 0.1f, .0f));
	aim.verts.push_back(glm::vec3(-0.1f, 0.1f, .0f));
	aim.uvs.push_back(glm::vec2(.0f, .0f));
	aim.uvs.push_back(glm::vec2(1.f, .0f));
	aim.uvs.push_back(glm::vec2(1.f, 1.f));
	aim.uvs.push_back(glm::vec2(.0f, .0f));
	aim.uvs.push_back(glm::vec2(1.f, 1.f));
	aim.uvs.push_back(glm::vec2(0.f, 1.f));
	aim.drawMode = GL_TRIANGLES;
	InitializeTexture(&aim.texture, "Image/aim2.png");
	assignBuffers(aim);
	setBufferData(aim);

	rear_view = createFramebuffer(game_state->window_width, game_state->window_height);
	shadow_buffer = createFramebuffer(game_state->window_width, game_state->window_height);

	//the code to load the font, may be do some refactor in the future.

	loadFont("Fonts/lora/Lora-Bold.ttf");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(gameState->window_width), 0.0f, static_cast<GLfloat>(gameState->window_height));
	glUseProgram(textShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(textShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUseProgram(0);
}

RenderingEngine::~RenderingEngine() {

}

void RenderingEngine::RenderScene(const std::vector<CompositeWorldObject>& objects) {
	glm::mat4 perspectiveMatrix = glm::perspective(PI_F*.4f, (float)game_state->window_width / (float)game_state->window_height, .1f, 750.f); // last argument changed from 200 to 500 to increase view range
	glm::mat4 depthperspectiveMatrix = glm::perspective(PI_F*.2f, (float)game_state->window_width / (float)game_state->window_height, 50.f, 250.f);
	//setting up framebuffer stuff
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer.id);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowshaderProgram);
	GLint transformGL = glGetUniformLocation(shadowshaderProgram, "transform");
	//glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(game_state->light, game_state->playerVehicle.position, glm::vec3(0, 1, 0));
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthperspectiveMatrix * depthViewMatrix;
	GLuint depthMatrixID = glGetUniformLocation(shadowshaderProgram, "modelViewProjection");
	glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
	for (int i = 0; i < (int)objects.size(); i++) {
		if (i == game_state->skyboxIndex) {
			continue;
		}
		glUniformMatrix4fv(transformGL, 1, false, glm::value_ptr(objects[i].subObjects[0].transform));
		glBindVertexArray(objects[i].subObjects[0].vao);
		glDrawArrays(objects[i].subObjects[0].drawMode, 0, objects[i].subObjects[0].verts.size());
	}
	
	//Clears the screen to a dark grey background
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//sets uniforms
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 modelViewProjection = perspectiveMatrix * game_state->camera.viewMatrix();

	glUseProgram(shaderProgram);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	transformGL = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelViewProjection"), 1, false, glm::value_ptr(modelViewProjection));

	depthMatrixID = glGetUniformLocation(shaderProgram, "depthViewProjection");
	glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);

	glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPosition"), 1, glm::value_ptr(game_state->camera.pos));
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightPosition"), 1, glm::value_ptr(game_state->light));
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightColour"), 1, glm::value_ptr(game_state->lightColor));
	glUniform1f(glGetUniformLocation(shaderProgram, "lightAttenuation"), game_state->lightAttenuation);
	glUniform1f(glGetUniformLocation(shaderProgram, "lightAmbientCoeff"), game_state->lightAmbientCoefficient);

	glUniform3fv(glGetUniformLocation(shaderProgram, "materialSpecularColor"), 1, glm::value_ptr(game_state->materialSpecularColor));
	glUniform1f(glGetUniformLocation(shaderProgram, "materialShininess"), game_state->materialShininess);
	glUniform1i(glGetUniformLocation(shaderProgram, "materialTex"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "shadowTex"), 1);

	//draw rear view
	int objectNum = objects.size();
	for (int i = 0; i < objectNum; i++) {
		if (i == game_state->skyboxIndex) {
			glUniform1i(glGetUniformLocation(shaderProgram, "isSkybox"), 1);
		}
		else {
			glUniform1i(glGetUniformLocation(shaderProgram, "isSkybox"), 0);
		}
		if (i == game_state->groundIndex) {
			glUniform1i(glGetUniformLocation(shaderProgram, "isGround"), 1);
		}
		else {
			glUniform1i(glGetUniformLocation(shaderProgram, "isGround"), 0);
		}
		glUniformMatrix4fv(transformGL, 1, false, glm::value_ptr(objects[i].subObjects[0].transform));
		//bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, objects[i].subObjects[0].texture.textureID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadow_buffer.depthTextureID);

		glBindVertexArray(objects[i].subObjects[0].vao);
		glDrawArrays(objects[i].subObjects[0].drawMode, 0, objects[i].subObjects[0].verts.size());
	}

	//draw actual frame
	glBindFramebuffer(GL_FRAMEBUFFER, rear_view.id);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	modelViewProjection = perspectiveMatrix * game_state->camera.backviewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelViewProjection"), 1, false, glm::value_ptr(modelViewProjection));
	for (int i = 0; i < objectNum; i++) {
		if (i == game_state->skyboxIndex) {
			glUniform1i(glGetUniformLocation(shaderProgram, "isSkybox"), 1);
		}
		else {
			glUniform1i(glGetUniformLocation(shaderProgram, "isSkybox"), 0);
		}
		glUniformMatrix4fv(transformGL, 1, false, glm::value_ptr(objects[i].subObjects[0].transform));
		//bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, objects[i].subObjects[0].texture.textureID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadow_buffer.depthTextureID);

		glBindVertexArray(objects[i].subObjects[0].vao);
		glDrawArrays(objects[i].subObjects[0].drawMode, 0, objects[i].subObjects[0].verts.size());

		// reset state to default (no shader or geometry bound)
	}

	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	//render mirror
	glUseProgram(basicshaderProgram);
	glUniform1i(glGetUniformLocation(basicshaderProgram, "materialTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rear_view.colorTextureID);
	glBindVertexArray(mirror.vao);
	glDrawArrays(mirror.drawMode, 0, mirror.verts.size());
	glBindVertexArray(0);

	//render health bar
	glUseProgram(healthshaderProgram);
	glUniform1f(glGetUniformLocation(healthshaderProgram, "health"), game_state->playerVehicle.health);
	glUniform1f(glGetUniformLocation(healthshaderProgram, "maxhealth"), 100);
	glBindVertexArray(health.vao);
	glDrawArrays(health.drawMode, 0, health.verts.size());

	//render radar
	glUseProgram(radarshaderProgram);
	GLint enemiesGL = glGetUniformLocation(radarshaderProgram, "enemies");
	GLint numenemiesGL = glGetUniformLocation(radarshaderProgram, "numenemies");
	GLint highlightsGL = glGetUniformLocation(radarshaderProgram, "highlights");
	GLint numhighlightGL = glGetUniformLocation(radarshaderProgram, "numhighlight");
	GLint playerposGL = glGetUniformLocation(radarshaderProgram, "playerpos");
	GLint playerdirGL = glGetUniformLocation(radarshaderProgram, "playerdir");
	GLint radar_distGL = glGetUniformLocation(radarshaderProgram, "radar_dist");
	std::vector<glm::vec2> enemy_locations;
	std::vector<glm::vec2> highlighted_locations;
	for (int i = 0; i < (int)game_state->Enemies.size(); i++) {
		if (game_state->Enemies[i].AIType == 1) {
			highlighted_locations.push_back(glm::vec2(game_state->Enemies[i].position.x, game_state->Enemies[i].position.z));
		} else {
			enemy_locations.push_back(glm::vec2(game_state->Enemies[i].position.x, game_state->Enemies[i].position.z));
		}
	}
	if(enemy_locations.size()!=0)
		glUniform2fv(enemiesGL, enemy_locations.size(), &(enemy_locations[0].x));
	if (highlighted_locations.size() != 0)
		glUniform2fv(highlightsGL, highlighted_locations.size(), &(highlighted_locations[0].x));
	glUniform2f(playerposGL, game_state->playerVehicle.position.x, game_state->playerVehicle.position.z);
	glUniform2f(playerdirGL, game_state->playerVehicle.direction.x, game_state->playerVehicle.direction.z);
	glUniform1i(numenemiesGL, enemy_locations.size());
	glUniform1i(numhighlightGL, highlighted_locations.size());
	glUniform1f(radar_distGL, game_state->radar_view);
	glBindVertexArray(radar.vao);
	glDrawArrays(radar.drawMode, 0, radar.verts.size());

	//render speedometer
	glUseProgram(basicshaderProgram);
	glUniform1i(glGetUniformLocation(basicshaderProgram, "materialTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, speedo.texture.textureID);
	glBindVertexArray(speedo.vao);
	glDrawArrays(speedo.drawMode, 0, speedo.verts.size());
	glBindVertexArray(0);

	glUseProgram(needleshaderProgram);
	float speed = game_state->playerVehicle.speed;
	glUniform1f(glGetUniformLocation(needleshaderProgram, "speed"), speed);
	glBindVertexArray(needle.vao);
	glDrawArrays(needle.drawMode, 0, needle.verts.size());
	glBindVertexArray(0);


	glUseProgram(lineShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(lineShaderProgram, "modelViewProjection"), 1, false, glm::value_ptr(modelViewProjection));
	glBindVertexArray(aimBeam.vao);
	glDrawArrays(aimBeam.drawMode, 0, aimBeam.verts.size());
	glBindVertexArray(0);
	glUseProgram(0);

	//render aim
	//GLuint shader = GetShaderProgram("menuShader");
	//SwitchShaderProgram(shader);
	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(basicshaderProgram);
	glBindVertexArray(aim.vao);
	glUniform1i(glGetUniformLocation(basicshaderProgram, "materialTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, aim.texture.textureID);

	glDrawArrays(aim.drawMode, 0, aim.verts.size());

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	glDisable(GL_BLEND);
	*/

	//render text
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	updateText();

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(textShaderProgram);

	//text color:
	//glUniform3f(glGetUniformLocation(textShaderProgram, "textColor"), 0.7f, 0.2f, 0.2f);
	//glActiveTexture(GL_TEXTURE0);

	for (Geometry& g : texObjects) {
		
		//bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g.textureID);

		glBindVertexArray(g.vao);
		glUniform3f(glGetUniformLocation(textShaderProgram, "textColor"), g.colors[0].x, g.colors[0].y, g.colors[0].z);
		glDrawArrays(g.drawMode, 0, g.verts.size());

		// reset state to default (no shader or geometry bound)
		deleteBufferData(g);
		glBindVertexArray(0);
	}
	
	texObjects.clear();

	glUseProgram(textShaderProgram);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, 0);
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
  
  updateText();

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(textShaderProgram);

	//text color:
	//glUniform3f(glGetUniformLocation(textShaderProgram, "textColor"), 0.7f, 0.2f, 0.2f);
	//glActiveTexture(GL_TEXTURE0);

	for (Geometry& g : texObjects) {

		//bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g.textureID);

		glBindVertexArray(g.vao);
		glUniform3f(glGetUniformLocation(textShaderProgram, "textColor"), g.colors[0].x, g.colors[0].y, g.colors[0].z);

		glDrawArrays(g.drawMode, 0, g.verts.size());

		// reset state to default (no shader or geometry bound)
		deleteBufferData(g);
		glBindVertexArray(0);
	}

	texObjects.clear();

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, 0);
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

void RenderingEngine::loadFont(const char* ttfFile) {

	//clear
	Characters.clear();

	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, ttfFile, 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)(face->glyph->advance.x)
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

}

void RenderingEngine::pushTextObj(std::vector<Geometry>& objects, std::string text, float x, float y, float scale, glm::vec3 color) {
	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		Geometry character;
		character.verts.push_back(glm::vec3(xpos, ypos + h, 0.0f));
		character.verts.push_back(glm::vec3(xpos, ypos, 0.0f));
		character.verts.push_back(glm::vec3(xpos + w, ypos, 0.0f));

		character.verts.push_back(glm::vec3(xpos, ypos + h, 0.0f));
		character.verts.push_back(glm::vec3(xpos + w, ypos, 0.0f));
		character.verts.push_back(glm::vec3(xpos + w, ypos + h, 0.0f));

		character.uvs.push_back(glm::vec2(0.0f, 0.0f));
		character.uvs.push_back(glm::vec2(0.0f, 1.0f));
		character.uvs.push_back(glm::vec2(1.0f, 1.0f));

		character.uvs.push_back(glm::vec2(0.0f, 0.0f));
		character.uvs.push_back(glm::vec2(1.0f, 1.0f));
		character.uvs.push_back(glm::vec2(1.0f, 0.0f));

		/*
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		*/
		character.textureID = ch.TextureID;
		character.drawMode = GL_TRIANGLES;
		character.colors.push_back(color);

		assignBuffers(character);
		setBufferData(character);

		objects.push_back(character);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
}

void RenderingEngine::updateText() {
	float scale = (float)game_state->window_height / 960.f;
	if (game_state->UIMode == "Game") {
		pushTextObj(texObjects, "Wave # " + std::to_string(game_state->wave) + " - " + game_state->gameMode, 0.01f*game_state->window_width, 0.95f*game_state->window_height, scale, glm::vec3(0.7f, 0.2f, 0.2f));
		if (game_state->breakSeconds == 0) {	
			pushTextObj(texObjects, "Enemies Left: " + std::to_string(game_state->enemiesLeft), 0.01f*game_state->window_width, 0.9f*game_state->window_height, scale, glm::vec3(0.7f, 0.2f, 0.2f));
		}
		else {
			pushTextObj(texObjects, "Break Seconds: " + std::to_string(game_state->breakSeconds), 0.01f*game_state->window_width, 0.85f*game_state->window_height, scale, glm::vec3(0.7f, 0.2f, 0.2f));
		}
    
		if (game_state->powerText) {
			switch (game_state->powerUpType)
			{
			case 0:
				pushTextObj(texObjects, "You have reached a checkpoint!", 0.3f*game_state->window_width, 0.8f*game_state->window_height, scale, glm::vec3(0.7f, 0.2f, 0.2f));
				break;
			case 1:
				pushTextObj(texObjects, "You have been healed to full health!", 0.3f*game_state->window_width, 0.8f*game_state->window_height, scale, glm::vec3(0.7f, 0.2f, 0.2f));
				break;
			case 2:
				pushTextObj(texObjects, "You maximum health has been increaced!", 0.3f*game_state->window_width, 0.8f*game_state->window_height, scale, glm::vec3(0.7f, 0.2f, 0.2f));
				break;
			case 3:
				pushTextObj(texObjects, "You have recieved a health boost!", 0.3f*game_state->window_width, 0.8f*game_state->window_height, scale, glm::vec3(0.7f, 0.2f, 0.2f));
				break;
			case 4:
				pushTextObj(texObjects, "You have recieved an armor boost!", 0.3f*game_state->window_width, 0.8f*game_state->window_height, scale, glm::vec3(0.7f, 0.2f, 0.2f));
				break;
			case 5:
				pushTextObj(texObjects, "You have recieved a damage boost!", 0.3f*game_state->window_width, 0.8f*game_state->window_height, scale, glm::vec3(0.7f, 0.2f, 0.2f));
				break;
			default:
				break;
			}
		}
	}

	if (game_state->UIMode == "Win") {
		pushTextObj(texObjects, "Your score was: " + std::to_string(game_state->score), 0.4f*game_state->window_width, 0.45f*game_state->window_height, scale, glm::vec3(.9f, 1.0f, .4f));
		pushTextObj(texObjects, "You survived in: " + std::to_string(game_state->scoreTime), 0.4f*game_state->window_width, 0.38f*game_state->window_height, scale,glm::vec3(.9f, 1.0f, .4f));
	}

	if (game_state->UIMode == "Lose") {
		pushTextObj(texObjects, "Your score was: " + std::to_string(game_state->score), 0.4f*game_state->window_width, 0.4f*game_state->window_height, scale, glm::vec3(.7f, .2f, .2f));
		pushTextObj(texObjects, "You died after: " + std::to_string(game_state->scoreTime), 0.4f*game_state->window_width, 0.33f*game_state->window_height, scale, glm::vec3(.7f, .2f, .2f));
	}

	if (game_state->UIMode == "Loading") {
		pushTextObj(texObjects, std::to_string(game_state->loadingPercentage)+"%", 0.7f*game_state->window_width, 0.315f*game_state->window_height, scale, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	if (game_state->UIMode == "Story") {
		pushTextObj(texObjects, "Press Enter to continue...", 0.65f*game_state->window_width, 0.1f*game_state->window_height, scale, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	if (game_state->UIMode == "Control") {
		pushTextObj(texObjects, "Press Enter to continue...", 0.65f*game_state->window_width, 0.1f*game_state->window_height, scale, glm::vec3(1.0f, 1.0f, 1.0f));
	}
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
