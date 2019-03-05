
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
	
	textShaderProgram = ShaderTools::InitializeShaders("../shaders/texVertex.glsl", "../shaders/texFragment.glsl");

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

	float increment = (2.f*3.1415926536) / 50;
	glm::vec3 center = glm::vec3(.75f, -.75f, 0.f);
	glm::vec3 scale = glm::vec3(.125f);

	for (float i = 0; i < 2.f*3.1415926536; i += increment) {
		speedo.verts.push_back(glm::vec3(std::cos(i), std::sin(i), 0.f)*scale + center);
		speedo.verts.push_back(glm::vec3(std::cos(i + increment), std::sin(i + increment), 0.f)*scale + center);
		speedo.verts.push_back(center);

		speedo.uvs.push_back(glm::vec2(std::cos(i), std::sin(i)));
		speedo.uvs.push_back(glm::vec2(std::cos(i + increment), std::sin(i + increment)));
		speedo.uvs.push_back(glm::vec2(0, 0));
	}
	speedo.drawMode = GL_TRIANGLES;
	assignBuffers(speedo);
	setBufferData(speedo);

	needle.verts.push_back(glm::vec3(std::cos(7.f*PI_F / 6.f), std::sin(7.f*PI_F / 6.f), 0.f)*scale + center);
	needle.verts.push_back(glm::vec3(std::cos(5.f*PI_F / 3.f), std::sin(5.f*PI_F / 3.f), 0.f)*scale + center);
	needle.verts.push_back(glm::vec3(std::cos(2.f*PI_F / 3.f), std::sin(2.f*PI_F / 3.f), 0.f)*scale + center);
	needle.drawMode = GL_TRIANGLES;
	assignBuffers(needle);
	setBufferData(needle);

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
	//Clears the screen to a dark grey background
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//sets uniforms

	glm::mat4 perspectiveMatrix = glm::perspective(PI_F*.4f, 512.f / 512.f, .1f, 500.f); // last argument changed from 200 to 500 to increase view range
	glm::mat4 modelViewProjection = perspectiveMatrix * game_state->camera.viewMatrix();

	glUseProgram(shaderProgram);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
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

	int objectNum = objects.size();
	for (int i = 0; i < objectNum; i++) {
		glUniformMatrix4fv(transformGL, 1, false, glm::value_ptr(objects[i].geometry[0].transform));
		//bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, objects[i].geometry[0].texture.textureID);

		glBindVertexArray(objects[i].geometry[0].vao);
		glDrawArrays(objects[i].geometry[0].drawMode, 0, objects[i].geometry[0].verts.size());

		// reset state to default (no shader or geometry bound)
	}
	glBindVertexArray(0);
	//}

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
	GLint playerdirGL = glGetUniformLocation(radarshaderProgram, "playerdir");
	GLint radar_distGL = glGetUniformLocation(radarshaderProgram, "radar_dist");
	std::vector<glm::vec2> enemy_locations;
	for (int i = 0; i < game_state->Enemies.size(); i++) {
		enemy_locations.push_back(glm::vec2(game_state->Enemies[i].position.x, game_state->Enemies[i].position.z));
	}
	//std::cout << enemy_locations[0].x << " " << enemy_locations[0].y << std::endl;
	glUniform2fv(enemiesGL, enemy_locations.size(), &(enemy_locations[0].x));
	glUniform2f(playerposGL, game_state->playerVehicle.position.x, game_state->playerVehicle.position.z);
	glUniform2f(playerdirGL, game_state->playerVehicle.direction.x, game_state->playerVehicle.direction.y);
	glUniform1i(numenemiesGL, enemy_locations.size());
	glUniform1f(radar_distGL, game_state->radar_view);
	glBindVertexArray(radar.vao);
	glDrawArrays(radar.drawMode, 0, radar.verts.size());

	//render speedometer
	glUseProgram(basicshaderProgram);
	GLint istexGL = glGetUniformLocation(basicshaderProgram, "istex");
	glUniform1i(istexGL, 1);
	glBindVertexArray(speedo.vao);
	glDrawArrays(speedo.drawMode, 0, speedo.verts.size());
	glBindVertexArray(0);
	glm::vec3 center = glm::vec3(.75f, -.75f, 0.f);
	glm::vec3 scale = glm::vec3(.125f);
	float speed = game_state->playerVehicle.speed*.075f;
	needle.verts[0] = (glm::vec3(std::cos(7.f*PI_F / 6.f - speed), std::sin(7.f*PI_F / 6.f - speed), 0.f)*scale + center);
	needle.verts[1] = (glm::vec3(std::cos(5.f*PI_F / 3.f - speed), std::sin(5.f*PI_F / 3.f - speed), 0.f)*scale*.0625f + center);
	needle.verts[2] = (glm::vec3(std::cos(2.f*PI_F / 3.f - speed), std::sin(2.f*PI_F / 3.f - speed), 0.f)*scale*.0625f + center);
	setBufferData(needle);
	glUniform1i(istexGL, 0);
	glBindVertexArray(needle.vao);
	glDrawArrays(needle.drawMode, 0, needle.verts.size());
	glBindVertexArray(0);

	//render text
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	updateText();

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(textShaderProgram);

	//text color:
	glUniform3f(glGetUniformLocation(textShaderProgram, "textColor"), 0.7f, 0.2f, 0.2f);
	//glActiveTexture(GL_TEXTURE0);

	for (Geometry& g : texObjects) {
		
		//bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g.textureID);

		glBindVertexArray(g.vao);
		glDrawArrays(g.drawMode, 0, g.verts.size());

		// reset state to default (no shader or geometry bound)
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
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

}

void RenderingEngine::pushTextObj(std::vector<Geometry>& objects, std::string text, float x, float y, float scale) {
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

		assignBuffers(character);
		setBufferData(character);

		objects.push_back(character);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
}

void RenderingEngine::updateText() {

	pushTextObj(texObjects, "wave # " + std::to_string(game_state->wave), 0.01f*game_state->window_width, 0.95*game_state->window_height, 1.0f);

	if (game_state->breakSeconds == 0) {
		pushTextObj(texObjects, "enemies left: " + std::to_string(game_state->enemiesLeft), 0.01f*game_state->window_width, 0.9*game_state->window_height, 1.0f);
	}
	else {
		pushTextObj(texObjects, "break seconds: " + std::to_string(game_state->breakSeconds), 0.01f*game_state->window_width, 0.85*game_state->window_height, 1.0f);
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