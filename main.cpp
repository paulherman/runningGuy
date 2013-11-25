#include "main.hpp"

StateManager stateManager;
StatePlaying statePlaying;
StateMenu stateMenu;

int main()
{
	stateManager.Startup(L"RunningGuy");
	stateManager.AddState(&statePlaying);
	stateManager.AddState(&stateMenu);
	stateManager.Loop(0);
	stateManager.Shutdown();
	return 0;
}
