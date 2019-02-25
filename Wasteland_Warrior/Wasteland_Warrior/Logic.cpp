#include "Logic.h"

Logic::Logic()
{
}

Logic::~Logic()
{
}

void Logic::Update(Gamestate* gameState)
{
	//if player->health > 0.0
	if (100 > 0.0) {
		if (wave == 0) {
			//create 3 enemy AIs
			wave = 1;
			waveBreak = 1;
			breakTime = 30 * 60; // 30 seconds times framerate
		}

		//WAVE 1
		else if (wave == 1) {
			//if there are no enemy AIs left
				wave = 2;
				//spawn power ups
			//else
				//for each enemy AI
					//check enemy AI health 
					//if  enemy AI health < 0.0
						//delete enemy AI			
		}
		else if (waveBreak == 1) {
			if (breakTime <= 0) {
				waveBreak = 2;
				breakTime = 30 * 60; // 30 seconds times framerate
				//remove powerups
				//create 5 enemy AIs
			}
			breakTime--;
		}

		//WAVE 2
		else if (wave == 2) {
			//if there are no enemy AIs left
				wave = 3;
				//spawn power ups
			//else
				//for each enemy AI
					//check enemy AI health 
					//if  enemy AI health < 0.0
						//delete enemy AI			
		}
		else if (waveBreak == 2) {
			if (breakTime <= 0) {
				waveBreak = 3;
				breakTime = 30 * 60; // 30 seconds times framerate
				//remove powerups
				//create 10 enemy AIs
			}
			breakTime--;
		}

		//WAVE 3
		else if (wave == 3) {
			//if there are no enemy AIs left
				wave = 4;
				//spawn power ups
			//else
				//for each enemy AI
					//check enemy AI health 
					//if  enemy AI health < 0.0
						//delete enemy AI			
		}
		else if (waveBreak == 3) {
			if (breakTime <= 0) {
				waveBreak = 4;
				breakTime = 30 * 60; // 30 seconds times framerate
				//remove powerups
				//create 15 enemy AIs
			}
			breakTime--;
		}

		//WAVE 4
		else if (wave == 4) {
			//if there are no enemy AIs left
				wave = 5;
				//spawn power ups
			//else
				//for each enemy AI
					//check enemy AI health 
					//if  enemy AI health < 0.0
						//delete enemy AI			
		}
		else if (waveBreak == 4) {
			if (breakTime <= 0) {
				waveBreak = 5;
				breakTime = 30 * 60; // 30 seconds times framerate
				//remove powerups
				//create 20 enemy AIs
			}
			breakTime--;
		}

		//WAVE 5
		else if (wave == 5) {
			//if there are no enemy AIs left
				wave = 6;
				//spawn power ups
			//else
				//for each enemy AI
					//check enemy AI health 
					//if  enemy AI health < 0.0
						//delete enemy AI			
		}

		//Player has beaten all five waves
		else if (wave == 6) {
			//Game over
		}
	}

	//Player has lost all health
	else {
		//Game over
	}
}