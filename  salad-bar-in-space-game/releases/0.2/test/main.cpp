


#include "irrlicht.h"
#include "MastEventReceiver.cpp"
#include "Common.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "irrnewt.hpp"
#include "PhysicsManager.h"
#include "LevelManager.h"
#include "Level.h"
#include "WorldEntity.h"
#include <stdio.h>

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


irr::scene::ISceneManager* smgr;
irr::IrrlichtDevice *device;

bool init(){
	InputManager::getSingleton().init();
	device = irr::createDevice( irr::video::EDT_DIRECT3D9, irr::core::dimension2d<irr::s32>(1024, 768), 16,
		false, false, false, InputManager::getSingleton().getEventReceiver());

	smgr = device->getSceneManager();

	
	PhysicsManager::getSingleton().init(device);

	LevelManager::getSingleton().init(device, "./res/scenarios/tutorial.xml");

	LevelManager::getSingleton().startGame();

	return true;
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

	/*
	Get a pointer to the video driver, the SceneManager and the
	graphical user interface environment, so that
	we do not always have to write device->getVideoDriver(),
	device->getSceneManager() and device->getGUIEnvironment().
	*/
	IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	/*
	We add a hello world label to the window, using the GUI environment.
	*/
	
	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<irr::s32>(10,10,260,22), true);

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
	smgr->addCameraSceneNode(0, irr::core::vector3df(0,100,-240), irr::core::vector3df(10,100,0));


	//Makes the mouse invisible
	device->getCursorControl()->setVisible(false);
	/*
	Ok, now we have set up the scene, lets draw everything:
	We run the device in a while() loop, until the device does not
	want to run any more. This would be when the user closed the window
	or pressed ALT+F4 in windows.
	*/

	bool show = false;
	while(device->run())
	{
		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene clears the screen with a color and also the
		depth buffer if wanted. Then we let the Scene Manager and the
		GUI Environment draw their content. With the endScene() call
		everything is presented on the screen.
		*/
		InputManager::getSingleton().stopPolling();
		InputManager::getSingleton().getInput();
		PhysicsManager::getSingleton().update();
		LevelManager::getSingleton().update();
		driver->beginScene(true, true, SColor(255,100,101,140));
			smgr->drawAll();
			guienv->drawAll();
		driver->endScene();

		InputManager::getSingleton().resumePolling();
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
	InputManager::getSingleton().shutdown();
	LevelManager::getSingleton().shutdown();
	PhysicsManager::getSingleton().shutdown();
	device->drop();

	return 0;
}

