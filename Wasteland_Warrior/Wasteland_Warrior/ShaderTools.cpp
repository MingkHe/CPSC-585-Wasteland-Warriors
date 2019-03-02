/*
* ShaderTools.cpp
*	Tools for parsing, compiling and linking shaders
*  Created on: Sep 10, 2018
*      Author: John Hall
*/

#include "ShaderTools.h"

#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <iterator>

//**Must include glad and GLFW in this order or it breaks**
#include "glad/glad.h"
#include <GLFW/glfw3.h>

//Shader associated functions are put in the ShaderTools namespace

std::string ShaderTools::LoadSource(const std::string &filename) {
	std::string source;

	std::ifstream input(filename.c_str());
	if (input) {
		std::copy(std::istreambuf_iterator<char>(input),
			std::istreambuf_iterator<char>(),
			back_inserter(source));
		input.close();
	} else {
		std::cout << "ERROR: Could not load shader source from file "
			<< filename << std::endl;
	}

	return source;
}

// creates and returns a shader object compiled from the given source
GLuint ShaderTools::CompileShader(GLenum shaderType, const std::string &source) {
	// allocate shader object name
	GLuint shaderObject = glCreateShader(shaderType);

	// try compiling the source as a shader of the given type
	const GLchar *source_ptr = source.c_str();
	glShaderSource(shaderObject, 1, &source_ptr, 0);
	glCompileShader(shaderObject);

	// retrieve compile status
	GLint status;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		std::string info(length, ' ');
		glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
		std::cout << "ERROR compiling shader:" << std::endl << std::endl;
		std::cout << source << std::endl;
		std::cout << info << std::endl;
	}

	return shaderObject;
}

// creates and returns a program object linked from vertex and fragment shaders
GLuint ShaderTools::LinkProgram(GLuint vertexShader, GLuint fragmentShader) {
	// allocate program object name
	GLuint programObject = glCreateProgram();

	// attach provided shader objects to this program
	if (vertexShader)   glAttachShader(programObject, vertexShader);
	if (fragmentShader) glAttachShader(programObject, fragmentShader);

	// try linking the program with given attachments
	glLinkProgram(programObject);

	// retrieve link status
	GLint status;
	glGetProgramiv(programObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
		std::string info(length, ' ');
		glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
		std::cout << "ERROR linking shader program:" << std::endl;
		std::cout << info << std::endl;
	}

	return programObject;
}

GLuint ShaderTools::LinkProgram(GLuint vertexShader, GLuint fragmentShader, GLint geoShader) {
	// allocate program object name
	GLuint programObject = glCreateProgram();

	// attach provided shader objects to this program
	if (vertexShader)   glAttachShader(programObject, vertexShader);
	if (fragmentShader) glAttachShader(programObject, fragmentShader);
	if (geoShader) glAttachShader(programObject, geoShader);

	// try linking the program with given attachments
	glLinkProgram(programObject);

	// retrieve link status
	GLint status;
	glGetProgramiv(programObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
		std::string info(length, ' ');
		glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
		std::cout << "ERROR linking shader program:" << std::endl;
		std::cout << info << std::endl;
	}

	return programObject;
}

GLuint ShaderTools::InitializeShaders(const char* vertexFile, const char* fragmentFile) {
	// load shader source from files
	std::string vertexSource = LoadSource(vertexFile);
	std::string fragmentSource = LoadSource(fragmentFile);
	//std::string geoSource = LoadSource("../shaders/geometry.glsl");
	if (vertexSource.empty() || fragmentSource.empty()) return false;

	// compile shader source into shader objects
	GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
	//GLuint geo = CompileShader(GL_VERTEX_SHADER, geoSource);

	// link shader program
	GLuint program = LinkProgram(vertex, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	//glDeleteShader(geo);

	// check for OpenGL errors and return false if error occurred
	return program;
}