#include "ContactReportCallback.h"
#include <vector>

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