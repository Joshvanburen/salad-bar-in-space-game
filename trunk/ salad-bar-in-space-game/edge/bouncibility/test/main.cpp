#include "Common.h"
#include "MastEventReceiver.cpp"
#include "GameIncludes.h"


#define WIN32_LEAN_AND_MEAN
#include "windows.h"


/* 
There are 5 sub namespaces in the Irrlicht Engine. Take a look 
at them, you can read a detailed description of them in the 
documentation by clicking on the top menu item 'Namespace List'
or using this link: http://irrlicht.sourceforge.net/docu/namespaces.html.
Like the irr Namespace, we do not want these 5 sub namespaces now,
to keep this example simple. Hence we tell the compiler again
that we do not want always to write their names:
*/
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::gui;


void PrintString(std::string& str){

	std::cout << str << std::endl;

}
irr::scene::ISceneManager* smgr;
irr::IrrlichtDevice *device;

bool running = true;

bool init(){

	ScriptManager::getSingleton().init();

	SoundManager::getSingleton().init();

	ScriptManager::getSingleton().registerAsGlobal("void PrintString(string &in)", ::asFUNCTION(PrintString));
	InputManager::getSingleton().init();

	device = irr::createDevice( irr::video::EDT_DIRECT3D9, irr::core::dimension2d<irr::s32>(1280, 1024), 16,
		false, false, false, InputManager::getSingleton().getEventReceiver());

	smgr = device->getSceneManager();

	WorldEntityAIManager::getSingleton().init();
	//PhysicsManager must be initialized before LevelManager because Entity initialization requires physics.
	PhysicsManager::getSingleton().init(device);

	GameSystem::getSingleton().init();


	LevelManager::getSingleton().init(device, "./res/scenarios/tutorial.xml");



	
	LevelManager::getSingleton().startGame();



	return true;
}

InputManager* m_InputMgr = InputManager::getSingletonPtr();
void getInput()
{
	while(running){
		m_InputMgr->getWiimote().poll();
		::Sleep(10);
	}
}

/*
This is the main method. We can use int main() on every platform.
On Windows platforms, we could also use the WinMain method
if we would want to get rid of the console window, which pops up when
starting a program with main(), but to keep this example simple,
we use main().
*/
int main()
{
	init();

	/*
	Set the caption of the window to some nice text. Note that there is 
	a 'L' in front of the string. The Irrlicht Engine uses wide character
	strings when displaying text.
	*/
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	Input::Action* quit = InputManager::getSingleton().createAction("quit", InputManager::getSingleton().getKeyboard(), Input::Keyboard::KEY_ESC, Input::Action::BEHAVIOR_DETECT_TAP);
	quit->addCode(Input::Wiimote::WII_HOME_BUTTON, InputManager::getSingleton().getWiimote());
	/*
	Get a pointer to the video driver, the SceneManager and the
	graphical user interface environment, so that
	we do not always have to write device->getVideoDriver(),
	device->getSceneManager() and device->getGUIEnvironment().
	*/
	IVideoDriver* driver = device->getVideoDriver();


	/*
	To display something interesting, we load a Quake 2 model 
	and display it. We only have to get the Mesh from the Scene 
	Manager (getMesh()) and add a SceneNode to display the mesh.
	(addAnimatedMeshSceneNode()). Instead of writing the filename
	sydney.md2, it would also be possible to load a Maya object file
	(.obj), a complete Quake3 map (.bsp) or a Milshape file (.ms3d).
	By the way, that cool Quake 2 model called sydney was modelled
	by Brian Collins.
	*/


	/*
	To let the mesh look a little bit nicer, we change its material a
	little bit: We disable lighting because we do not have a dynamic light
	in here, and the mesh would be totally black. Then we set the frame
	loop, so that the animation is looped between the frames 0 and 310.
	And at last, we apply a texture to the mesh. Without it the mesh
	would be drawn using only a color.
	*/


	/*
	To look at the mesh, we place a camera into 3d space at the position
	(0, 30, -40). The camera looks from there to (0,5,0).
	*/

	//Makes the mouse invisible
	device->getCursorControl()->setVisible(false);


	boost::thread inputThread(&getInput);

	//LevelManager::getSingleton().getSceneManager()->loadScene("./res/levels/football.irr");
	//smgr->addSkyDomeSceneNode(driver->getTexture("./res/textures/skyWarp_evening.jpg"), 10, 10, 1.0f, 1.0f);
	
	PhysicsManager* m_PhysicsMgr = PhysicsManager::getSingletonPtr();
	LevelManager* m_LevelMgr = LevelManager::getSingletonPtr();
	GameSystem* m_GameSystem = GameSystem::getSingletonPtr();
	while(device->run())
	{
		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene clears the screen with a color and also the
		depth buffer if wanted. Then we let the Scene Manager and the
		GUI Environment draw their content. With the endScene() call
		everything is presented on the screen.
		*/
		m_InputMgr->stopPolling();
		m_PhysicsMgr->update();
		m_LevelMgr->update();
		m_GameSystem->update();
		m_InputMgr->getInput();
		driver->beginScene(true, true, SColor(255,100,101,140));
			smgr->drawAll();
		driver->endScene();

		if(quit->isPressed()){
			running = false;
			inputThread.join();
			break;
		}
		m_InputMgr->resumePolling();
	}

	/*
	After we are finished, we have to delete the Irrlicht Device
	created before with createDevice(). In the Irrlicht Engine, 
	you will have to delete all objects you created with a method or 
	function which starts with 'create'. The object is simply deleted
	by calling ->drop().
	See the documentation at 
	http://irrlicht.sourceforge.net//docu/classirr_1_1IUnknown.html#a3
	for more information.
	*/

	m_InputMgr->shutdown();
	m_LevelMgr->shutdown();
	SoundManager::getSingleton().shutdown();
	m_PhysicsMgr->shutdown();
	ScriptManager::getSingleton().shutdown();
	WorldEntityAIManager::getSingleton().shutdown();
	device->drop();

	return 0;
}

