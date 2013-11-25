#include "main.hpp"
#ifndef HEADER_STATE_MENU
#define HEADER_STATE_MENU
class StateMenu : public GameState
{
public:
	virtual void Load();
	virtual void Unload();
	virtual void Update(s32 time);
	virtual void Initialize();
	virtual void Terminate();
	virtual bool Event(const SEvent& MyEvent);
};
#endif
