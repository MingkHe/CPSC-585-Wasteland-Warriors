#include "Gamestate.h"

class AI_Interaction
{
public:
	AI_Interaction();
	AI_Interaction(Gamestate* newGameState);
	~AI_Interaction();

	int Update();
	Gamestate* gameState;
};