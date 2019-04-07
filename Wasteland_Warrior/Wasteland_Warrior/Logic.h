#include "Gamestate.h"

class Logic
{
private:
	int waveBreak;
	int breakTime;
	int enemiesLeft;
	int score;
	bool payloadcollected;

public:
	Logic();
	~Logic();

	void Update(Gamestate* gameState);
	
	//Game Logic
	bool waveFinished(Gamestate *gameState);
	int checkEnemyHealth(Gamestate *gameState);
	bool checkpointsRemaining(Gamestate *gameState);
	bool huntedEnemiesRemaining(Gamestate *gameState);
	bool payloadCollected(Gamestate *gameState);
	void spawnPowerUps(Gamestate *gameState);
	
	//Game Modes
	void modeSelection(Gamestate *gameState);
	
	void survival(Gamestate *gameState);
	void checkpoint(Gamestate *gameState);
	void payload(Gamestate *gameState);
	void headHunter(Gamestate *gameState);
	void bossBattle(Gamestate *gameState);

};

