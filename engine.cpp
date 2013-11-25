#include "engine.hpp"

/**
 * Draws a 2D sprite on the screen.
 *
 * @param name
 *   The name of the sprite.
 * @param x1
 *   The top left x coordinate on the screen.
 * @param y1
 *   The top left y coordinate on the screen.
 * @param x2
 *   The bottom right x coordinate on the screen.
 * @param y2
 *   The bottom right y coordinate on the screen.
 * @param sx1
 *   The top left x coordinate on the sprite.
 * @param sy1
 *   The top left y coordinate on the sprite.
 * @param sx2
 *   The bottom right x coordinate on the sprite.
 * @param sy2
 *   The bottom right y coordinate on the sprite.
 * @param source_absolute
 *   Set to true in order to use absolute sprite coordinates.
 * @param absolute
 *   Set to true in order to use absolute screen coordinates.
 */
void GameState::drawSprite(std::string name, int x1, int y1, int x2, int y2, int sx1, int sy1, int sx2, int sy2, bool source_absolute, bool absolute)
{
	if (sy1 == -1)
		sy1 = 0;
	if (sx1 == -1)
		sx1 = 0;
	if (source_absolute == false)
	{
		if (sx2 == -1)
			sx2 = 1000;
		if (sy2 == -1)
			sy2 = 1000;
		sx1 = (float)(sx1 * m_Sprites[name]->getSize().Width) / 1000;
		sy1 = (float)(sy1 * m_Sprites[name]->getSize().Height) / 1000;
		sx2 = (float)(sx2 * m_Sprites[name]->getSize().Width) / 1000;
		sy2 = (float)(sy2 * m_Sprites[name]->getSize().Height) / 1000;
	}
	else
	{
		if (sx2 == -1)
			sx2 = m_Sprites[name]->getSize().Width;
		if (sy2 == -1)
			sy2 = m_Sprites[name]->getSize().Height;
	}
	if (absolute == false)
	{
		x1 = (float)(x1 * m_Driver->getScreenSize().Width) / 1000;
		x2 = (float)(x2 * m_Driver->getScreenSize().Width) / 1000;
		y1 = (float)(y1 * m_Driver->getScreenSize().Height) / 1000;
		y2 = (float)(y2 * m_Driver->getScreenSize().Height) / 1000;
	}
	rect<s32> destRect(max(0, x1), y1, max(0, x2), y2);
	rect<s32> sourceRect(sx1, sy1, sx2, sy2);
	rect<s32> clipRect(x1, y1, x2, y2);
	vector2d<s32> destPos(x1, y1);
	m_Driver->enableMaterial2D(true);
	m_Driver->draw2DImage(m_Sprites[name], destRect, sourceRect, 0, 0, true);
	m_Driver->enableMaterial2D(false);
}

/**
 * Loads a 2D sprite.
 *
 * @param path
 *   The path to the sprite to load.
 * @param
 *   The name of the sprite.
 */
void GameState::loadSprite(char *path, std::string name)
{
	m_Sprites.insert(std::pair<std::string, ITexture *>(name, m_Driver->getTexture(path)));
}

/**
 * Initializes the state.
 */
void GameState::StartUp(IrrlichtDevice *device, ISceneManager *scene, IGUIEnvironment *GUI, IVideoDriver *driver)
{
	m_Device = device;
	m_Scene = scene;
	m_GUI = GUI;
	m_Driver = driver;
	Initialize();
}

/**
 * Changes the current state.
 *
 * @param state
 *   The integer representing the state to which to change.
 */
void StateManager::ChangeState(int state)
{
	if(m_States.size() > state)
	{
		while(m_Locked == true);
		m_Locked = true;
		m_States[m_State]->Unload();
		m_State = state;
		m_States[m_State]->Load();
		m_Locked = false;
	}
}

/**
 * Initializes the state manager and Irrlicht library.
 *
 * @param title
 *   The title caption of the window.
 */
void StateManager::Startup(wchar_t *title)
{
	//TODO: use the configuration parser
	m_Resolution[0] = 500;
	m_Resolution[1] = 200;
	int Fullscreen = 0;
	m_DriverType = video::EDT_OPENGL;
	m_Depth = 32;
	m_vSync = false;
	m_Device = createDevice(m_DriverType, core::dimension2d<u32>(m_Resolution[0], m_Resolution[1]), m_Depth, m_Fullscreen, false, m_vSync, this);
	if (m_Device != NULL)
	{
		m_Driver = m_Device->getVideoDriver();
		m_GUI = m_Device->getGUIEnvironment();
		m_Scene = m_Device->getSceneManager();
	}
	m_Device->setWindowCaption(title);
	m_Device->setResizable(false);
}

/**
 * Gets the current width of resolution.
 *
 * @return
 *   The current width of resolution.
 */
int StateManager::GetResolutionX()
{
	return m_Resolution[0];
}

/**
 * Gets the current height of resolution.
 *
 * @return
 *   The current height of resolution.
 */
int StateManager::GetResolutionY()
{
	return m_Resolution[1];
}

/**
 * Adds a state to the list.
 *
 * @param state
 *   Pointer to the state which will be added.
 */
int StateManager::AddState(GameState *state)
{
	while(m_Locked == true);
	m_Locked = true;
    m_States.push_back(state);
	m_States[m_States.size() - 1]->StartUp(m_Device, m_Scene, m_GUI, m_Driver);
	m_Locked = false;
    return m_States.size() - 1;
}

/**
 * Gets the current color depth.
 *
 * @return
 *   An integer representing the current color depth.
 */
int StateManager::GetDepth()
{
	return m_Depth;
}

/**
 * Gets if the program runs in fullscreen mode.
 *
 * @return
 *   True if the program runs in fullscreen mode, false otherwise.
 */
bool StateManager::GetFullscreen()
{
	return m_Fullscreen;
}

/**
 * Catches the events and sends them to the current state to be handled.
 *
 * @param MyEvent
 *   The event which has been catched.
 * @return
 *   A boolean representing if the event has been handled.
 */
bool StateManager::OnEvent(const SEvent& MyEvent)
{
	if(m_States.size() > m_State)
		return m_States[m_State]->Event(MyEvent);
	else
		return true;
}

/**
 * Shuts the program down and clears all the states.
 */
void StateManager::Shutdown()
{
	m_Run = false;
	while (m_Locked == true);
	m_Locked = true;
	for (int i=0; i<m_States.size(); i++)
	{
		m_States[i]->Terminate();
	}
	m_Locked = false;
}

/**
 * Runs the main loop of the program and calls the update function for the
 * current state.
 *
 * @param state
 *   The number representing the state returned by the AddState method.
 */
void StateManager::Loop(int state)
{
	if(m_States.size() > state)
	{
		while(m_Locked == true);
		m_Locked = true;
		m_State = state;
		m_States[m_State]->Load();
		m_Locked = false;
		m_Run = true;
		u32 then = m_Device->getTimer()->getTime();
		u32 now = 0;
		while ((m_Run == true) && (m_Device->run() == true))
		{
			while (m_Locked == true);
			m_Locked = true;
			now = m_Device->getTimer()->getTime();
			m_Driver->beginScene(true, true, video::SColor(204, 204, 204, 204));
			m_States[m_State]->Update((float)(now - then));
			m_Driver->endScene();
			then = now;
			m_Locked = false;
		}
	}
}
