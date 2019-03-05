#pragma once
#include <glm/glm.hpp>
#include "ContactReportCallback.h"
class Gamestate;

class Physics_Controller
{
private:

public:
	Physics_Controller(Gamestate* newGameState);
	~Physics_Controller();

	void initPhysics(bool interactive);
	void stepPhysics(bool interactive);
	void cleanupPhysics(bool interactive);

	void Update();
	void updateEntities();
	void userDriveInput(bool WKey, bool AKey, bool SKey, bool DKey, bool SPACEKey, bool hello, float leftStickX, float leftTrigger, float rightTrigger);

	void setPosition(int actorIndex, glm::vec3 newLocation);
	int createMap(const PxVec3* verts, const PxU32 numVerts, const PxU32* indices, const PxU32 triCount);
	int createStaticObject(const PxVec3* verts, const PxU32 numVerts, const PxU32* indices, const PxU32 triCount, float x, float y, float z);
	int createDynamicObject(PxU32 objectType, PxVec3 dimensions, PxVec3 MOI, PxReal mass, PxReal density, float x, float y, float z);

	void resetOrientation(int actorIndex);
	int createEnemyVehicle();
	int createPlayerVehicle();

	ContactReportCallback gContactReportCallback;


	int rigidDynamicActorIndex = -1;
	int rigidStaticActorIndex = -1;
	Gamestate* gameState;
};
