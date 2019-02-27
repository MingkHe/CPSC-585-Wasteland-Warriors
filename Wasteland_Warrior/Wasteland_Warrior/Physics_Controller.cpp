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
	0.0f,		0.2f,
	5.0f,		0.2f,
	30.0f,		0.125f,
	120.0f,		0.1f,
	PX_MAX_F32, PX_MAX_F32,
	PX_MAX_F32, PX_MAX_F32,
	PX_MAX_F32, PX_MAX_F32,
	PX_MAX_F32, PX_MAX_F32
};*/

PxF32 gSteerVsForwardSpeedData[2 * 8] =
{
	0.1*PX_MAX_F32,		0.5f,
	0.2*PX_MAX_F32,		0.45f,
	0.3*PX_MAX_F32,		0.4f,
	0.4*PX_MAX_F32,		0.35f,
	0.5*PX_MAX_F32,		0.3f,
	0.6*PX_MAX_F32,		0.25f,
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
	const PxVec3 chassisDims(3.0f, 2.0f, 5.0f);
	const PxVec3 chassisMOI
		((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass/12.0f,
		 (chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.6f*chassisMass/12.0f,
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
	gVehicleSceneQueryData = VehicleSceneQueryData::allocate(1, PX_MAX_NB_WHEELS, 1, 1, WheelSceneQueryPreFilterBlocking, NULL, gAllocator); 
	gBatchQuery = VehicleSceneQueryData::setUpBatchedSceneQuery(0, *gVehicleSceneQueryData, gScene);

	//Create the friction table for each combination of tire and surface type.
	gFrictionPairs = createFrictionPairs(gMaterial);

	//Create a plane to drive on.
	PxFilterData groundPlaneSimFilterData(COLLISION_FLAG_GROUND, COLLISION_FLAG_GROUND_AGAINST, 0, 0);
	gGroundPlane = createDrivablePlane(groundPlaneSimFilterData, gMaterial, gPhysics);
	gScene->addActor(*gGroundPlane);

	createVehicle();
	createVehicle();

	startBrakeMode();
}

int Physics_Controller::createVehicle() {
	//Create a vehicle that will drive on the plane.
	VehicleDesc vehicleDesc = initPlayerVehiclePhysicsDesc();
	gVehicle4W = createVehicle4W(vehicleDesc, gPhysics, gCooking);
	PxTransform startTransform(PxVec3(0, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0.0f), PxQuat(PxIdentity));
	gVehicle4W->getRigidDynamicActor()->setGlobalPose(startTransform);
	gScene->addActor(*gVehicle4W->getRigidDynamicActor());

	//Set the vehicle to rest in first gear.
	//Set the vehicle to use auto-gears.
	gVehicle4W->setToRestState();
	gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
	gVehicle4W->mDriveDynData.setUseAutoGears(true);

	rigidDynamicActorNumber++;
	return rigidDynamicActorNumber;
}

void Physics_Controller::setPosition(int actorIndex, glm::vec3 newLocation){
	PxU32 numOfRidg = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
	PxActor *userBuffer[50];

	PxU32 numOfRidgActors = gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, userBuffer, numOfRidg, 0);
	PxActor *actor = userBuffer[actorIndex];
	PxRigidActor *rigidActor = actor->is<PxRigidActor>();

	//PxTransform
	rigidActor->setGlobalPose({ newLocation.x, newLocation.y, newLocation.z });
}

void userDriveInput(bool WKey, bool AKey, bool SKey, bool DKey, bool SPACEKey) {
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

	if ((WKey) && !(SKey) && !(SPACEKey))/*Check if high-order bit is set (1 << 15)*/
	{
		if (currentGear < 0) {
			currentGear = 1;
			changeToForwardGear = true;
		}

		if ((AKey) && !(DKey))
		{
			steerDirection = "left";
			if (SPACEKey) {
				startHandbrakeTurnRightMode();
				
			}
			else {
				startTurnHardRightMode();
				
			}
		}
		else if ((DKey) && !(AKey))
		{
			steerDirection = "right";
			if (SPACEKey) {
				
				startHandbrakeTurnLeftMode();
			}
			else {
				startTurnHardLeftMode();
				
			}
		}
		else {
			steerDirection = "straight";
			startAccelerateForwardsMode();
		}
	}

	else if ((SKey) && !(WKey) && !(SPACEKey))/*Check if high-order bit is set (1 << 15)*/
	{
		if (currentGear > 0) {
			currentGear = -1;
			changeToReverseGear = true;
		}
		if ((AKey) && !(DKey))
		{
			steerDirection = "left";
			if (SPACEKey) {
				startHandbrakeTurnRightMode();

			}
			else {
				startTurnHardRightMode();

			}
		}
		else if ((DKey) && !(AKey))
		{
			steerDirection = "right";
			if (SPACEKey) {

				startHandbrakeTurnLeftMode();
			}
			else {
				startTurnHardLeftMode();

			}
		}
		else {
			steerDirection = "straight";
			startAccelerateReverseMode();
		}
	}

	if ((SPACEKey) || ((WKey) && (SKey))) {
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

	userDriveInput(gameState->WKey, gameState->AKey, gameState->SKey, gameState->DKey, gameState->SPACEKey);
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

	PxU32 numOfRidgActors = gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, userBuffer, numOfRidg, 0);
	PxActor *box = userBuffer[1];
	PxRigidActor *rigidActor = box->is<PxRigidActor>();
	
	//std::cout << "Number of Ridged objects: " << numOfRidgActors << std::endl; //Test statement, delete it if you want


	PxTransform orientation = rigidActor->getGlobalPose();		//   https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/apireference/files/classPxRigidActor.html
	PxVec3 location = orientation.p;							//	https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/apireference/files/classPxTransform.html
	PxQuat rotation = orientation.q;			
	
	PxVec3 xRotation = rotation.getBasisVector0();
	PxVec3 yRotation = rotation.getBasisVector1();
	PxVec3 zRotation = rotation.getBasisVector2();

	gameState->playerVehicle.position.x = location.x;
	gameState->playerVehicle.position.y = location.y;
	gameState->playerVehicle.position.z = location.z;

	
	gameState->playerVehicle.transformationMatrix[0] = {xRotation.x, xRotation.y, xRotation.z, 0.0f };
	gameState->playerVehicle.transformationMatrix[1] = {yRotation.x, yRotation.y, yRotation.z, 0.0f };
	gameState->playerVehicle.transformationMatrix[2] = {zRotation.x, zRotation.y, zRotation.z, 0.0f };
	gameState->playerVehicle.transformationMatrix[3] = {location.x , location.y , location.z , 1.0f };
	
	std::cout << "Box position:  X:" << location.x << "  Y:" << location.y << "  Z:" << location.z << std::endl; //Test statement, delete it if you want

	
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