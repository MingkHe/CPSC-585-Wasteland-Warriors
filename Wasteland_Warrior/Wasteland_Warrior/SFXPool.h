#pragma once
#include <SDL_mixer.h>
#include <map>

class SFXPool
{
private:
	std::map<std::string, Mix_Chunk*> mySFX;

public:
	SFXPool();
	~SFXPool();

	Mix_Chunk* getSFX(std::string fileName);
	int getPoolSize();
	void clearPool();
};