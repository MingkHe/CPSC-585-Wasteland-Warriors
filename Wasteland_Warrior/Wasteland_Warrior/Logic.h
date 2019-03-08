#include "Gamestate.h"

class Logic
{
private:
	int enemiesLeft;
	int waveBreak;
	int breakTime;
public:
	Logic();
	~Logic();

	void Update(Gamestate* gameState);
	void modeSelection(Gamestate *gameState, int wave);
	void mode1(Gamestate *gameState, int wave);
	void mode2(Gamestate *gameState, int wave);
	void mode3(Gamestate *gameState, int wave);
};

