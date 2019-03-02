#pragma once
#include "PxContactModifyCallback.h"

class MyContactModification : public PxContactModifyCallback
{
public:
		void onContactModify(PxContactModifyPair* const pairs, PxU32 count);
};
