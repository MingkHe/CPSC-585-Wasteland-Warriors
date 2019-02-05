#include "Gamestate.h"
#include <SDL_mixer.h>
#include <SDL.h>

class Audio_Controller
{
private:

public:
	Audio_Controller();
	~Audio_Controller();

	int playSound(Gamestate gameState);
};
