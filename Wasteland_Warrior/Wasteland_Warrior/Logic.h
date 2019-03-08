#include "Gamestate.h"

class Logic
{
private:
	int waveBreak;
	int breakTime;
	int enemiesLeft;

public:
	Logic();
	~Logic();

	void Update(Gamestate* gameState);
	int checkEnemyHealth(Gamestate *gameState);
	void spawnPowerUps(Gamestate *gameState);
	void modeSelection(Gamestate *gameState);
	void mode1(Gamestate *gameState);
	void mode2(Gamestate *gameState);
	void mode3(Gamestate *gameState);

};

