#include "Physics_Controller.h"

#include <ctype.h>

#include "PxPhysicsAPI.h"

#include "../snippetFiles/snippetcommon/SnippetPrint.h"
#include "../snippetFiles/snippetcommon/SnippetPVD.h"
#include "../snippetFiles/snippetutils/SnippetUtils.h"

#include <PxScene.h>
#include <iostream>

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene = NULL;

PxMaterial*				gMaterial = NULL;

PxPvd*                  gPvd = NULL;

PxReal stackZ = 10.0f;



Physics_Controller::Physics_Controller(Gamestate* newGameState)
{
	gameState = newGameState;
	initPhysics(false);
}

Physics_Controller::~Physics_Controller()
{
	cleanupPhysics(false);
}

int Physics_Controller::Update()
{
	std::cout << "Updateing Physics\n" << std::endl;
	stepPhysics(false);
	return 0;
}



void Physics_Controller::initPhysics(bool interactive)
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	gScene->addActor(*groundPlane);



	PxReal halfExtent = 2.0f;
	PxU32 size = 10;
	const PxTransform& t = PxTransform(PxVec3(0, 0, 0));

	PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);

	PxTransform localTm(PxVec3(2, 10, 0) * halfExtent);
	PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	gScene->addActor(*body);

	shape->release();
}


void Physics_Controller::stepPhysics(bool interactive)
{
	PX_UNUSED(interactive);
	gScene->simulate(gameState->timeStep);
	gScene->fetchResults(true);

	PxU32 numOfRidg = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);

	PxActor *userBuffer[50];

	PxU32 numOfRidgActors = gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, userBuffer, numOfRidg, 0);
	PxActor *box = userBuffer[0];
	PxBounds3 bBox = box->getWorldBounds();
	PxVec3 xyzBox = bBox.getCenter();

	//std::cout << "Box position:  X:" << xyzBox.x << "  Y:" << xyzBox.y << "  Z:" << xyzBox.z << std::endl;
	gameState->scene->objects[1].transform[3][0] = xyzBox.x;//glm::vec3(xyzBox.x, xyzBox.y, xyzBox.z);
	gameState->scene->objects[1].transform[3][1] = xyzBox.y;
	gameState->scene->objects[1].transform[3][2] = xyzBox.z;
}


void Physics_Controller::cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);
	gScene->release();
	gDispatcher->release();
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();

	printf("SnippetHelloWorld done.\n");
}