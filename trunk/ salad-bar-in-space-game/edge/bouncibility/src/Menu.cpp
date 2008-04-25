/*

Menu class file

*/

#include <irrlicht.h>
#include "Menu.h"
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

//Constructor 
Menu::Menu(irr::IrrlichtDevice* device)
{
	//Istantiates the device scene manager and video driver
	s_Device = device;
	s_Smgr = device->getSceneManager();
	s_Driver = device->getVideoDriver();

	//Calls the initialization function
	init();
}

//Destructor
Menu::~Menu()
{
}

//Initialization
void Menu::init()
{
	//Initializes the scene node and sets its material
	scene::IBillboardSceneNode * b_newGame = s_Smgr->addBillboardSceneNode();
	b_newGame->setMaterialFlag(video::EMF_LIGHTING, false);
	b_newGame->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
	b_newGame->setMaterialTexture(0, s_Driver->getTexture("newGame.bmp"));
	b_newGame->setSize(core::dimension2d<f32>(20.0f, 10.0f));
	b_newGame->setPosition(core::vector3df(0,10,0));

	scene::IBillboardSceneNode * b_quit = s_Smgr->addBillboardSceneNode();
	b_quit->setMaterialFlag(video::EMF_LIGHTING, false);
	b_quit->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
	b_quit->setMaterialTexture(0, s_Driver->getTexture("quit.bmp"));
	b_quit->setSize(core::dimension2d<f32>(20.0f, 10.0f));
	b_quit->setPosition(core::vector3df(0,-5,0));	
}

//Quit
void Menu::quit()
{
}

//New game
void Menu::newGame()
{
}

//Pause
void Menu::pause()
{
}