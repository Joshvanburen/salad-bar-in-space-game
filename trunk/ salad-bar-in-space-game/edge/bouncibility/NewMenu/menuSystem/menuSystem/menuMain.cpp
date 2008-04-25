
#include <irrlicht.h>
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

int main()
{

	IrrlichtDevice *device = createDevice( video::EDT_OPENGL, dimension2d<s32>(640, 480), 16, false, false, false, 0);

	device->setWindowCaption(L"Menu System");

	/*
	Get a pointer to the video driver, the SceneManager and the
	graphical user interface environment, so that
	we do not always have to write device->getVideoDriver(),
	device->getSceneManager() and device->getGUIEnvironment().
	*/
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode();
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
	bill->setMaterialTexture(0, driver->getTexture("newGame.bmp"));
	bill->setSize(core::dimension2d<f32>(20.0f, 10.0f));
	bill->setPosition(core::vector3df(0,10,0));

	scene::IBillboardSceneNode * quit = smgr->addBillboardSceneNode();
	quit->setMaterialFlag(video::EMF_LIGHTING, false);
	quit->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
	quit->setMaterialTexture(0, driver->getTexture("quit.bmp"));
	quit->setSize(core::dimension2d<f32>(20.0f, 10.0f));
	quit->setPosition(core::vector3df(0,-5,0));	

	/*
	To look at the mesh, we place a camera into 3d space at the position
	(0, 30, -40). The camera looks from there to (0,5,0).
	*/
	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

	/*
	Ok, now we have set up the scene, lets draw everything:
	We run the device in a while() loop, until the device does not
	want to run any more. This would be when the user closed the window
	or pressed ALT+F4 in windows.
	*/
	while(device->run())
	{
	
		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}

