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
	int createMap(const PxVec3* verts, const PxU32 numVerts, const PxVec3* indices, const PxU32 triCount);
	int createVehicle();
	int createEnemyVehicle();
	int createPlayerVehicle();

	ContactReportCallback gContactReportCallback;

	int rigidDynamicActorIndex = -1;
	int rigidStaticActorIndex = -1;
	Gamestate* gameState;


};
