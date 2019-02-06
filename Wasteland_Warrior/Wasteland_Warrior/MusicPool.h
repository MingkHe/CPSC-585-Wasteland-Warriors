#pragma once
#include <SDL_mixer.h>
#include <map>

class MusicPool
{
private:
	std::map<std::string, Mix_Music*> myMusic;

public:
	MusicPool();
	~MusicPool();

	Mix_Music* getMusic(std::string fileName);
};