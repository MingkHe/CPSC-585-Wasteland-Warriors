#include "Gamestate.h"

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

	void setPosition(int actorIndex, glm::vec3 newLocation);
	int createVehicle();

	int rigidDynamicActorNumber = 0;
	Gamestate* gameState;
};
