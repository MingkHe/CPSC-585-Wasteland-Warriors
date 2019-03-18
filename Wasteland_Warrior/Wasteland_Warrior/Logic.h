#include "Gamestate.h"

class Logic
{
private:
	int waveBreak;
	int breakTime;
	int enemiesLeft;
	int score;

public:
	Logic();
	~Logic();

	void Update(Gamestate* gameState);
	int checkEnemyHealth(Gamestate *gameState);
	void spawnPowerUps(Gamestate *gameState);
	void modeSelection(Gamestate *gameState);
	void survival(Gamestate *gameState);
	void checkpoint(Gamestate *gameState);
	void payload(Gamestate *gameState);
	void headHunter(Gamestate *gameState);
	void bossBattle(Gamestate *gameState);

};

