#include "Physics_Controller.h"

#include <ctype.h>

#include "PxPhysicsAPI.h"

#include "../include/vehicle/PxVehicleUtil.h"
#include "../include/snippetFiles/snippetvehiclecommon/SnippetVehicleSceneQuery.h"
#include "../include/snippetFiles/snippetvehiclecommon/SnippetVehicleFilterShader.h"
#include "../include/snippetFiles/snippetvehiclecommon/SnippetVehicleTireFriction.h"
#include "../include/snippetFiles/snippetvehiclecommon/SnippetVehicleCreate.h"

#include "../include/snippetFiles/snippetcommon/SnippetPrint.h"
#include "../include/snippetFiles/snippetcommon/SnippetPVD.h"
#include "../include/snippetFiles/snippetutils/SnippetUtils.h"

#include <PxScene.h>
#include <iostream>

using namespace physx;
using namespace snippetvehicle;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene = NULL;

PxCooking*				gCooking = NULL;

PxMaterial*				gMaterial = NULL;

PxPvd*                  gPvd = NULL;

VehicleSceneQueryData*	gVehicleSceneQueryData = NULL;
PxBatchQuery*			gBatchQuery = NULL;

PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;

PxRigidStatic*			gGroundPlane = NULL;
PxVehicleDrive4W*		gVehicle4W = NULL;

bool					gIsVehicleInAir = true;
std::string				steerDirection = "straight";
bool					brakeCar = false;
int                     currentGear = 1;
bool					changeToReverseGear = false;
bool					changeToForwardGear = false;



Physics_Controller::Physics_Controller(Gamestate* newGameState)
{
	gameState = newGameState;
	initPhysics(false);
}

Physics_Controller::~Physics_Controller()
{
	cleanupPhysics(false);
}

void Physics_Controller::Update()
{
	//std::cout << "Updating Physics\n" << std::endl;
	stepPhysics(false);
}

/*PxF32 gSteerVsForwardSpeedData[2*8]=
{
	0.0f,		0.75f,
	5.0f,		0.75f,
	30.0f,		0.125f,
	120.0f,		0.1f,
	PX_MAX_F32, PX_MAX_F32,
	PX_MAX_F32, PX_MAX_F32,
	PX_MAX_F32, PX_MAX_F32,
	PX_MAX_F32, PX_MAX_F32
};*/

PxF32 gSteerVsForwardSpeedData[2 * 8] =
{
	0.1*PX_MAX_F32,		0.8f,
	0.2*PX_MAX_F32,		0.7f,
	0.3*PX_MAX_F32,		0.6f,
	0.4*PX_MAX_F32,		0.5f,
	0.5*PX_MAX_F32,		0.4f,
	0.6*PX_MAX_F32,		0.3f,
	0.7*PX_MAX_F32,		0.2f,
	0.8*PX_MAX_F32,		0.1f
};


PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable(gSteerVsForwardSpeedData, 4);

PxVehicleKeySmoothingData gKeySmoothingData =
{
	{
		6.0f,	//rise rate eANALOG_INPUT_ACCEL
		6.0f,	//rise rate eANALOG_INPUT_BRAKE		
		6.0f,	//rise rate eANALOG_INPUT_HANDBRAKE	
		2.5f,	//rise rate eANALOG_INPUT_STEER_LEFT
		2.5f,	//rise rate eANALOG_INPUT_STEER_RIGHT
	},
	{
		10.0f,	//fall rate eANALOG_INPUT_ACCEL
		10.0f,	//fall rate eANALOG_INPUT_BRAKE		
		10.0f,	//fall rate eANALOG_INPUT_HANDBRAKE	
		5.0f,	//fall rate eANALOG_INPUT_STEER_LEFT
		5.0f	//fall rate eANALOG_INPUT_STEER_RIGHT
	}
};

PxVehiclePadSmoothingData gPadSmoothingData =
{
	{
		6.0f,	//rise rate eANALOG_INPUT_ACCEL
		6.0f,	//rise rate eANALOG_INPUT_BRAKE		
		6.0f,	//rise rate eANALOG_INPUT_HANDBRAKE	
		2.5f,	//rise rate eANALOG_INPUT_STEER_LEFT
		2.5f,	//rise rate eANALOG_INPUT_STEER_RIGHT
	},
	{
		10.0f,	//fall rate eANALOG_INPUT_ACCEL
		10.0f,	//fall rate eANALOG_INPUT_BRAKE		
		10.0f,	//fall rate eANALOG_INPUT_HANDBRAKE	
		5.0f,	//fall rate eANALOG_INPUT_STEER_LEFT
		5.0f	//fall rate eANALOG_INPUT_STEER_RIGHT
	}
};

PxVehicleDrive4WRawInputData gVehicleInputData;

PxU32					gVehicleOrderProgress = 0;
bool					gVehicleOrderComplete = false;
bool					gMimicKeyInputs = false;

VehicleDesc initPlayerVehiclePhysicsDesc()
{
	//Set up the chassis mass, dimensions, moment of inertia, and center of mass offset.
	//The moment of inertia is just the moment of inertia of a cuboid but modified for easier steering.
	//Center of mass offset is 0.65m above the base of the chassis and 0.25m towards the front.
	const PxF32 chassisMass = 1500.0f;
	const PxVec3 chassisDims(2.5f, 2.0f, 5.0f);
	const PxVec3 chassisMOI
		((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass/12.0f,
		 (chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.8f*chassisMass/12.0f,
		 (chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass/12.0f);
	/*const PxVec3 chassisMOI
	((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass,
		(chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.8f*chassisMass,
		(chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass);*/
	const PxVec3 chassisCMOffset(0.0f, -chassisDims.y*0.5f + 0.65f, 0.25f);

	//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
	//Moment of inertia is just the moment of inertia of a cylinder.
	const PxF32 wheelMass = 20.0f;
	const PxF32 wheelRadius = 0.5f;
	const PxF32 wheelWidth = 0.4f;
	const PxF32 wheelMOI = 0.5f*wheelMass*wheelRadius*wheelRadius;
	const PxU32 nbWheels = 4;

	VehicleDesc vehicleDesc;

	vehicleDesc.chassisMass = chassisMass;
	vehicleDesc.chassisDims = chassisDims;
	vehicleDesc.chassisMOI = chassisMOI;
	vehicleDesc.chassisCMOffset = chassisCMOffset;
	vehicleDesc.chassisMaterial = gMaterial;
	vehicleDesc.chassisSimFilterData = PxFilterData(COLLISION_FLAG_CHASSIS, COLLISION_FLAG_CHASSIS_AGAINST, 0, 0);

	vehicleDesc.wheelMass = wheelMass;
	vehicleDesc.wheelRadius = wheelRadius;
	vehicleDesc.wheelWidth = wheelWidth;
	vehicleDesc.wheelMOI = wheelMOI;
	vehicleDesc.numWheels = nbWheels;
	vehicleDesc.wheelMaterial = gMaterial;
	vehicleDesc.chassisSimFilterData = PxFilterData(COLLISION_FLAG_WHEEL, COLLISION_FLAG_WHEEL_AGAINST, 0, 0);

	return vehicleDesc;
}

void startAccelerateForwardsMode()
{

	if (gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalAccel(true);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(1.0f);
	}
}

void startAccelerateReverseMode()
{
	gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eREVERSE);

	if (gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalAccel(true);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(1.0f);
	}
}

void startBrakeMode()
{
	if (gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalBrake(true);
	}
	else
	{
		gVehicleInputData.setAnalogBrake(1.0f);
	}
}

void startTurnHardLeftMode()
{
	if (gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalAccel(true);
		gVehicleInputData.setDigitalSteerLeft(true);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(true);
		gVehicleInputData.setAnalogSteer(-1.0f);
	}
}

void startTurnHardRightMode()
{
	if (gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalAccel(true);
		gVehicleInputData.setDigitalSteerRight(true);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(1.0f);
		gVehicleInputData.setAnalogSteer(1.0f);
	}
}

void startHandbrakeTurnLeftMode()
{
	if (gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalSteerLeft(true);
		gVehicleInputData.setDigitalHandbrake(true);
	}
	else
	{
		gVehicleInputData.setAnalogSteer(-1.0f);
		gVehicleInputData.setAnalogHandbrake(1.0f);
	}
}

void startHandbrakeTurnRightMode()
{
	if (gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalSteerRight(true);
		gVehicleInputData.setDigitalHandbrake(true);
	}
	else
	{
		gVehicleInputData.setAnalogSteer(1.0f);
		gVehicleInputData.setAnalogHandbrake(1.0f);
	}
}


void releaseAllControls()
{
	if (gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalAccel(false);
		gVehicleInputData.setDigitalSteerLeft(false);
		gVehicleInputData.setDigitalSteerRight(false);
		gVehicleInputData.setDigitalBrake(false);
		gVehicleInputData.setDigitalHandbrake(false);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(0.0f);
		gVehicleInputData.setAnalogSteer(0.0f);
		gVehicleInputData.setAnalogBrake(0.0f);
		gVehicleInputData.setAnalogHandbrake(0.0f);
	}
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
	PxU32 numWorkers = 1;
	gDispatcher = PxDefaultCpuDispatcherCreate(numWorkers);
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

	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(PxTolerancesScale()));

	PxInitVehicleSDK(*gPhysics);
	PxVehicleSetBasisVectors(PxVec3(0, 1, 0), PxVec3(0, 0, 1));
	PxVehicleSetUpdateMode(PxVehicleUpdateMode::eVELOCITY_CHANGE);
	

	//Create the batched scene queries for the suspension raycasts.
	gVehicleSceneQueryData = VehicleSceneQueryData::allocate(1, PX_MAX_NB_WHEELS, 1, 1, WheelSceneQueryPreFilterBlocking, NULL, gAllocator); //!!
	gBatchQuery = VehicleSceneQueryData::setUpBatchedSceneQuery(0, *gVehicleSceneQueryData, gScene);

	//Create the friction table for each combination of tire and surface type.
	gFrictionPairs = createFrictionPairs(gMaterial);

	//Create a plane to drive on.
	PxFilterData groundPlaneSimFilterData(COLLISION_FLAG_GROUND, COLLISION_FLAG_GROUND_AGAINST, 0, 0);
	gGroundPlane = createDrivablePlane(groundPlaneSimFilterData, gMaterial, gPhysics);
	gScene->addActor(*gGroundPlane);

	//Create a vehicle that will drive on the plane.
	VehicleDesc vehicleDesc = initPlayerVehiclePhysicsDesc();
	gVehicle4W = createVehicle4W(vehicleDesc, gPhysics, gCooking);
	PxTransform startTransform(PxVec3(0, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0), PxQuat(PxIdentity));
	gVehicle4W->getRigidDynamicActor()->setGlobalPose(startTransform);
	gScene->addActor(*gVehicle4W->getRigidDynamicActor());

	//Set the vehicle to rest in first gear.
	//Set the vehicle to use auto-gears.
	gVehicle4W->setToRestState();
	gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
	gVehicle4W->mDriveDynData.setUseAutoGears(true);

	startBrakeMode();
}

void userDriveInput(std::string userInput) {
	releaseAllControls();
	steerDirection = "";
	brakeCar = false;
	//currentGear
	//changeToForwardGear
	//changeToReverseGear

	if (changeToForwardGear)
	{
		changeToForwardGear = false;
		gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
	}

	if ((GetKeyState('W') & 0x8000) && !(GetKeyState('S') & 0x8000))/*Check if high-order bit is set (1 << 15)*/
	{
		if (currentGear < 0) {
			currentGear = 1;
			changeToForwardGear = true;
		}

		if ((GetKeyState('A') & 0x8000) && !(GetKeyState('D') & 0x8000))
		{
			steerDirection = "left";
			if (GetKeyState('C') & 0x8000) {
				startHandbrakeTurnRightMode();
				
			}
			else {
				startTurnHardRightMode();
				
			}
		}
		else if ((GetKeyState('D') & 0x8000) && !(GetKeyState('A') & 0x8000))
		{
			steerDirection = "right";
			if (GetKeyState('C') & 0x8000) {
				
				startHandbrakeTurnLeftMode();
			}
			else {
				startTurnHardLeftMode();
				
			}
		}
		else
			steerDirection = "straight";
			startAccelerateForwardsMode();
	}

	else if ((GetKeyState('S') & 0x8000) && !(GetKeyState('W') & 0x8000))/*Check if high-order bit is set (1 << 15)*/
	{
		if (currentGear > 0) {
			currentGear = -1;
			changeToReverseGear = true;
		}
		if ((GetKeyState('A') & 0x8000) && !(GetKeyState('D') & 0x8000))
		{
			steerDirection = "left";
			if (GetKeyState('C') & 0x8000) {
				startHandbrakeTurnRightMode();

			}
			else {
				startTurnHardRightMode();

			}
		}
		else if ((GetKeyState('D') & 0x8000) && !(GetKeyState('A') & 0x8000))
		{
			steerDirection = "right";
			if (GetKeyState('C') & 0x8000) {

				startHandbrakeTurnLeftMode();
			}
			else {
				startTurnHardLeftMode();

			}
		}
		else
			steerDirection = "straight";
		startAccelerateReverseMode();
	}

	if ((GetKeyState('C') & 0x8000) || ((GetKeyState('W') & 0x8000) && (GetKeyState('S') & 0x8000))) {
		startBrakeMode();
		
	}

	//If the mode about to start is eDRIVE_MODE_ACCEL_REVERSE then switch to reverse gears.
	if (changeToReverseGear)
	{
		changeToReverseGear = false;
		gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eREVERSE);
	}

}

void Physics_Controller::stepPhysics(bool interactive)
{
	PX_UNUSED(interactive);
	const PxF32 timestep = 1.0f / 60.0f;

	userDriveInput(gameState->button);
	//Update the control inputs for the vehicle.
	if (gMimicKeyInputs)
	{
		PxVehicleDrive4WSmoothDigitalRawInputsAndSetAnalogInputs(gKeySmoothingData, gSteerVsForwardSpeedTable, gVehicleInputData, timestep, gIsVehicleInAir, *gVehicle4W);
	}
	else
	{
		PxVehicleDrive4WSmoothAnalogRawInputsAndSetAnalogInputs(gPadSmoothingData, gSteerVsForwardSpeedTable, gVehicleInputData, timestep, gIsVehicleInAir, *gVehicle4W);
	}

	//Raycasts.
	PxVehicleWheels* vehicles[1] = { gVehicle4W };
	PxRaycastQueryResult* raycastResults = gVehicleSceneQueryData->getRaycastQueryResultBuffer(0);
	const PxU32 raycastResultsSize = gVehicleSceneQueryData->getQueryResultBufferSize();
	PxVehicleSuspensionRaycasts(gBatchQuery, 1, vehicles, raycastResultsSize, raycastResults);

	//Vehicle update.
	const PxVec3 grav = gScene->getGravity();
	PxWheelQueryResult wheelQueryResults[PX_MAX_NB_WHEELS];
	PxVehicleWheelQueryResult vehicleQueryResults[1] = { {wheelQueryResults, gVehicle4W->mWheelsSimData.getNbWheels()} };
	PxVehicleUpdates(timestep, grav, *gFrictionPairs, 1, vehicles, vehicleQueryResults);

	//Work out if the vehicle is in the air.
	gIsVehicleInAir = gVehicle4W->getRigidDynamicActor()->isSleeping() ? false : PxVehicleIsInAir(vehicleQueryResults[0]);

	//Scene update.
	gScene->simulate(timestep);
	gScene->fetchResults(true);


	PxU32 numOfRidg = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
	PxActor *userBuffer[50];

	//std::cout << "Number of obj:" << numOfRidg;
	PxU32 numOfRidgActors = gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, userBuffer, numOfRidg, 0);
	PxActor *box = userBuffer[0];
	//If we had a PxRigidActor 
	PxRigidActor *rigidActor = box->is<PxRigidActor>();
	
	PxTransform orientation = rigidActor->getGlobalPose();		//   https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/apireference/files/classPxRigidActor.html
	PxVec3 location = orientation.p;								//	https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/apireference/files/classPxTransform.html
	PxQuat rotation = orientation.q;			
	
	PxVec3 xRotation = rotation.getBasisVector0();
	PxVec3 yRotation = rotation.getBasisVector1();
	PxVec3 zRotation = rotation.getBasisVector2();


	//FIX THIS LATER!
	gameState->scene->objects[1].transform[3][0] = location.x;
	gameState->scene->objects[1].transform[3][1] = location.y;
	gameState->scene->objects[1].transform[3][2] = location.z;

	std::cout << "Box position:  X:" << yRotation.x << "  Y:" << yRotation.y << "  Z:" << yRotation.z << std::endl; //Test statement, delete it if you want

	
	gameState->scene->objects[1].transform[0][0] = xRotation.x;
	gameState->scene->objects[1].transform[0][1] = xRotation.y;
	gameState->scene->objects[1].transform[0][2] = xRotation.z;

	gameState->scene->objects[1].transform[1][0] = yRotation.x;
	gameState->scene->objects[1].transform[1][1] = yRotation.y;
	gameState->scene->objects[1].transform[1][2] = yRotation.z;

	gameState->scene->objects[1].transform[2][0] = zRotation.x;
	gameState->scene->objects[1].transform[2][1] = zRotation.y;
	gameState->scene->objects[1].transform[2][2] = zRotation.z;


	/*


	glm::mat4 transofrmationMatrix = 
		glm::mat4{
		{xRotation.x, yRotation.x, zRotation.x, 0.f},
		{xRotation.y, yRotation.y, zRotation.y, 0.f},
		{xRotation.z, yRotation.z, zRotation.z, 0.f},
		{0.0f		, 0.0f		 , 0.0f		  , 1.0f}
	};

	gameState->camera.pos = glm::vec4(location.x, (location.y+3), (location.z+5), 1.0f)*transofrmationMatrix;
	gameState->camera.dir = glm::vec4(0.0f, -5.0f, 0.0f, 0.0f)*transofrmationMatrix;
	gameState->camera.right = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)*transofrmationMatrix;
	gameState->camera.up = glm::cross(gameState->camera.dir, gameState->camera.right);*/
}


void Physics_Controller::cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);
	gVehicle4W->getRigidDynamicActor()->release();
	gVehicle4W->free();
	gGroundPlane->release();
	gBatchQuery->release();
	gVehicleSceneQueryData->free(gAllocator);
	gFrictionPairs->release();
	PxCloseVehicleSDK();

	gMaterial->release();
	gCooking->release();
	gScene->release();
	gDispatcher->release();
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	gFoundation->release();

	printf("SnippetVehicle4W done.\n");
}