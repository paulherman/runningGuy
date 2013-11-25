#include "main.hpp"

void StateMenu::Terminate()
{
}
void StateMenu::Initialize()
{
	//Load the background sprite
	loadSprite("./resources/background.jpg", "bg");
}
void StateMenu::Load()
{
}
void StateMenu::Unload()
{
}
bool StateMenu::Event(const SEvent& MyEvent)
{
	return false;
}
void StateMenu::Update(s32 time)
{
	//Draw the background
	drawSprite("bg", 0, 0, 1000, 1000);
}
