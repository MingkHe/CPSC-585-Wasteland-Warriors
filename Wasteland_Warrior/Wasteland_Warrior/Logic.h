#include "Gamestate.h"

class Logic
{
private:
	int waveBreak;
	int breakTime;
	int enemiesLeft;
	int score;
	bool payloadcollected;
	int collected;

public:
	Logic();
	~Logic();

	void Update(Gamestate* gameState);
	
	//Game Logic
	bool waveFinished(Gamestate *gameState);
	int checkEnemyHealth(Gamestate *gameState);

	bool checkpointsCollected(Gamestate *gameState);
	bool huntedEnemiesRemaining(Gamestate *gameState);
	bool bossRemaining(Gamestate *gameState);
	bool payloadCollected(Gamestate *gameState);
	bool endgame(Gamestate *gameState);
	
	//Game Modes
	void modeSelection(Gamestate *gameState);
	
	//Spawning
	void enemies(Gamestate *gameState);
	void powerUps(Gamestate *gameState);
	void checkpoint(Gamestate *gameState);
	void payload(Gamestate *gameState);
	void headHunter(Gamestate *gameState);
	void boss(Gamestate *gameState);

};

