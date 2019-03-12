#include "Audio_Controller.h"

Audio_Controller* Audio_Controller::myInstance = NULL;

Audio_Controller* Audio_Controller::instance() 
{
	if (myInstance == NULL)
	{
		myInstance = new Audio_Controller();
	}

	return myInstance;
}

Audio_Controller::Audio_Controller()
{
	//testing audio
	SDL_Init(SDL_INIT_AUDIO);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		printf("Mixer initialization error: %s\n",Mix_GetError());

	myMusics = new MusicPool();
	mySFX = new SFXPool();
	currentMusic = "bgm.mp3";

	
	car_start = Mix_LoadWAV("SoundEffect/car_ignition.wav");
	car_idle = Mix_LoadWAV("SoundEffect/car_idle.wav");
	car_run = Mix_LoadWAV("SoundEffect/car_run.wav");
	car_brake = Mix_LoadWAV("SoundEffect/car_brake.wav");
	car_crash = Mix_LoadWAV("SoundEffect/car_crash.wav");
	car_expo = Mix_LoadWAV("SoundEffect/car_explosion.mp3");
	car_crash_static = Mix_LoadWAV("SoundEffect/car_crash_static.wav");
	car_powerUp = Mix_LoadWAV("SoundEffect/car_powerUp.wav");

	ui_click = Mix_LoadWAV("SoundEffect/click.wav");
	ui_enter = Mix_LoadWAV("SoundEffect/selected.mp3");

	bgm = Mix_LoadMUS("Music/bgm2.mp3");
	bgm_pause = Mix_LoadMUS("Music/pause.mp3");
	bgm_win = Mix_LoadMUS("Music/win.mp3");
	bgm_lose = Mix_LoadMUS("Music/lose.mp3");
	bgm_gamePlay = Mix_LoadMUS("Music/gamePlay.mp3");
}


Audio_Controller::~Audio_Controller()
{
	Mix_CloseAudio();
	Mix_Quit();
}

int Audio_Controller::playSound(Gamestate* gameState)
{

	//Implementation
	std::string input = gameState->button;

	if (gameState->carStart_sound && gameState->ui_gameplay)
	{
		Mix_Volume(0, MIX_MAX_VOLUME);
		if (!Mix_Playing(0)) {
			Mix_PlayChannel(0, car_start, 0);
		}
		gameState->carStart_sound = false;
	}

	if (gameState->carIdle_sound && gameState->ui_gameplay)
	{
		if (!Mix_Playing(1)) {
			Mix_Volume(1, MIX_MAX_VOLUME);
			Mix_PlayChannel(1, car_idle, -1);
		}	
	}
	else {
		if (Mix_Playing(1)) {
			Mix_HaltChannel(1);
		}
	}

	if (gameState->carRunning_sound && gameState->ui_gameplay)
	{
		//printf("speed volumn: %f\n", 20+MIX_MAX_VOLUME*gameState->playerVehicle.speed/10.0f);
		Mix_Volume(2, MIX_MAX_VOLUME*gameState->playerVehicle.speed/10.0f);
		if (!Mix_Playing(2)) {	
			Mix_PlayChannel(2, car_run, -1);
		}	
	}
	else {
		if (Mix_Playing(2)) {
			Mix_HaltChannel(2);
		}
	}

	if (gameState->carBrake_sound && gameState->ui_gameplay)
	{
		if (!Mix_Playing(3)) {
			Mix_Volume(3, MIX_MAX_VOLUME/3);
			Mix_PlayChannel(3, car_brake, -1);
		}
	}
	else {
		if (Mix_Playing(3)) {
			Mix_HaltChannel(3);
		}
	}

	if (gameState->carCrash_sound && gameState->ui_gameplay)
	{
		Mix_Volume(4, MIX_MAX_VOLUME);
		if (!Mix_Playing(4)) {
			Mix_PlayChannel(4, car_crash, 0);
		}		
		gameState->carCrash_sound = false;
	}

	if (gameState->carExpo_sound && gameState->ui_gameplay)
	{
		Mix_Volume(5, MIX_MAX_VOLUME);
		if (!Mix_Playing(5)) {
			Mix_PlayChannel(5, car_expo, 0);
		}
		gameState->carExpo_sound = false;
	}

	if (gameState->carCrashStatic_sound && gameState->ui_gameplay)
	{
		Mix_Volume(6, MIX_MAX_VOLUME);
		if (!Mix_Playing(6)) {
			Mix_PlayChannel(6, car_crash_static, 0);
		}
		gameState->carCrashStatic_sound = false;
	}

	if (gameState->carPowerUp_sound && gameState->ui_gameplay)
	{
		Mix_Volume(7, MIX_MAX_VOLUME);
		if (!Mix_Playing(7)) {
			Mix_PlayChannel(7, car_powerUp, 0);
		}
		gameState->carPowerUp_sound = false;
	}

	if (gameState->ui_enter)
	{
		//Mix_Volume(0, MIX_MAX_VOLUME);
		Mix_PlayChannel(-1, ui_enter, 0);
		gameState->ui_enter = false;
	}

	if (gameState->ui_switch)
	{
		//Mix_Volume(0, MIX_MAX_VOLUME);
		Mix_PlayChannel(-1, ui_click, 0);
		gameState->ui_switch = false;
	}

	if (gameState->ui_menu)
	{
		Mix_VolumeMusic(MIX_MAX_VOLUME / 2.5);
		playMusic();
	}
	else if (gameState->ui_pauseMenu)
	{
		Mix_VolumeMusic(MIX_MAX_VOLUME);
		if (!Mix_PlayingMusic()) {
			Mix_RewindMusic();
			Mix_SetMusicPosition(12.0);
			Mix_FadeInMusicPos(bgm_pause, -1, 0, 9.5);
		}
	}
	else if (gameState->ui_win)
	{
		Mix_VolumeMusic(MIX_MAX_VOLUME);
		if (!Mix_PlayingMusic()) {
			Mix_PlayMusic(bgm_win,-1);
		}
	}
	else if (gameState->ui_lose)
	{
		Mix_VolumeMusic(MIX_MAX_VOLUME);
		if (!Mix_PlayingMusic()) {
			Mix_PlayMusic(bgm_lose,-1);
		}
	}
	else if (gameState->ui_gameplay)
	{
		Mix_VolumeMusic(MIX_MAX_VOLUME/3);
		if (!Mix_PlayingMusic()) {
			Mix_PlayMusic(bgm_gamePlay, -1);
		}
	}
	else 
	{
		haltMusic();
	}

	/* testing code
	if (input == "N")
	{ 
		gameState->carExpo_sound = true;
	}

	else if (input == "M")
	{
		gameState->carExpo_sound = false;
	}
	else if (input == "LSHIFT")
	{
		switchMusic();
	}
	*/
	return 0;
}

void Audio_Controller::playMusic()
{
	if (!Mix_PlayingMusic())
		Mix_PlayMusic(bgm, -1);
}

void Audio_Controller::pauseMusic()
{
	if (Mix_PlayingMusic() != 0)
		Mix_PauseMusic();
}

void Audio_Controller::resumeMusic()
{
	if (Mix_PausedMusic() != 0)
		Mix_ResumeMusic();
}

void Audio_Controller::haltMusic()
{
	Mix_HaltMusic();
}

void Audio_Controller::switchMusic()
{
	if (currentMusic == "bgm.mp3")
		currentMusic = "bgm2.mp3";
	else
		currentMusic = "bgm.mp3";

	Mix_PlayMusic(myMusics->getMusic(currentMusic), -1);
}




