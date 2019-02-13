#include "Audio_Controller.h"

Audio_Controller::Audio_Controller()
{
	//testing audio
	SDL_Init(SDL_INIT_AUDIO);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		printf("Mixer initialization error: %s\n",Mix_GetError());

	myMusics = new MusicPool();
	currentMusic = "bgm.mp3";
}


Audio_Controller::~Audio_Controller()
{
	Mix_Quit();
}

int Audio_Controller::playSound(Gamestate* gameState)
{
	//Implementation
	std::string input = gameState->button;

	printf("Button: %s\n",input.c_str());

	printf("Music: %s\n",currentMusic.c_str());

	printf("Pool Size: %d\n", myMusics->getPoolSize());

	if (input == "N")
	{ 
		if (!Mix_PlayingMusic())
			playMusic();
		else if (Mix_PausedMusic() != 0)
			resumeMusic();
		else
			pauseMusic();
	}

	else if (input == "M")
	{
		haltMusic();
	}
	else if (input == "LSHIFT")
	{
		switchMusic();
	}
	return 0;
}

void Audio_Controller::playMusic()
{
	if (!Mix_PlayingMusic())
		Mix_PlayMusic(myMusics->getMusic(currentMusic), -1);
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
	//Mix_RewindMusic(); Rewind music to beginning
}

void Audio_Controller::switchMusic()
{
	if (currentMusic == "bgm.mp3")
		currentMusic = "bgm2.mp3";
	else
		currentMusic = "bgm.mp3";

	Mix_PlayMusic(myMusics->getMusic(currentMusic), -1);
}
