#include "main.hpp"

bool removeOldBlocks(const Block &blk)
{
	return (blk.x2 < 0);
}
void StatePlaying::Load()
{
	m_Blocks.clear();
	loadBlock(0, 0);
	m_dx = 1;
	m_dy = 13;
	m_Guy.x1 = 480;
	m_Guy.x2 = 520;
	m_Guy.y1 = 800;
	m_Guy.y2 = 900;
	m_Level = 0.3;
	m_Drawback = 0.1;
}
void StatePlaying::Unload()
{
}
void StatePlaying::Update(int time)
{
	//Draw the background
	drawSprite("bg", 0, 0, 1000, 1000);

	//Iterator for the blocks
	std::list<Block>::iterator i;

	//If there is a collision between the guy and the blocks
	bool collision_horizontal = false;
	bool collision_top = false;
	bool collision_bottom = false;

	//The distance which the guy overlaps the blocks
	int overlap_horizontal;
	int overlap_top;
	int overlap_bottom;

	//Erase the old blocks
	m_Blocks.remove_if(removeOldBlocks);

	//Get the length of the blocks loaded
	int x_max = 0;
	for (i = m_Blocks.begin(); i != m_Blocks.end(); ++i)
		if (i->x2 >= x_max)
			x_max = i->x2;
	//If it's not possible to render the next frame load a random block
	if (x_max < 1200)
		loadBlock(x_max, rand() % 1);

	//Check for game over
	if ((m_Guy.x2 <= 0) || (m_Guy.y2 <= 0) || (m_Guy.y1 >= 1000))
	{
		//Draw the game over and the score or the new highscore
	}
	else
	{
		//Loop through the blocks
		for (i = m_Blocks.begin(); i != m_Blocks.end(); i++)
		{
			//Assume that the guy does not overlap the block
			overlap_bottom = 0;
			overlap_horizontal = 0;
			overlap_top = 0;

			//If the guy overlaps on top, calculate the distance
			if ((m_Guy.y1 < i->y2) && (m_Guy.y1 > i->y1) && (m_Guy.x2 > i->x1) && (m_Guy.x1 < i->x2))
				overlap_top = i->y2 - m_Guy.y1;
			//If the guy overlaps on bottom, calculate the distance
			if ((m_Guy.y2 < i->y2) && (m_Guy.y2 > i->y1) && (m_Guy.x2 > i->x1) && (m_Guy.x1 < i->x2))
				overlap_bottom = m_Guy.y2 - i->y1;
			//If the guy overlaps on horizontal, calculate the distance
			if ((m_Guy.y1 < i->y2) && (m_Guy.y2 > i->y1) && (m_Guy.x2 > i->x1) && (m_Guy.x1 < i->x2))
				overlap_horizontal = m_Guy.x2 - i->x1;

			//If the guy overlaps on Oy more than on Ox there is a horizontal collision
			if (overlap_horizontal < (overlap_bottom + overlap_top))
			{
				//Mark the collision
				collision_horizontal = true;
				//Move the guy such that the graphics will be right
				m_Guy.x1 -= overlap_horizontal;
				m_Guy.x2 -= overlap_horizontal;
			}
			else
			{
				//Mov the guy such that the graphics will be right
				m_Guy.y1 += overlap_top - overlap_bottom;
				m_Guy.y2 += overlap_top - overlap_bottom;

				//Mark the top collision
				if (overlap_top > 0)
					collision_top = true;
				//Mark the bottom collision
				if (overlap_bottom > 0)
					collision_bottom = true;
			}
		}
		//Draw the guy
		if (m_dy > 0)
			drawSprite("ninja_up", max(0, m_Guy.x1), max(0, m_Guy.y1), max(0, m_Guy.x2), max(0, m_Guy.y2));
		else
			drawSprite("ninja_down", max(0, m_Guy.x1), max(0, m_Guy.y1), max(0, m_Guy.x2), max(0, m_Guy.y2));

		//Draw the blocks
		for (i = m_Blocks.begin(); i != m_Blocks.end(); i++)
			drawSprite(i->sprite, max(0, i->x1), i->y1, max(0, i->x2), i->y2, -min(0, i->x1) * 1000 / (i->x2 - i->x1), 0, 1000, 1000, true);

		//If the game is not paused then update the physics
		if (m_ButtonPause == false)
		{
			//If we have a horizontal collision
			if (collision_horizontal == true)
			{
				//Move the guy to the left
				m_Guy.x1 -= time * m_Drawback;
				m_Guy.x2 -= time * m_Drawback;

				//Move the world to the left
				for (i = m_Blocks.begin(); i != m_Blocks.end(); i++)
				{
					i->x1 -= time * m_Drawback;
					i->x2 -= time * m_Drawback;
				}
			}
			else
			{
				//If the guy was moved to the left due to a horizontal collision
				if (m_Guy.x1 < 480)
				{
					//Move the guy towards the center
					m_Guy.x1 += time * m_Drawback;
					m_Guy.x2 += time * m_Drawback;
				}
				else
				{
					//Correction if the guy is on the right due to precision of calculation
					m_Guy.x1 = 480;
					m_Guy.x2 = 520;
				}

				//Move the world to the left
				for (i = m_Blocks.begin(); i != m_Blocks.end(); i++)
				{
					i->x1 -= time * m_dx * m_Level;
					i->x2 -= time * m_dx * m_Level;
				}
			}
			//If the guy is on the ground and the change gravity button was pressed
			if ((m_ButtonGravity == true) && (((collision_top == true) && (m_dy < 0)) || ((collision_bottom == true) && (m_dy > 0))))
			{
				//Unpress the change gravity button
				m_ButtonGravity = false;
				//Change the gravity
				m_dy = -m_dy;
			}
			//If the guy is in the air
			if (((collision_top == false) && (m_dy < 0)) || ((collision_bottom == false) && (m_dy > 0)))
			{
				//Move the guy on Oy
				m_Guy.y1 += m_dy;
				m_Guy.y2 += m_dy;
			}
		}
	}
}

void StatePlaying::initBlock(int b)
{
	int count; //The number of subblocks
	vector<Block> blk; //A temporary subblock used for loading from file
	Block sblk; //A temporary subblock used for loading from file

	//Create a stream containing the name of the block file
	std::stringstream strm;
	strm << "./blocks/block_" << b << ".blu";
	//Opens the block file
	ifstream fin(strm.str().c_str());
	//Read the number of subblocks
	fin >> count;

	//As long as we have subblocks to load
	while (count > 0)
	{
		//Read the subblock from the file
		fin >> sblk.x1 >> sblk.y1 >> sblk.x2 >> sblk.y2 >> sblk.sprite;
		//Add the subblock to the block
		blk.push_back(sblk);
		//Decrease the number of subblocks to load
		count--;
	}
	//Close the block file
	fin.close();
	//Add the block to the list
	m_LoadedBlocks.push_back(blk);
}

void StatePlaying::loadBlock(int x, int b)
{
	Block sblk;
	for (int i = 0; i < m_LoadedBlocks[b].size(); i++)
	{
		sblk = m_LoadedBlocks[b][i];
		sblk.x1 += x;
		sblk.x2 += x;
		m_Blocks.push_back(sblk);
	}
}

void StatePlaying::Initialize()
{
	//Load the background sprite
	loadSprite("./resources/background.jpg", "bg");
	//Load the ninja running, step 1
	loadSprite("./resources/ninja_1.png", "ninja_up");
	//Load the ninja running step 2
	loadSprite("./resources/ninja_2.png", "ninja_down");
	for (int i = 0; i < 1; i++)
		initBlock(i);
	//Generate a random seed from the current time since UNIX Epoch
	srand(time(0));
}

void StatePlaying::Terminate()
{
}

bool StatePlaying::Event(const SEvent& MyEvent)
{
	//If the event was a keyboard or a mouse one
	if (MyEvent.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		//If a button was pressed
		if (MyEvent.KeyInput.PressedDown == true)
		{
			//Key 'Space' changes gravity
			if ((MyEvent.KeyInput.Key == KEY_SPACE) && (m_ButtonPause == false))
				m_ButtonGravity = true;
			//Key 'Esc' goes to menu
			if (MyEvent.KeyInput.Key == KEY_ESCAPE)
				stateManager.ChangeState(1);
			//Key 'P' is pause
			if (MyEvent.KeyInput.Key == KEY_KEY_P)
				if (m_ButtonPause == false)
					m_ButtonPause = true;
				else
					m_ButtonPause = false;
		}
	}

	return false;
}
