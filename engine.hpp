#ifndef HEADER_ENGINE
#define HEADER_ENGINE
#include <vector>
#include <map>
#include <string>
#include <irrlicht/irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

class GameState
{
public:
	/**
	 * Called when the state is activated.
	 */
	virtual void Load() = 0;

	/**
	 * Called when the state is deactivate.
	 */
	virtual void Unload() = 0;

	/**
	 * Called before a frame is prepared.
	 */
	virtual void Update(s32 time) = 0;

	/**
	 * Initializes the state.
	 */
	void StartUp(IrrlichtDevice *m_Device, ISceneManager *m_Scene, IGUIEnvironment *m_GUI, IVideoDriver *m_Driver);

	/**
	 * The user initialization.
	 */
	virtual void Initialize() = 0;

	/**
	 * Shutdowns the state.
	 */
	virtual void Terminate() = 0;

	/**
	 * Called when the state has to handle an event.
	 *
	 * @param MyEvent
	 *   The event which has to be handled.
	 * @return
	 *   True if the event was handled, false otherwise.
	 */
	virtual bool Event(const SEvent& MyEvent) = 0;

	/**
	 * The scene manager.
	 */

	ISceneManager *m_Scene;
	/**
	 * The GUI manager.
	 */

	IGUIEnvironment *m_GUI;
	/**
	 * The Irrlicht driver.
	 */

	IVideoDriver *m_Driver;
	/**
	 * The Irrlicht device.
	 */
	IrrlichtDevice *m_Device;

	/**
	 * Draws a 2D sprite on the screen.
	 */
	void drawSprite(std::string name, int x1, int x2, int y1, int y2, int sx1 = -1, int sx2 = -1, int sy1 = -1, int sy2 = -1, bool source_absolute = false, bool absolute = false);

	/**
	 * Loads a 2D sprite.
	 */
	void loadSprite(char *path, std::string name);
private:
	std::map<std::string, ITexture *> m_Sprites;
};

class StateManager : public IEventReceiver
{
public:
	/**
	 * Changes the current state.
	 *
	 * @param
	 *   The integer representing the state to which to change.
	 */
	void ChangeState(int state);

	/**
	 * Runs the main loop of the program and calls the update function for the
	 * current state.
	 *
	 * @param state
	 *   The number representing the state returned by the AddState method.
	 */
	void Loop(int state);

	/**
	 * Adds a state to the list.
	 *
	 * @param state
	 *   Pointer to the state which will be added.
	 */
	int AddState(GameState *state);

	/**
	 * Gets the current width of resolution.
	 *
	 * @return
	 *   The current width of resolution.
	 */
	int GetResolutionX();

	/**
	 * Gets the current height of resolution.
	 *
	 * @return
	 *   The current height of resolution.
	 */
	int GetResolutionY();

	/**
	 * Gets the current color depth.
	 *
	 * @return
	 *   An integer representing the current color depth.
	 */
	int GetDepth();

	/**
	 * Gets if the program runs in fullscreen mode.
	 *
	 * @return
	 *   True if the program runs in fullscreen mode, false otherwise.
	 */
	bool GetFullscreen();

	/**
	 * Initializes the state manager and Irrlicht library.
	 *
	 * @param title
	 *   The title caption of the window.
	 */
	void Startup(wchar_t *title);

	/**
	 * Shuts the program down and clears all the states.
	 */
	void Shutdown();
private:
	/**
	 * The scene manager.
	 */
	ISceneManager *m_Scene;
	/**
	 * The GUI manager.
	 */
	IGUIEnvironment *m_GUI;
	/**
	 * The Irrlicht driver.
	 */
	IVideoDriver *m_Driver;
	/**
	 * The Irrlicht device.
	 */
	IrrlichtDevice *m_Device;

	/**
	 * Catches the events and sends them to the current state to be handled.
	 *
	 * @param MyEvent
	 *   The event which has been catched.
	 * @return
	 *   A boolean representing if the event has been handled.
	 */
	bool OnEvent(const SEvent& MyEvent);

	/**
	 * The video driver used for rendering.
	 */
	video::E_DRIVER_TYPE m_DriverType;

	/**
	 * A vector containing the states.
	 */
	vector<GameState*> m_States;

	/**
	 * True if vSync is enabled, false otherwise.
	 */
	bool m_vSync;

	/**
	 * An array containing the resolution.
	 */
	int m_Resolution[2];

	/**
	 * True if the application runs in fullscreen, false otherwise.
	 */
	bool m_Fullscreen;

	/**
	 * The current color depth.
	 */
	int m_Depth;

	/**
	 * The current state running.
	 */
	int m_State;

	/**
	 * True if the current state is in use, false otherwise.
	 */
	bool m_Locked;

	/**
	 * Shows if the main loop is running.
	 */
	bool m_Run;
};
#endif
