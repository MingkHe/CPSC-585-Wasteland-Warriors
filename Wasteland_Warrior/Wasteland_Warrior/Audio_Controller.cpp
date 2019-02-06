#include "Audio_Controller.h"



Audio_Controller::Audio_Controller()
{
	//testing audio
	SDL_Init(SDL_INIT_AUDIO);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		printf("Mixer initialization error: %s\n",Mix_GetError());

	myMusics = new MusicPool();
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

	if (input == "W")
	{ 
		playMusic();
	}
	else if (input == "A")
	{
		pauseMusic();
	}
	else if (input == "S")
	{
		resumeMusic();
	}
	else if (input == "D") 
	{
		haltMusic();
	}
	return 0;
}

void Audio_Controller::playMusic()
{
	if (!Mix_PlayingMusic())
		Mix_PlayMusic(myMusics->getMusic("bgm.mp3"), -1);
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
