#include "SFXPool.h"


SFXPool::SFXPool()
{
}


SFXPool::~SFXPool()
{
}

Mix_Chunk* SFXPool::getSFX(std::string fileName)
{
	//Implementation
	std::string path = "SoundEffect/" + fileName;
	
	if (mySFX[path] == nullptr) 
	{
		
		mySFX[path] = Mix_LoadWAV(path.c_str());

		if (mySFX[path] == NULL) {
			printf("SFX loading error: File %s Error %s", fileName.c_str(), Mix_GetError());
		}
		printf("OK \n");
		return mySFX[path];
	}

	return 0;
}

int SFXPool::getPoolSize() {
	return mySFX.size();
}

void SFXPool::clearPool() {
	mySFX.clear();
}