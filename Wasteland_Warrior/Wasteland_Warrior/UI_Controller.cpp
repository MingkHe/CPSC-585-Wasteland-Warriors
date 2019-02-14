#include "UI_Controller.h"

UI_Controller::UI_Controller()
{
}

UI_Controller::~UI_Controller()
{
}

void UI_Controller::Update(Gamestate* GameState)
{
	//In Game UI
	if (GameState->UIMode == "Game") {

		//render UI elements over already rendered scene.
		//Based on info from GameState.

	//Pause Game Menu
	} else if (GameState->UIMode == "Pause") {

		//render pause game image. 
		//Should update based on selected menu item.
	
	//Start Menu
	} else if (GameState->UIMode == "Start") {

		//render start screen image
		//Should update based on selected menu item.
	}
}