/*
* Scene.cpp
*
*  Created on: Sep 10, 2018
*  Author: John Hall
*/

#define _CRT_SECURE_NO_WARNINGS

#include "Scene.h"
#include "Gamestate.h"

#include "RenderingEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include "glad/glad.h"
#include <GLFW/glfw3.h>

Scene::Scene(RenderingEngine* renderer, Gamestate* newGamestate) : renderer(renderer) {
	gameState = newGamestate;
	gameState->scene = this;

	Geometry ground;
	ground.verts.push_back(glm::vec3(-250.f, 0.0f, -250.f));
	ground.verts.push_back(glm::vec3(250.f, 0.0f, -250.f));
	ground.verts.push_back(glm::vec3(-250.f, 0.0f, 250.f));
	ground.verts.push_back(glm::vec3(250.f, 0.0f, 250.f));
	for (int i = 0; i < 4; i++) {
		ground.colors.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
		ground.normals.push_back(glm::vec3(0.f, 1.f, 0.f));
	}

	ground.transform = glm::mat4(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
	);

	ground.drawMode = GL_TRIANGLE_STRIP;
	RenderingEngine::assignBuffers(ground);
	RenderingEngine::setBufferData(ground);
	objects.push_back(ground);


	//generateRectPrism(5.0, 3.0, 2.0);
}

void Scene::setGamestate(Gamestate* newGamestate) {
	gameState = newGamestate;
}

void Scene::loadOBJObject(const char* filepath, glm::vec3 color) {
	Geometry OBJobject;
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	bool openSuccessful = true;
	int count = 0;
	//printf("1\n");
	FILE * file = fopen(filepath, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		openSuccessful = false;
		//return false;
	}
	while (openSuccessful) {
		//printf("2\n");
		//count++;
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {
			openSuccessful = false;
			break; // EOF = End Of File. Quit the loop.
		}
		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			//printf("6\n");
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			//printf("7\n");
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			//printf("8\n");
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			//printf("9\n");
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read properly: (Faces larger than triangles\n");
				//return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		OBJobject.verts.push_back(temp_vertices[vertexIndices[i] - 1]);
	}
	for (unsigned int j = 0; j < uvIndices.size(); j++) {
		OBJobject.uvs.push_back(temp_uvs[uvIndices[j] - 1]);
	}
	for (unsigned int k = 0; k < normalIndices.size(); k++) {
		OBJobject.colors.push_back(color);
		OBJobject.normals.push_back(temp_normals[normalIndices[k] - 1]);
	}
	OBJobject.drawMode = GL_TRIANGLES;
	RenderingEngine::assignBuffers(OBJobject);
	RenderingEngine::setBufferData(OBJobject);
	objects.push_back(OBJobject);
}

int Scene::generateRectPrism(float length, float width, float height) {
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

	//Casting to signed int to resolve warning of signed/unsigned comparison
	for (int i = 0; i < (signed int)box.verts.size(); i++) {
		box.colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	}


	box.drawMode = GL_TRIANGLE_STRIP;
	RenderingEngine::assignBuffers(box);
	RenderingEngine::setBufferData(box);
	objects.push_back(box);

	sceneObjectIndex++;
	return(sceneObjectIndex);
}


Scene::~Scene() {

}

void Scene::displayScene() {
	//glUseProgram(renderer->shaderProgramList[0]);
	GLuint shader = renderer->GetShaderProgram("gamePlayShader");
	renderer->SwitchShaderProgram(shader);

	for (int i = 1; i <= sceneObjectIndex; i++) {
		objects[i].transform = gameState->getEntityTransformation(i);
	}
	//objects[1].transform = gameState->playerVehicle.transformationMatrix;

	renderer->RenderScene(objects);
}
