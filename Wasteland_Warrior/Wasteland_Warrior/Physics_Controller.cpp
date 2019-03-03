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

PxVehicleDrive4W*		enemyVehicle = NULL;

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
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_CONTACT_POINTS;

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;

	/*
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	// all initial and persisting reports for everything, with per-point data
	pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT
		| PxPairFlag::eNOTIFY_TOUCH_FOUND
		| PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| PxPairFlag::eNOTIFY_CONTACT_POINTS;
	return PxFilterFlag::eDEFAULT;*/
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

PxF32 gSteerVsForwardSpeedData[2*8]=
{
	0.0f,		0.2f,
	5.0f,		0.2f,
	30.0f,		0.125f,
	120.0f,		0.1f,
	PX_MAX_F32, PX_MAX_F32,
	PX_MAX_F32, PX_MAX_F32,
	PX_MAX_F32, PX_MAX_F32,
	PX_MAX_F32, PX_MAX_F32
};


// Casting values to PxF32 to resolve warning of mismatch types
/*PxF32 gSteerVsForwardSpeedData[2 * 8] =
{
	(PxF32)0.1*PX_MAX_F32,		0.5f,
	(PxF32)0.2*PX_MAX_F32,		0.45f,
	(PxF32)0.3*PX_MAX_F32,		0.4f,
	(PxF32)0.4*PX_MAX_F32,		0.35f,
	(PxF32)0.5*PX_MAX_F32,		0.3f,
	(PxF32)0.6*PX_MAX_F32,		0.25f,
	(PxF32)0.7*PX_MAX_F32,		0.2f,
	(PxF32)0.8*PX_MAX_F32,		0.1f
};*/


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
PxVehicleDrive4WRawInputData enemyInputData;

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

VehicleDesc initEnemyVehiclePhysicsDesc()
{
	//Set up the chassis mass, dimensions, moment of inertia, and center of mass offset.
	//The moment of inertia is just the moment of inertia of a cuboid but modified for easier steering.
	//Center of mass offset is 0.65m above the base of the chassis and 0.25m towards the front.
	const PxF32 chassisMass = 1500.0f;
	const PxVec3 chassisDims(3.0f, 2.0f, 5.0f);
	const PxVec3 chassisMOI
	((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.6f*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass / 12.0f);
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

	startBrakeMode();
}

int Physics_Controller::createVehicle() {
	//Create a vehicle that will drive on the plane.
	VehicleDesc vehicleDesc = initPlayerVehiclePhysicsDesc();;
	gVehicle4W = createEnemyVehicle4W(vehicleDesc, gPhysics, gCooking);
	PxTransform startTransform(PxVec3(0, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0.0f), PxQuat(PxIdentity));
	gVehicle4W->getRigidDynamicActor()->setGlobalPose(startTransform);
	gScene->addActor(*gVehicle4W->getRigidDynamicActor());

	//Set the vehicle to rest in first gear.
	//Set the vehicle to use auto-gears.
	gVehicle4W->setToRestState();
	gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
	gVehicle4W->mDriveDynData.setUseAutoGears(true);

	rigidDynamicActorIndex++;
	return rigidDynamicActorIndex;
}

int Physics_Controller::createEnemyVehicle() {
	//Create a vehicle that will drive on the plane.
	VehicleDesc vehicleDesc = initPlayerVehiclePhysicsDesc();;
	enemyVehicle = createEnemyVehicle4W(vehicleDesc, gPhysics, gCooking);
	PxTransform startTransform(PxVec3(0, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0.0f), PxQuat(PxIdentity));
	enemyVehicle->getRigidDynamicActor()->setGlobalPose(startTransform);
	gScene->addActor(*enemyVehicle->getRigidDynamicActor());

	//Set the vehicle to rest in first gear.
	//Set the vehicle to use auto-gears.
	enemyVehicle->setToRestState();
	enemyVehicle->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
	enemyVehicle->mDriveDynData.setUseAutoGears(true);

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

	rigidDynamicActorIndex++;
	return rigidDynamicActorIndex;
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

void Physics_Controller::userDriveInput(bool WKey, bool AKey, bool SKey, bool DKey, bool SPACEKey, bool hello) {
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

	if (!(WKey) && !(SKey) && !(SPACEKey))/*Check if high-order bit is set (1 << 15)*/
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

	userDriveInput(gameState->WKey, gameState->AKey, gameState->SKey, gameState->DKey, gameState->SPACEKey, true);
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
	gContactReportCallback.gContactPositions.clear();
	gScene->simulate(timestep);
	gScene->fetchResults(true);



	/*
	//This is how to get impulse data, this will be deleted later but I am using it as a refrence
	//printf("%d contact points\n", PxU32(gContactReportCallback.gContactPositions.size()));
	if (gContactReportCallback.gContactImpulses.size() >= 1) {
		printf("Calucalting contact vector\n");
		glm::vec2 impulse = glm::normalize(glm::vec2{ gContactReportCallback.gContactImpulses[0].x, gContactReportCallback.gContactImpulses[0].z });
		std::cout << "Contact vector:  [" << impulse.x << "," << impulse.y << "]" << std::endl;
	}

	if (gContactReportCallback.gContactImpulses.size() >= 1) {
		printf("Calucalting contact vector\n");
		glm::vec2 contactPoint = glm::normalize(glm::vec2{ gContactReportCallback.gContactPositions[0].x, gContactReportCallback.gContactPositions[0].z });
		std::cout << "Contact point:  [" << contactPoint.x << "," << contactPoint.y << "]" << std::endl;
	}*/


	//Efficency of this could be improved but not critical
	if (gContactReportCallback.gContactActor1s.size() >= 1) {
		PxU32 numOfRidg = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
		PxActor *userBuffer[50];
		PxU32 numOfRidgActors = gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, userBuffer, numOfRidg, 0);

		Vehicle* vehicle1 = new Vehicle();
		Vehicle* vehicle2 = new Vehicle();

		for (int index = 0; index <= rigidDynamicActorIndex; index++) {
			PxActor *actor = userBuffer[index];
			if (gContactReportCallback.gContactActor1s[0] == actor) {
				vehicle1 = gameState->lookupVUsingPI(index);
			}
			if (gContactReportCallback.gContactActor2s[0] == actor) {
				vehicle2 = gameState->lookupVUsingPI(index);
			}
		}

		glm::vec2 impulse = (glm::vec2{ gContactReportCallback.gContactImpulses[0].x, gContactReportCallback.gContactImpulses[0].z });
		gameState->Collision(vehicle1, vehicle2, impulse);
	}


	//Clear contact report
	gContactReportCallback.gContactActor1s.clear();
	gContactReportCallback.gContactActor2s.clear();
	gContactReportCallback.gContactPositions.clear();
	gContactReportCallback.gContactImpulses.clear();

	updateEntities();
}




void Physics_Controller::updateEntities() {
	PxU32 numOfRidg = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
	
	PxActor *userBuffer[50];
	//std::cout << "Number of Ridged objects: " << numOfRidg << std::endl; //Test statement, delete it if you want

	PxU32 numOfRidgActors = gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, userBuffer, numOfRidg, 0);

	for (int index = 0; index <= rigidDynamicActorIndex; index++) {
		PxActor *actor = userBuffer[index];

		PxRigidActor *rigidActor = actor->is<PxRigidActor>();
		PxRigidBody *rigidBody = rigidActor->is<PxRigidBody>();
		PxVec3 velocity = rigidBody->getLinearVelocity();
		float speed = glm::length(glm::vec2{ velocity.x, velocity.z });
		//PxRigidDynamicActor *rigidActor = actor->is<PxRigidActor>();
		//rigidActor->setAnalogAccel(1.0f);

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
	PX_UNUSED(interactive);

	gVehicle4W->getRigidDynamicActor()->release();
	gVehicle4W->free();

	enemyVehicle->getRigidDynamicActor()->release();
	enemyVehicle->free();

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

	//printf("SnippetVehicle4W done.\n");
}