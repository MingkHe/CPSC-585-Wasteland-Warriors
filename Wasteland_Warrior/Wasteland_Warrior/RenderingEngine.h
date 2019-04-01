
/*
* RenderingEngine.h
*
*  Created on: Sep 10, 2018
*      Author: John Hall
*/

#ifndef RENDERINGENGINE_H_
#define RENDERINGENGINE_H_

//**Must include glad and GLFW in this order or it breaks**
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Geometry.h"
#include "Gamestate.h"
#include <sstream>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Framebuffer.h"

//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
struct GLFWwindow;

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class RenderingEngine {
public:
	RenderingEngine(Gamestate *gameState);
	virtual ~RenderingEngine();
	Gamestate *game_state;
	//std::vector<GLuint> shaderProgramList;

	std::map<std::string, GLuint> shaderProgramList;

	std::map<GLchar, Character> Characters;

	//load font [normal use 128 font]
	void loadFont(const char* ttfFile);
	//push render text into geometry array
	void pushTextObj(std::vector<Geometry>& objects, std::string text, float x, float y, float scale, glm::vec3 color);
	//update text information
	void updateText();

	//Renders each object
	void SwitchShaderProgram(GLuint shader);
	// the shader program will be loaded into the shaderProgramList map.
	void LoadShaderProgram(std::string name, const char* vertexFile, const char* fragmentFile);
	//Get the shader use its name seted by the LoadShaderProgram method
	GLuint GetShaderProgram(std::string name);
	void RenderScene(const std::vector<Geometry>& objects);
	void RenderScene(const std::vector<CompositeWorldObject>& objects);
	void RenderMenuScene(const std::vector<Geometry>& objects);
	void RenderMenuSceneClear(const std::vector<Geometry>& objects);

	//Create vao and vbos for objects
	static void assignBuffers(Geometry& geometry);
	static void setBufferData(Geometry& geometry);
	static void deleteBufferData(Geometry& geometry);

	//Ensures that vao and vbos are set up properly
	bool CheckGLErrors();

//private:
	//Pointer to the current shader program being used to render
	GLuint shaderProgram;
	GLuint healthshaderProgram;
	GLuint radarshaderProgram;
	GLuint basicshaderProgram;
	GLuint shadowshaderProgram;
	GLuint needleshaderProgram;
	Geometry health;
	Geometry radar;
	Geometry speedo;
	Geometry needle;
	Geometry mirror;
	Geometry square;

	Framebuffer shadow_buffer;
	Framebuffer shadow_buffertwo;
	Framebuffer rear_view;
	Framebuffer main_view;

	GLuint textShaderProgram;

	std::vector<Geometry> texObjects;

	
};

#endif /* RENDERINGENGINE_H_ */



