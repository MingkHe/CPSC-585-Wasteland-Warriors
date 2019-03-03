#include "Gamestate.h"
#include "MusicPool.h"
#include "SFXPool.h"
#include <SDL_mixer.h>
#include <SDL.h>

using namespace std;

class Audio_Controller
{
private:
	Mix_Music* bgm;
	Mix_Chunk* ui_click;
	Mix_Chunk* ui_enter;
	Mix_Chunk* car_start;
	Mix_Chunk* car_idle;
	Mix_Chunk* car_run;
	Mix_Chunk* car_brake;
	Mix_Chunk* car_crash;
	Mix_Chunk* car_expo;
	std::string currentMusic;
	
public:
	Audio_Controller();
	~Audio_Controller();

	int playSound(Gamestate* gameState);
	SFXPool* mySFX;
	MusicPool* myMusics;
	void playMusic();
	void pauseMusic();
	void resumeMusic();
	void haltMusic();
	void switchMusic();

	static Audio_Controller* myInstance;
	static Audio_Controller* instance();
};
