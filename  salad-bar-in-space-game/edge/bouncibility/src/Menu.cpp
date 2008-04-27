/*

Menu class file

*/

#include <irrlicht.h>
#include "Menu.h"
#include "GameIncludes.h"
#include "console.h"
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
Menu::Menu()
{
}

//Destructor
Menu::~Menu()
{
}

//Initialization
void Menu::init(irr::IrrlichtDevice* device)
{

	//Istantiates the device scene manager and video driver
	s_Device = device;
	s_Smgr = device->getSceneManager();
	s_Driver = device->getVideoDriver();

	//Initializes the scene node and sets its material
	scene::IBillboardSceneNode * b_newGame = s_Smgr->addBillboardSceneNode();
	b_newGame->setMaterialFlag(video::EMF_LIGHTING, false);
	b_newGame->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
	b_newGame->setMaterialTexture(0, s_Driver->getTexture("textures/buttons/newGame.bmp"));
	b_newGame->setSize(core::dimension2d<f32>(20.0f, 10.0f));
	b_newGame->setPosition(core::vector3df(0,10,0));

	scene::IBillboardSceneNode * b_quit = s_Smgr->addBillboardSceneNode();
	b_quit->setMaterialFlag(video::EMF_LIGHTING, false);
	b_quit->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
	b_quit->setMaterialTexture(0, s_Driver->getTexture("textures/buttons/quit.bmp"));
	b_quit->setSize(core::dimension2d<f32>(20.0f, 10.0f));
	b_quit->setPosition(core::vector3df(0,-5,0));	
}

//Quit
void Menu::quit()
{
	//Exits the program
	exit(0);
}

//New game
void Menu::newGame()
{
	//Calls the start game method from game system
	GameSystem* gs = GameSystem::getSingletonPtr();

	gs->startGame();


}

//Pause
void Menu::pause()
{
}

//UnPause
void Menu::unPause()
{
}