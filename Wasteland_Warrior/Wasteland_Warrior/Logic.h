#include "Gamestate.h"

class Logic
{
private:
	int waveBreak = 0;
	int breakTime = 30 * 60; // 30 seconds times framerate
public:
	Logic();
	~Logic();

	void Update(Gamestate* gameState);
};

