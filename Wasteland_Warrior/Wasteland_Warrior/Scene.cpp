/*
* Scene.cpp
*
*  Created on: Sep 10, 2018
*  Author: John Hall
*/

#define _CRT_SECURE_NO_WARNINGS

#include "Scene.h"
#include "Gamestate.h"
#include "texture.h"

#include "RenderingEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

Scene::Scene(RenderingEngine* renderer, Gamestate* newGamestate) : renderer(renderer) {
	gameState = newGamestate;
	gameState->scene = this;

	CompositeWorldObject groundComp;
	Geometry ground;

	ground.textureFilePath = "Texture/redLines.jpg";
	InitializeTexture(&ground.texture, ground.textureFilePath);
	glBindTexture(GL_TEXTURE_2D, ground.texture.textureID);

	ground.verts.push_back(glm::vec3(-250.f, -50.0f, -250.f));
	ground.verts.push_back(glm::vec3(250.f, -50.0f, -250.f));
	ground.verts.push_back(glm::vec3(-250.f, -50.0f, 250.f));
	ground.verts.push_back(glm::vec3(250.f, -50.0f, 250.f));
	for (int i = 0; i < 4; i++) {
		ground.colors.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
		ground.normals.push_back(glm::vec3(0.f, 1.f, 0.f));
		ground.uvs.push_back(glm::vec2(1.0f, 1.0f));
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
	groundComp.geometry.push_back(ground);
	objects.push_back(groundComp);
	
	//loadOBJObject("Objects/testLevel.obj", "Textures/redLines.jpg");


}

void Scene::setGamestate(Gamestate* newGamestate) {
	gameState = newGamestate;
}

int Scene::loadOBJObject(const char* filepath, const char* textureFilepath) {
	CompositeWorldObject OBJobjectComp;
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
				printf("File can't be read properly: (Faces larger than 3 triangles\n");
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

	OBJobject.transform = glm::mat4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	OBJobject.textureFilePath = textureFilepath;
	InitializeTexture(&OBJobject.texture, OBJobject.textureFilePath);
	OBJobject.vertsPhys = temp_vertices;
	OBJobject.faceVertexIndices = vertexIndices;
	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		OBJobject.verts.push_back(temp_vertices[vertexIndices[i] - 1]);
	}
	for (unsigned int j = 0; j < uvIndices.size(); j++) {
		OBJobject.uvs.push_back(temp_uvs[uvIndices[j] - 1]);
		//std::cout << " " << temp_uvs[uvIndices[j] - 1].x << " " << temp_uvs[uvIndices[j] - 1].y << std::endl;
	}
	for (unsigned int k = 0; k < normalIndices.size(); k++) {
		OBJobject.colors.push_back(glm::vec3(0.5,0.5,0.5));
		OBJobject.normals.push_back(temp_normals[normalIndices[k] - 1]);
	}
	OBJobject.drawMode = GL_TRIANGLES;
	RenderingEngine::assignBuffers(OBJobject);
	RenderingEngine::setBufferData(OBJobject);
	OBJobjectComp.geometry.push_back(OBJobject);
	objects.push_back(OBJobjectComp);

	sceneObjectIndex++;
	return(sceneObjectIndex);
}

int Scene::generateRectPrism(float length, float width, float height) {
	CompositeWorldObject boxComp;
	Geometry box;
	float carL = width; // Actually is width
	float carW = length; //Actually is length
	float carH = height;

	box.transform = glm::mat4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);

	box.textureFilePath = "Texture/sandTexture.jpg";
	InitializeTexture(&box.texture, box.textureFilePath);
	glBindTexture(GL_TEXTURE_2D, box.texture.textureID);

	//Front Side
	box.verts.push_back({ carL, carH, carW });
	box.verts.push_back({ -carL, carH, carW });
	box.verts.push_back({ carL, -carH, carW });
	box.verts.push_back({ -carL, -carH, carW });
	for (int i = 0; i < 4; i++) {
		box.normals.push_back(glm::vec3(1.f, 0.f, 0.f));
		box.uvs.push_back(glm::vec2(i*0.2f, i*0.2f));
	}

	//Back
	box.verts.push_back({ carL, carH, -carW });
	box.verts.push_back({ -carL, carH, -carW });
	box.verts.push_back({ carL, -carH, -carW });
	box.verts.push_back({ -carL, -carH, -carW });
	for (int i = 0; i < 4; i++) {
		box.normals.push_back(glm::vec3(-1.f, 0.f, 0.f));
		box.uvs.push_back(glm::vec2(i*0.2f, i*0.2f));
	}

	//Left
	box.verts.push_back({ carL, carH, carW });
	box.verts.push_back({ carL, -carH, carW });
	box.verts.push_back({ carL, carH, -carW });
	box.verts.push_back({ carL, -carH, -carW });
	for (int i = 0; i < 4; i++) {
		box.normals.push_back(glm::vec3(0.f, 0.f, -1.f));
		box.uvs.push_back(glm::vec2(i*0.2f, i*0.2f));
	}

	//Right
	box.verts.push_back({ -carL, carH, carW });
	box.verts.push_back({ -carL, -carH, carW });
	box.verts.push_back({ -carL, carH, -carW });
	box.verts.push_back({ -carL, -carH, -carW });

	for (int i = 0; i < 4; i++) {
		box.normals.push_back(glm::vec3(0.f, 0.f, 1.f));
		box.uvs.push_back(glm::vec2(i*0.2f, i*0.2f));
	}

	//Top
	box.verts.push_back({ carL, carH, carW });
	box.verts.push_back({ -carL, carH, carW });
	box.verts.push_back({ carL, carH, -carW });
	box.verts.push_back({ -carL, carH, -carW });
	for (int i = 0; i < 4; i++) {
		box.normals.push_back(glm::vec3(0.f, 1.f, 0.f));
		box.uvs.push_back(glm::vec2(i*0.2f, i*0.2f));
	}

	//Bottom
	box.verts.push_back({ carL, -carH, carW });
	box.verts.push_back({ -carL, -carH, carW });
	box.verts.push_back({ carL, -carH, -carW });
	box.verts.push_back({ -carL, -carH, -carW });

	for (int i = 0; i < 4; i++) {
		box.normals.push_back(glm::vec3(0.f, -1.f, 0.f));
		box.uvs.push_back(glm::vec2(i*0.2f, i*0.2f));
	}

	//Casting to signed int to resolve warning of signed/unsigned comparison
	for (int i = 0; i < (signed int)box.verts.size(); i++) {
		box.colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	}


	box.drawMode = GL_TRIANGLE_STRIP;
	RenderingEngine::assignBuffers(box);
	RenderingEngine::setBufferData(box);
	boxComp.geometry.push_back(box);
	objects.push_back(boxComp);
	
	sceneObjectIndex++;
	return(sceneObjectIndex);
}


Scene::~Scene() {

}


void Scene::displayScene() {
	
	//glUseProgram(renderer->shaderProgramList[0]);
	GLuint shader = renderer->GetShaderProgram("gamePlayShader");
	renderer->SwitchShaderProgram(shader);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderer->shaderProgram);
	//Update Player Position
	int vehicleIndex = gameState->playerVehicle.sceneObjectIndex;
	objects[vehicleIndex].geometry[0].transform = gameState->getEntityTransformation(vehicleIndex);

	int numOfEnemies = gameState->Enemies.size();
	int index = 0;
	for (int i = 0; i < numOfEnemies; i++) {
		index = gameState->Enemies[i].sceneObjectIndex;
		objects[index].geometry[0].transform = gameState->getEntityTransformation(index);
		

	}
	/*int numOfPowerUps = gameState->DynamicObjects.size();
	index = 0;
	for (int i = 0; i < numOfPowerUps; i++) {
		index = gameState->DynamicObjects[i].sceneObjectIndex;
		std::cout << index << std::endl;
		objects[index].geometry[0].transform = gameState->getEntityTransformation(index);
		std::cout << objects[index].geometry[0].transform[0][3] << " " << objects[index].geometry[0].transform[1][3] << " " << objects[index].geometry[0].transform[2][3] << " " <<std::endl;

	}*/
	renderer->RenderScene(objects);
	


}
