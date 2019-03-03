#pragma once
#include "PxSimulationEventCallback.h"
#include <vector>

class Gamestate;


using namespace physx;

class ContactReportCallback: public PxSimulationEventCallback
{
private:

public:
	ContactReportCallback();
	ContactReportCallback(Gamestate* newGamestate);
	Gamestate* gameState;

	std::vector<PxVec3> gContactPositions;
	std::vector<PxVec3> gContactImpulses;

	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count);
	void onWake(PxActor** actors, PxU32 count);
	void onSleep(PxActor** actors, PxU32 count);
	void onTrigger(PxTriggerPair* pairs, PxU32 count);
	void onAdvance(const PxRigidBody*const*, const PxTransform*, const PxU32);
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
};
