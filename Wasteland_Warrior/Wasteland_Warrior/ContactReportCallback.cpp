#include "ContactReportCallback.h"
#include <vector>
#include "Gamestate.h"

#include "PxPhysicsAPI.h"
#include <iostream>





ContactReportCallback::ContactReportCallback() {}

ContactReportCallback::ContactReportCallback(Gamestate* newGamestate) {
	gameState = newGamestate;
}

void ContactReportCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {
	PX_UNUSED(constraints); PX_UNUSED(count);
}

void ContactReportCallback::onWake(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
void ContactReportCallback::onSleep(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
void ContactReportCallback::onTrigger(PxTriggerPair* pairs, PxU32 count) { PX_UNUSED(pairs); PX_UNUSED(count); }
void ContactReportCallback::onAdvance(const PxRigidBody*const*, const PxTransform*, const PxU32) {}
void ContactReportCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	PX_UNUSED((pairHeader));
	std::vector<PxContactPairPoint> contactPoints;

	for (PxU32 i = 0; i < nbPairs; i++)
	{
		gameState->testFunction(0);

		PxActor* actor1 = pairHeader.actors[0]->is<PxActor>();
		PxActor* actor2 = pairHeader.actors[1]->is<PxActor>();

		const char* name0 = "BLA";
		const char* name1 = actor1->getName();
		const char* name2 = actor2->getName();


		/* This code will be used later but after the recent merge causes a crash (non-critical for the moment)
		if (strcmp(name1, name0) == 0 || strcmp(name2, name0) == 0)
		{
			gameState->testFunction(1);
		}*/



		PxU32 contactCount = pairs[i].contactCount;
		if (contactCount)
		{
			contactPoints.resize(contactCount);
			pairs[i].extractContacts(&contactPoints[0], contactCount);

			for (PxU32 j = 0; j < contactCount; j++)
			{
				gContactPositions.push_back(contactPoints[j].position);
				gContactImpulses.push_back(contactPoints[j].impulse);
			}
		}
	}
}