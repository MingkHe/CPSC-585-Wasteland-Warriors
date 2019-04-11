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
	Mix_Music* bgm_pause;
	Mix_Music* bgm_win;
	Mix_Music* bgm_lose;
	Mix_Music* bgm_gamePlay;
	Mix_Chunk* ui_click;
	Mix_Chunk* ui_enter;
	Mix_Chunk* car_start;
	Mix_Chunk* car_idle;
	Mix_Chunk* car_run;
	Mix_Chunk* car_brake;
	Mix_Chunk* car_crash;
	Mix_Chunk* car_expo;
	Mix_Chunk* car_crash_static;
	Mix_Chunk* car_powerUp;
	Mix_Chunk* weapon_machine_gun;
	Mix_Chunk* weapon_cannon;
	Mix_Chunk* weapon_metal_hit;
	Mix_Chunk* weapon_empty;
	Mix_Chunk* weapon_bullet_drop;
	Mix_Chunk* weapon_swap;
	Mix_Chunk* weapon_reload;
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
