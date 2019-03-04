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

		std::cout << "----------------Collision---------------" << std::endl;

		PxActor* actor1 = pairHeader.actors[0]->is<PxActor>();
		PxActor* actor2 = pairHeader.actors[1]->is<PxActor>();

		gContactActor1s.push_back(actor1);
		gContactActor2s.push_back(actor2);



		PxU32 contactCount = pairs[i].contactCount;
		//std::cout << "Contact Point Count: " << contactCount << std::endl;



		if (contactCount)
		{
			contactPoints.resize(contactCount);
			pairs[i].extractContacts(&contactPoints[0], contactCount);

			for (PxU32 j = 0; j < 1; j++)
			{
				gContactPositions.push_back(contactPoints[j].position);
				gContactImpulses.push_back(contactPoints[j].impulse);
			}
		}
	}
}
