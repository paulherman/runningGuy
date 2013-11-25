#include "main.hpp"

#ifndef HEADER_STATE_PLAYING
#define HEADER_STATE_PLAYING

struct Block
{
	int x1, x2;
	int y1, y2;
	std::string sprite;
};

class StatePlaying : public GameState
{
public:
	virtual void Load();
	virtual void Unload();
	virtual void Update(int time);
	virtual void Initialize();
	virtual void Terminate();
	virtual bool Event(const SEvent& MyEvent);
private:
	void initBlock(int b);
	void loadBlock(int x, int b);
	vector< vector<Block> > m_LoadedBlocks;
	float m_Level;
	float m_Drawback;
	int m_dx;
	int m_dy;
	std::list<Block> m_Blocks;
	Block m_Guy;
	pair<int, int> m_Position;
	bool m_ButtonGravity;
	bool m_ButtonPause;
};
#endif
