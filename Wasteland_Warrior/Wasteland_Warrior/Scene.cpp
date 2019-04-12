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
#include <fstream>
#include <vector>
using namespace std;

Scene::Scene(RenderingEngine* renderer, Gamestate* newGamestate) : renderer(renderer) {
	gameState = newGamestate;
	gameState->scene = this;

	/*CompositeWorldObject groundComp;
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
	groundComp.subObjects.push_back(ground);
	allWorldCompObjects.push_back(groundComp);
	*/
	//loadOBJObject("Objects/testLevel.obj", "Textures/redLines.jpg");


}

void Scene::setGamestate(Gamestate* newGamestate) {
	gameState = newGamestate;
}


int Scene::loadOBJObjectInstance(std::vector<const char*> filepath, std::vector<const char*> textureFilepath) {
	CompositeWorldObject OBJobjectComposite;
	std::vector< unsigned int > vertexIndices[30], uvIndices[30], normalIndices[30];
	//std::vector < std::vector< std::vector< unsigned int > > > vertexIndices, uvIndices, normalIndices;
	//std::vector < std::vector< std::vector< glm::vec3 > > > temp_vertices;
	//std::vector < std::vector< std::vector< glm::vec2 > > > temp_uvs;
	//std::vector < std::vector< std::vector< glm::vec3 > > > temp_normals;
	std::vector< glm::vec3 > vertice_holder;
	std::vector< glm::vec2 > uv_holder;
	std::vector< glm::vec3 > normal_holder;
	std::vector<string> materialNames;
	int materialIndex;
	bool openSuccessful = true;
	int subobjectIndex = 0;
	previousHeader = 'v';
	printf("NewObject\n");
	FILE * file = fopen(filepath[0], "r");
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
		if (strcmp(lineHeader, "o") == 0) {
			char maybeWheel[128];
			fscanf(file, "%s\n", maybeWheel);
		}
		// else : parse lineHeader
		else if (strcmp(lineHeader, "v") == 0) {
			//printf("6\n");
			if (previousHeader == 'f') {

			}
			previousHeader = 'v';
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertice_holder.push_back(vertex);
			//temp_vertices[0].push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			//previousHeader = 't';
			//printf("7\n");
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.x = uv.x - floor(uv.x);
			uv.y = uv.y - floor(uv.y);
			uv_holder.push_back(uv);
			//temp_uvs[0].push_back(uv);

		}
		else if (strcmp(lineHeader, "vn") == 0) {
			//printf("8\n");
			//previousHeader = 'n';
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normal_holder.push_back(normal);
			//temp_normals[0].push_back(normal);
		}
		else if (strcmp(lineHeader, "usemtl") == 0) {
			char matName[128];
			string materialName;
			materialIndex = -1;
			fscanf(file, "%s\n", matName);
			materialName = matName;
			std::cout << materialName << std::endl;
			int numberOfMaterials = materialNames.size();
			for (int s = 0; s < numberOfMaterials; s++) {
				if (materialName == materialNames[s]) {
					materialIndex = s;
				}
			}
			if (materialIndex == -1) {
				materialNames.push_back(materialName);
				materialIndex = materialNames.size()-1;
			}

			//std::cout << materialName << " " << materialIndex << std::endl;
		}
		else if (strcmp(lineHeader, "f") == 0) {
			//printf("9\n");
			previousHeader = 'f';
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read properly: (Faces larger than 3 triangles\n");
				//return false;
			}
			//int objMatIndex = subObject_by_Material_count[materialIndex];
			//std::cout << materialIndex << std::endl;
			//std::cout << materialIndex << std::endl;
			vertexIndices[materialIndex].push_back(vertexIndex[0]);
			vertexIndices[materialIndex].push_back(vertexIndex[1]);
			vertexIndices[materialIndex].push_back(vertexIndex[2]);
			uvIndices[materialIndex].push_back(uvIndex[0]);
			uvIndices[materialIndex].push_back(uvIndex[1]);
			uvIndices[materialIndex].push_back(uvIndex[2]);
			normalIndices[materialIndex].push_back(normalIndex[0]);
			normalIndices[materialIndex].push_back(normalIndex[1]);
			normalIndices[materialIndex].push_back(normalIndex[2]);
		}
	}

	//temp_vertices[materialIndex].push_back(vertice_holder);
	//temp_uvs[materialIndex].push_back(uv_holder);
	//temp_normals[materialIndex].push_back(normal_holder);
	//subObject_by_Material_count[materialIndex]++;
	std::cout << materialNames.size() << std::endl;
	for (int objIndex = 0; objIndex < (int)materialNames.size(); objIndex++) {
		int textureIndex = objIndex;
		if (textureIndex >= (int)textureFilepath.size()) {
			textureIndex = 0;
		}
		OBJobjectComposite.subObjects.push_back(createObjectInstance(textureFilepath[textureIndex], vertexIndices[objIndex], uvIndices[objIndex], normalIndices[objIndex], vertice_holder, uv_holder, normal_holder, objIndex));
		sceneObjectIndex++;
		OBJobjectComposite.subobjectIndices.push_back(sceneObjectIndex);
		OBJobjectComposite.textureObjectIndexMap.push_back(objIndex);
	}

	//printf("%da\n", compObjectInstances.size());

	//printf("%db\n", compObjectInstances.size());
	//vertexIndices.clear();
	//uvIndices.clear();
	//normalIndices.clear();
	vertice_holder.clear();
	uv_holder.clear();
	normal_holder.clear();

	OBJobjectComposite.transform = glm::mat4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
	OBJobjectComposite.subObjectsCount = materialNames.size();
	compObjectInstances.push_back(OBJobjectComposite);
	objectInstanceIndex++;
	//printf("%dc\n", objectInstanceIndex);
	return(objectInstanceIndex);
}

Geometry Scene::createObjectInstance(const char* textureFilepath,
	std::vector< unsigned int > vertexIndices,  std::vector< unsigned int > uvIndices,  std::vector< unsigned int > normalIndices,
	std::vector< glm::vec3 > temp_vertices, std::vector< glm::vec2 > temp_uvs, std::vector< glm::vec3 > temp_normals, int materialIndex) {

	Geometry OBJobject;
	OBJobject.transform = glm::mat4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);

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
		OBJobject.colors.push_back(glm::vec3(0.5, 0.5, 0.5));
		OBJobject.normals.push_back(temp_normals[normalIndices[k] - 1]);
	}
	OBJobject.drawMode = GL_TRIANGLES;
	RenderingEngine::assignBuffers(OBJobject);
	RenderingEngine::setBufferData(OBJobject);
	//OBJobjectComp.subObjects.push_back(OBJobject);
	//std::cout << OBJobjectComp.subObjects.size() << std::endl;

	/*vertexIndices.clear();
	uvIndices.clear();
	normalIndices.clear();
	temp_vertices.clear();
	temp_uvs.clear();
	temp_normals.clear();*/
	//std::cout << sceneObjectIndex << std::endl;
	//sceneObjectIndex++;
	//OBJobjectComp.subobjectIndices.push_back(sceneObjectIndex);
	//OBJobjectComp.textureObjectIndexMap.push_back(textureIndex);
	return OBJobject;
	//compObjectInstances.push_back(OBJobjectComp);
}


int Scene::loadCompObjectInstance(int compObjIndex) {
	//std::cout << compObjectInstances.size() << std::endl;
	if (compObjIndex < (int)compObjectInstances.size()) {
		allWorldCompObjects.push_back(compObjectInstances[compObjIndex]);
		
		sceneCompObjectIndex++;
		//printf("%dc\n", sceneCompObjectIndex);
		return(sceneCompObjectIndex);
	}
	else {
		throw "Error loading comp object instance!";
	}
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
	boxComp.subObjects.push_back(box);
	allWorldCompObjects.push_back(boxComp);
	
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
	glm::mat4 newTransform = gameState->getEntityTransformation(vehicleIndex);
	//int s = 0;//-----------------------------------------
	for (int s = 0; s < allWorldCompObjects[vehicleIndex].subObjectsCount; s++) {
		allWorldCompObjects[vehicleIndex].subObjects[s].transform = newTransform;
	}

	int numOfEnemies = gameState->Enemies.size();
	int index = 0;
	for (int i = 0; i < numOfEnemies; i++) {
		index = gameState->Enemies[i].sceneObjectIndex;
		glm::mat4 newTransform = gameState->getEntityTransformation(index);
		//int s = 0;//-------------------------------
		for (int s = 0; s < allWorldCompObjects[index].subObjectsCount; s++) {
			allWorldCompObjects[index].subObjects[s].transform = newTransform;
		}
	}

	int numOfPowerUps = gameState->PowerUps.size();
	index = 0;
	for (int i = 0; i < numOfPowerUps; i++) {
		index = gameState->PowerUps[i].sceneObjectIndex;
		glm::mat4 newTransform = gameState->getEntityTransformation(index);
		//int s = 0;//---------------------------------------------------------------
		for (int j = 0; j < allWorldCompObjects[index].subObjectsCount; j++) {
			allWorldCompObjects[index].subObjects[j].transform = newTransform;
		}
	}

	renderer->RenderScene(allWorldCompObjects);
	
}
