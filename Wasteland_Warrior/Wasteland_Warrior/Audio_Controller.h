#include "Gamestate.h"
#include "MusicPool.h"
#include <SDL_mixer.h>
#include <SDL.h>

class Audio_Controller
{
private:
	Mix_Music* bgm;
	MusicPool* myMusics;

public:
	Audio_Controller();
	~Audio_Controller();

	int playSound(Gamestate gameState);

	void playMusic();
	void pauseMusic();
	void resumeMusic();
	void haltMusic();
};
