#include "MusicPool.h"


MusicPool::MusicPool()
{
}


MusicPool::~MusicPool()
{
}

Mix_Music* MusicPool::getMusic(std::string fileName)
{
	//Implementation
	std::string path = "Music/" + fileName;
	if (myMusic[path] == nullptr) 
	{
		myMusic[path] = Mix_LoadMUS(path.c_str());

		if (myMusic[path] == NULL) {
			printf("Music loading error: File %s Error %s", fileName.c_str(), Mix_GetError());
		}

		return myMusic[path];
	}
	return 0;
}