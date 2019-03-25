#include "Physics_Controller.h"
#include "Gamestate.h"

#include <ctype.h>

#include "PxPhysicsAPI.h"

#include "vehicle/PxVehicleUtil.h"
#include "SnippetVehicleSceneQuery.h"
#include "SnippetVehicleFilterShader.h"
#include "SnippetVehicleTireFriction.h"
#include "SnippetVehicleCreate.h"

#include "snippetcommon/SnippetPrint.h"
#include "snippetcommon/SnippetPVD.h"
#include "snippetutils/SnippetUtils.h"

#include <PxScene.h>
#include "Scene.h"
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
PxCooking*				gCookingNoCleaning = NULL;

PxMaterial*				gMaterial = NULL;
PxMaterial*				tireMaterial = NULL;

PxPvd*                  gPvd = NULL;

VehicleSceneQueryData*	gVehicleSceneQueryData = NULL;
PxBatchQuery*			gBatchQuery = NULL;

PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;

PxRigidStatic*			gGroundPlane = NULL;
PxRigidStatic*			gMap1Ground = NULL;
PxRigidStatic*			gStaticObject = NULL;
PxRigidDynamic*			gDynamicObject = NULL;
PxVehicleDrive4W*		gVehicle4W = NULL;

PxVehicleDrive4W*		enemyVehicle = NULL;

std::vector<PxVehicleDrive4W*> vehiclesVector;

bool					gIsVehicleInAir = true;
std::string				steerDirection = "straight";
bool					brakeCar = false;
int                     currentGear = 1;
bool					changeToReverseGear = false;
bool					changeToForwardGear = false;








PxFilterFlags contactReportFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{

	// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		//pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		//return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_CONTACT_POINTS | PxPairFlag::eTRIGGER_DEFAULT;

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;
}






Physics_Controller::Physics_Controller(Gamestate* newGameState)
{
	gameState = newGameState;
	gameState->physics_Controller = this;
	gContactReportCallback = ContactReportCallback(newGameState);
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

PxF32 playerSteerVsForwardSpeedData[2 * 5] =
{
	0.0f,		1.0f,
	5.0f,		0.9f,	
	30.0f,		0.50f,
	120.0f,		0.9f,
	PX_MAX_F32, PX_MAX_F32
};

PxF32 enemySteerVsForwardSpeedData[2 * 5] =
{
	0.0f,		0.3f,
	5.0f,		0.1f,
	30.0f,		0.1f,
	120.0f,		0.05f,
	PX_MAX_F32, PX_MAX_F32
};
//Prior speed table
/*PxF32 enemySteerVsForwardSpeedData[2 * 10] =
{
	0.0f,		0.5f,
	3.0f,		0.45f,
	5.0f,		0.4f,
	7.0f,		0.35f,
	10.0f,		0.3f,
	15.0f,		0.20f,
	20.0f,		0.12f,
	30.0f,		0.10f,
	120.0f,		0.10f,
	PX_MAX_F32, PX_MAX_F32
};*/



PxFixedSizeLookupTable<8> playerSteerVsForwardSpeedTable(playerSteerVsForwardSpeedData, 5);
PxFixedSizeLookupTable<8> enemySteerVsForwardSpeedTable(enemySteerVsForwardSpeedData, 5);

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
PxVehicleDrive4WRawInputData enemyInputData;

PxU32					gVehicleOrderProgress = 0;
bool					gVehicleOrderComplete = false;
bool					gMimicKeyInputs = false;

VehicleDesc initPlayerVehiclePhysicsDesc()
{
	//Set up the chassis mass, dimensions, moment of inertia, and center of mass offset.
	//The moment of inertia is just the moment of inertia of a cuboid but modified for easier steering.
	//Center of mass offset is 0.65m above the base of the chassis and 0.25m towards the front.
	const PxF32 chassisMass = 1400.0f;
	const PxVec3 chassisDims(2.0f, 1.0f, 5.2f);
	const PxVec3 chassisMOI
	((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass / 12.0f);
	const PxVec3 chassisCMOffset(0.0f, -chassisDims.y*0.5f - 0.8f, -0.3f);

	//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
	//Moment of inertia is just the moment of inertia of a cylinder.
	const PxF32 wheelMass = 40.0f;
	const PxF32 wheelRadius = 0.6f;
	const PxF32 wheelWidth = 0.2f;		//This became needed after uneven driving terrain was added. On
	const PxF32 wheelMOI = 0.5f*wheelMass*wheelRadius*wheelRadius*2;
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

VehicleDesc initEnemyVehiclePhysicsDesc()
{
	//Set up the chassis mass, dimensions, moment of inertia, and center of mass offset.
	//The moment of inertia is just the moment of inertia of a cuboid but modified for easier steering.
	//Center of mass offset is 0.65m above the base of the chassis and 0.25m towards the front.
	const PxF32 chassisMass = 1400.0f;
	const PxVec3 chassisDims(2.0f, 1.0f, 5.2f);
	const PxVec3 chassisMOI
	((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass / 12.0f);
	const PxVec3 chassisCMOffset(0.0f, -chassisDims.y*0.5f - 0.8f, -0.3f);

	//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
	//Moment of inertia is just the moment of inertia of a cylinder.
	const PxF32 wheelMass = 40.0f;
	const PxF32 wheelRadius = 0.6f;
	const PxF32 wheelWidth = 0.01f;		//This became needed after uneven driving terrain was added. On
	const PxF32 wheelMOI = 0.5f*wheelMass*wheelRadius*wheelRadius * 2;
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

void startSteerRightMode()
{
	if (gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalSteerRight(true);
	}
	else
	{
		gVehicleInputData.setAnalogSteer(1.0f);
	}
}

void startSteerLeftMode()
{
	if (gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalSteerLeft(true);
	}
	else
	{
		gVehicleInputData.setAnalogSteer(-1.0f);
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
		gVehicleInputData.setAnalogAccel(1.0f);
		gVehicleInputData.setAnalogSteer(-1.0f);
		gVehicleInputData.setAnalogHandbrake(1.0f);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(1.0f);
		gVehicleInputData.setAnalogSteer(-1.0f);
		gVehicleInputData.setAnalogHandbrake(1.0f);
	}
}

void startHandbrakeTurnRightMode()
{
	if (gMimicKeyInputs)
	{
		gVehicleInputData.setAnalogAccel(1.0f);
		gVehicleInputData.setAnalogSteer(1.0f);
		gVehicleInputData.setAnalogHandbrake(1.0f);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(1.0f);
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
	sceneDesc.gravity = PxVec3(0.0f, -9.81f*1.5f, 0.0f);
	PxU32 numWorkers = 1;
	gDispatcher = PxDefaultCpuDispatcherCreate(numWorkers);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.3f, 0.8f);
	//tireMaterial = gPhysics->createMaterial(0.5f, 0.2f, 0.6f);

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

	startBrakeMode();
}

int Physics_Controller::createMap(const PxVec3* verts, const PxU32 numVerts, const PxU32* indices, const PxU32 triCount){

	PxFilterData groundPlaneSimFilterData(COLLISION_FLAG_GROUND, COLLISION_FLAG_GROUND_AGAINST, 0, 0);
	gMap1Ground = createRigidTriangleMeshDrivable(verts, numVerts, indices, triCount, gMaterial, *gPhysics, *gCooking, groundPlaneSimFilterData);
	gScene->addActor(*gMap1Ground);

	rigidStaticActorIndex++;
	return rigidStaticActorIndex;

}

int Physics_Controller::createStaticObject(const PxVec3* verts, const PxU32 numVerts, const PxU32* indices, const PxU32 triCount, float x, float y, float z) {
	gStaticObject= createRigidTriangleMeshStatic(verts, numVerts, indices, triCount, gMaterial, *gPhysics, *gCooking);
	gStaticObject->setGlobalPose({ x, y, z });
	gScene->addActor(*gStaticObject);

	rigidStaticActorIndex++;
	return rigidStaticActorIndex;

}

int Physics_Controller::createDynamicObject(PxU32 objectType, PxVec3 dimensions, PxVec3 MOI, PxReal mass, PxReal density, float x, float y, float z) {
	gDynamicObject = createRigidDynamicObject(objectType, dimensions, MOI, mass, gMaterial, density, *gPhysics, *gCooking);
	gDynamicObject->setGlobalPose({ x, y, z });
	
	gScene->addActor(*gDynamicObject);
	gDynamicObject->wakeUp();

	rigidDynamicActorIndex++;
	return rigidDynamicActorIndex;

}

int Physics_Controller::createEnemyVehicle() {
	//Create a vehicle that will drive on the plane.
	VehicleDesc vehicleDesc = initEnemyVehiclePhysicsDesc();;
	enemyVehicle = createEnemyVehicle4W(vehicleDesc, gPhysics, gCooking);
	PxTransform startTransform(PxVec3(0, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0.0f), PxQuat(PxIdentity));
	enemyVehicle->getRigidDynamicActor()->setGlobalPose(startTransform);
	gScene->addActor(*enemyVehicle->getRigidDynamicActor());

	//Set the vehicle to rest in first gear.
	//Set the vehicle to use auto-gears.
	enemyVehicle->setToRestState();
	enemyVehicle->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
	enemyVehicle->mDriveDynData.setUseAutoGears(true);

	vehiclesVector.push_back(enemyVehicle);

	rigidDynamicActorIndex++;
	return rigidDynamicActorIndex;
}

int Physics_Controller::createPlayerVehicle() {
	//Create a vehicle that will drive on the plane.
	VehicleDesc vehicleDesc = initPlayerVehiclePhysicsDesc();
	gVehicle4W = createPlayerVehicle4W(vehicleDesc, gPhysics, gCooking);
	PxTransform startTransform(PxVec3(0, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0.0f), PxQuat(PxIdentity));

	//Test code to identify vehicles
	PxActor *actor = gVehicle4W->getRigidDynamicActor()->is<PxActor>();

	gVehicle4W->getRigidDynamicActor()->setGlobalPose(startTransform);
	gScene->addActor(*gVehicle4W->getRigidDynamicActor());

	//Set the vehicle to rest in first gear.
	//Set the vehicle to use auto-gears.
	gVehicle4W->setToRestState();
	gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
	gVehicle4W->mDriveDynData.setUseAutoGears(true);

	vehiclesVector.push_back(gVehicle4W);

	rigidDynamicActorIndex++;
	return rigidDynamicActorIndex;
}

void Physics_Controller::setPosition(int actorIndex, glm::vec3 newLocation){
	PxU32 numOfRidg = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
	PxActor *userBuffer[50];

	PxU32 numOfRidgActors = gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, userBuffer, numOfRidg, 0);
	PxActor *actor = userBuffer[actorIndex];
	PxRigidActor *rigidActor = actor->is<PxRigidActor>();
	rigidActor->setGlobalPose({ newLocation.x, newLocation.y, newLocation.z });
}

void Physics_Controller::resetOrientation(int actorIndex) {
	PxU32 numOfRidg = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
	PxActor *userBuffer[50];

	PxU32 numOfRidgActors = gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, userBuffer, numOfRidg, 0);
	PxActor *actor = userBuffer[actorIndex];
	PxRigidActor *rigidActor = actor->is<PxRigidActor>();
	PxTransform orientation = rigidActor->getGlobalPose();	
	PxVec3 location = orientation.p;
	PxQuat rotation = orientation.q;

	PxVec3 xRotation = rotation.getBasisVector0();
	PxVec3 yRotation = rotation.getBasisVector1();
	PxVec3 zRotation = rotation.getBasisVector2();


	//int gsi = gameState->lookupGSIUsingPI(actorIndex);
	
	glm::vec3 heading = gameState->playerVehicle.direction;

	float angle = atan2(heading.x, heading.z); // Note: I expected atan2(z,x) but OP reported success with atan2(x,z) instead! Switch around if you see 90?off.
	float qx = 0;
	float qy = 1 * sin(angle / 2);
	float qz = 0;
	float qw = cos(angle / 2);

	PxQuat relativeQuatReset = PxQuat(qx, qy, qz, qw);

	PxTransform resetTransform = PxTransform(location, relativeQuatReset);
	rigidActor->setGlobalPose(resetTransform);
	std::cout << "orientation reset" << std::endl;
}

void Physics_Controller::userDriveInput(bool WKey, bool AKey, bool SKey, bool DKey, bool SPACEKey, bool hello, float leftStickX, float leftTrigger, float rightTrigger) {
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

	if (gameState->controller == false) {
		if ((WKey) && !(SKey))/*Check if high-order bit is set (1 << 15)*/
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

		else if ((SKey) && !(WKey))/*Check if high-order bit is set (1 << 15)*/
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

		if (!(WKey) && !(SKey))/*Check if high-order bit is set (1 << 15)*/
		{

			if ((AKey) && !(DKey))
			{
				steerDirection = "left";
				startSteerRightMode();
			}
			else if ((DKey) && !(AKey))
			{
				steerDirection = "right";
				startSteerLeftMode();
			}
		}

		if ((WKey) && (SKey)) {
			startBrakeMode();

		}
	}

	//Gamepad Driving Input
	else {
		if (rightTrigger > -1) {
			if (currentGear < 0) {
				currentGear = 1;
				changeToForwardGear = true;
			}
			gVehicleInputData.setAnalogAccel((rightTrigger + 1)/2);
		}
		else if (leftTrigger > -1) {
			if (currentGear > 0) {
				currentGear = -1;
				changeToReverseGear = true;
			}
			gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eREVERSE);
			gVehicleInputData.setAnalogAccel((leftTrigger + 1)/2);
		}

		gVehicleInputData.setAnalogSteer(-leftStickX);

		if (gameState->button == "B") {
			gVehicleInputData.setAnalogHandbrake(1.0f);
		}
		else {
			     gVehicleInputData.setAnalogHandbrake(0.0f);
		}
	}

	//If the mode about to start is eDRIVE_MODE_ACCEL_REVERSE then switch to reverse gears.
	if (changeToReverseGear)
	{
		changeToReverseGear = false;
		gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
	}

}

void Physics_Controller::stepPhysics(bool interactive)
{
	//Get set of rigid dynamic actors
	PxU32 numOfRidg = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
	PxActor *userBufferRD[50];
	PxU32 numOfRidgDynamicActors = gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, userBufferRD, numOfRidg, 0);

	//Get set of rigid static actors
	PxU32 numOfStat = gScene->getNbActors(PxActorTypeFlag::eRIGID_STATIC);
	PxActor *userBufferRS[50];
	PxU32 numOfRidgStaticActors = gScene->getActors(PxActorTypeFlag::eRIGID_STATIC, userBufferRS, numOfStat, 0);


	PX_UNUSED(interactive);
	const PxF32 timestep = 1.0f / 60.0f;

	//Update the control inputs for the vehicle.
	userDriveInput(gameState->WKey, gameState->AKey, gameState->SKey, gameState->DKey, gameState->SPACEKey, true, gameState->leftStickX, gameState->leftTrigger, gameState->rightTrigger);
	

	//Update each vehicles drive direction based on input values
	for (int i = 0; i < (int)vehiclesVector.size(); i++) {
		
		PxActor *actor = vehiclesVector[i]->getRigidDynamicActor()->is<PxActor>();
		
		int gameStateIndex = -1;


		for (int index = 0; index <= rigidDynamicActorIndex; index++) {
			if (actor == userBufferRD[index]) {
				gameStateIndex = gameState->lookupGSIUsingPI(index);
				break;
			}
		}
		
		if (gameStateIndex != -1) {		//If this is an AI, get its pathfinding computed input
			glm::vec2 pathfindingInput = gameState->pathfindingInputs[gameStateIndex];

			//std::cout << "Enemy active: " << gameState->Enemies[gameStateIndex].getActive() << std::endl;

			if (gameState->Enemies[gameStateIndex].getActive() == 1) {	//If the vehicle should move  

				if (gameState->Enemies[gameStateIndex].CheckForStuck()) {		//If vehicle is stuck, go in reverse
					vehiclesVector[i]->mDriveDynData.forceGearChange(PxVehicleGearsData::eREVERSE);

					enemyInputData.setAnalogAccel(pathfindingInput[0]);			
					enemyInputData.setAnalogSteer(-pathfindingInput[1]);

					if (gameState->Enemies[gameStateIndex].forceRelocate) {		//If vehicle has finished trying to get unstuck and is still not moving, relocate
						EnemyUnit enemy = gameState->Enemies[gameStateIndex];
						setPosition(enemy.physicsIndex, glm::vec3(enemy.position.x, enemy.position.y, enemy.position.z) + glm::vec3(0.2f, 0.0f, 0.0f));
					}
				}

				else {															//Else drive as normal
					vehiclesVector[i]->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
					vehiclesVector[i]->mDriveDynData.setUseAutoGears(true);
					enemyInputData.setAnalogAccel(pathfindingInput[0]);			
					enemyInputData.setAnalogSteer(pathfindingInput[1]);
				}

				PxVehicleDrive4WSmoothAnalogRawInputsAndSetAnalogInputs(gPadSmoothingData, enemySteerVsForwardSpeedTable, enemyInputData, timestep, gIsVehicleInAir, *vehiclesVector[i]);
			}
			else {
				enemyInputData.setAnalogAccel(0.0f);
				enemyInputData.setAnalogSteer(0.0f);
				PxVehicleDrive4WSmoothAnalogRawInputsAndSetAnalogInputs(gPadSmoothingData, enemySteerVsForwardSpeedTable, enemyInputData, timestep, gIsVehicleInAir, *vehiclesVector[i]);
			}
		}
		else {							//If this is the player, record as normal
			PxVehicleDrive4WSmoothAnalogRawInputsAndSetAnalogInputs(gPadSmoothingData, playerSteerVsForwardSpeedTable, gVehicleInputData, timestep, gIsVehicleInAir, *vehiclesVector[i]);
		}
		

		//Raycasts.
		PxVehicleWheels* vehicles[1] = { vehiclesVector[i] };
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
	}



	//Scene update.
	gContactReportCallback.gContactPositions.clear();
	gScene->simulate(timestep);
	gScene->fetchResults(true);



	//Check collisions for Vehicle/Vehicle collisions
	for (int i = 0; i < (int)gContactReportCallback.gContactActor1s.size(); i++) {
		Vehicle* vehicle1 = NULL;
		Vehicle* vehicle2 = NULL;
		//std::cout << "gContactReportCallback contact: " << i << "/" << gContactReportCallback.gContactActor1s.size() << std::endl;


		for (int index = 0; index <= rigidDynamicActorIndex; index++) {
			PxActor *actor = userBufferRD[index];
			if (gContactReportCallback.gContactActor1s[i] == actor) {
				vehicle1 = gameState->lookupVUsingPI(index);
			}
			if (gContactReportCallback.gContactActor2s[i] == actor) {
				vehicle2 = gameState->lookupVUsingPI(index);
			}
		}
		
		if (vehicle1 != NULL && vehicle2 != NULL && (gContactReportCallback.gContactImpulses[i] != PxVec3{ 0.0f,0.0f,0.0f })) {
			//std::cout << "Found 2 vehicles, Contact Impule Vector length is : " << gContactReportCallback.gContactImpulses.size() << std::endl;
			glm::vec3 impulse = (glm::vec3{ gContactReportCallback.gContactImpulses[i].x, gContactReportCallback.gContactImpulses[i].y, gContactReportCallback.gContactImpulses[i].z });
			gameState->Collision(vehicle1, vehicle2, impulse);
		}
	}
	
	if (powerupGrabbed == false) {
		//std::cout << "Starting Powerup check" << std::endl;
		//Check collisions for Player/PowerUp collisions
		for (int i = 0; i < (int)gContactReportCallback.gContactActor1s.size(); i++) {
			//std::cout << "..." << std::endl;
			Vehicle* vehicle1 = NULL;
			PowerUp* powerUp = NULL;

			for (int index = 0; index <= rigidDynamicActorIndex; index++) {
				PxActor *actor = userBufferRD[index];

				if (index == gameState->playerVehicle.physicsIndex) {
					//std::cout << "Checking if either actor is the player" << std::endl;
					if ((gContactReportCallback.gContactActor1s[i] == actor || gContactReportCallback.gContactActor2s[i] == actor) && vehicle1 == NULL) {
						vehicle1 = gameState->lookupVUsingPI(index);
						//std::cout << "Player was involved with contact!!!!!!!!!!!!!!!!!!!" << std::endl;
					}
				}

				//std::cout << "Checking rigidDynamicActorIndex of: " << index << std::endl;
				if ((gContactReportCallback.gContactActor1s[i] == actor || gContactReportCallback.gContactActor2s[i] == actor) && powerUp == NULL) {
					powerUp = gameState->lookupPUUsingPI(index);
					//std::cout << "Found powerup" << std::endl;
				}
			}

			if (vehicle1 != NULL && powerUp != NULL) {
				std::cout << "Powerup activating" << std::endl;
				gameState->Collision(vehicle1, powerUp);
				powerupGrabbed = true;
				break;
			}
		}
	}
	else {
		powerupGrabbed = false;
	}
	
	
	//Check collisions for Player/Static Object collisions
	for (int i = 0; i < (int)gContactReportCallback.gContactActor1s.size(); i++) {
		Vehicle* vehicle1 = NULL;
		Object* object = NULL;

		//Try to find player vehicle
		for (int index = 0; index <= rigidDynamicActorIndex; index++) {
			PxActor *actor = userBufferRD[index];
			if (index == gameState->playerVehicle.physicsIndex) {				
				if (gContactReportCallback.gContactActor1s[i] == actor || gContactReportCallback.gContactActor2s[i] == actor) {
					vehicle1 = gameState->lookupVUsingPI(index);
				}
			}
		}

		//Try to find static object
		for (int index = 0; index <= rigidStaticActorIndex; index++) {
			PxActor *actor = userBufferRS[index];
			//printf("%d\n", index);
			if (index != gameState->mapGroundPhysicsIndex+1) {
				if (gContactReportCallback.gContactActor1s[i] == actor || gContactReportCallback.gContactActor2s[i] == actor) {
					object = new Object();	//Since it does not matter at this point, object refrence is not accurate
					//std::cout << "Collision with object with index: " << index << std::endl;
				}
			}
		}

		if (vehicle1 != NULL && object != NULL) {
			gameState->Collision(vehicle1, object);
		}
	}






	//Clear contact report
	gContactReportCallback.gContactActor1s.clear();
	gContactReportCallback.gContactActor2s.clear();
	gContactReportCallback.gContactPositions.clear();
	gContactReportCallback.gContactImpulses.clear();

	//Update each vehicle's location, transformation matrix, etc.
	updateEntities();
}




void Physics_Controller::updateEntities() {
	PxU32 numOfRidg = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
	
	PxActor *userBuffer[50];
	//std::cout << "Number of Ridged objects: " << numOfRidg << std::endl; //Test statement, delete it if you want

	PxU32 numOfRidgActors = gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, userBuffer, numOfRidg, 0);
	//std::cout << "Number of Ridged objects: " << numOfRidgActors << std::endl; //Test statement, delete it if you want

	for (int index = 0; index <= rigidDynamicActorIndex; index++) {
		PxActor *actor = userBuffer[index];

		PxRigidActor *rigidActor = actor->is<PxRigidActor>();
		PxRigidBody *rigidBody = rigidActor->is<PxRigidBody>();
		PxVec3 velocity = rigidBody->getLinearVelocity();
		float speed = glm::length(glm::vec3{ velocity.x, velocity.y, velocity.z });
		if (gameState->playerVehicle.physicsIndex == index && std::abs(velocity.x) > .005f && std::abs(velocity.z) > .005f) {
			gameState->playerVehicle.heading = glm::normalize(glm::vec3{ velocity.x, velocity.y, velocity.z });
		}


		PxTransform orientation = rigidActor->getGlobalPose();		//   https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/apireference/files/classPxRigidActor.html
		PxVec3 location = orientation.p;							//	https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/apireference/files/classPxTransform.html
		PxQuat rotation = orientation.q;

		PxVec3 xRotation = rotation.getBasisVector0();
		PxVec3 yRotation = rotation.getBasisVector1();
		PxVec3 zRotation = rotation.getBasisVector2();

		glm::mat4 transformationMatrix;
		transformationMatrix[0] = { xRotation.x, xRotation.y, xRotation.z, 0.0f };
		transformationMatrix[1] = { yRotation.x, yRotation.y, yRotation.z, 0.0f };
		transformationMatrix[2] = { zRotation.x, zRotation.y, zRotation.z, 0.0f };
		transformationMatrix[3] = { location.x , location.y , location.z , 1.0f };

		gameState->updateEntity(index, glm::vec3{ location.x, location.y, location.z }, transformationMatrix, speed);
	}
}





void Physics_Controller::cleanupPhysics(bool interactive)
{
	/*
	//This cleanup call is triggering errors as the objects appear to have already been removed

	PX_UNUSED(interactive);

	gVehicle4W->getRigidDynamicActor()->release();
	gVehicle4W->free();

	enemyVehicle->getRigidDynamicActor()->release();
	enemyVehicle->free();

	//gMap1Ground->release();
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

	//printf("SnippetVehicle4W done.\n");*/
}