/*
CSCE 552
Level.cpp
Level class.
*/

#ifndef LEVEL_CPP
#define LEVEL_CPP

//Includes the irrlicht header and iostream
#include <irrlicht.h>
#include "Level.h"
#include <iostream>

//Allows the use of vectors
#include <vector>

//Irrlicht namespace and standard namespace
using namespace irr;
using namespace std;

//Links with the irrlicht library file
#pragma comment(lib, "Irrlicht.lib")

//Constructor
Level::Level(string lName, string mName, string lfName, string mfName, vector<WorldEntity> wE)
{
	//Initializes the name of the level, music file, and vector of world entities
	levelName = lName;
	musicFile = mName;
	levelFileName = lfName;
	meshFileName = mfName;
	worldEntities = wE;

	//Calls the load function
	load();
}

//Draws the level
void Level::drawLevel()
{
	//While the device is running
	while(device->run())
	{
		//If the window is active
		if (device->isWindowActive())
		{
			//Begins the scene
			driver->beginScene(true, true, video::SColor(0,200,200,200));

			//Uses the scene manager to draw all nodes
			smgr->drawAll();

			//Ends the scene
			driver->endScene();
		}//End-if
	}//End-while
}

//Updates the level
void Level::update()
{
	//Not extactly sure what needs to be here
}

//Loads the level
void Level::load()
{
	//Sets the video driver type, assuming DX9
	video::E_DRIVER_TYPE driverType = video::EDT_DIRECT3D9;

	//Creates the device
	device = createDevice(driverType, core::dimension2d<s32>(640, 480));

	//Pointers to the video driver and scene manager
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();

	//Loads the map file
	device->getFileSystem()->addZipFileArchive((const c8*)levelFileName);

	//Loads the mesh and initializes the node
	scene::IAnimatedMesh* mesh = smgr->getMesh((const c8*)meshFileName);
	sceneNode = 0;

	//Creates the node if the mesh exists
	if(mesh)
	{
		//Creates the node
		sceneNode = smgr->addOctTreeSceneNode(mesh->getMesh(0), 0, -1, 128);
	}

	//If the node exists, set its positon (assumes that the level is centered on the origin)
	if(sceneNode)
	{
		//Sets the position of the node
		sceneNode->setPosition(core::vector3df(0,0,0));
	}
	
	//Adds a firt person shooter camera scene node
	smgr->addCameraSceneNodeFPS();

	//Makes the mouse invisible
	device->getCursorControl()->setVisible(false);

	//Calls the draw method
	drawLevel();
}

//Gets the level name
string Level::getLevelName()
{
	//Returns the name of the level
	return levelName;
}

//Sets the level name
void Level::setLevelName(string lName)
{
	//Sets the level name
	levelName = lName;
}

//Gets the scene node
scene::ISceneNode* Level::getSceneNode()
{
	//Returns the scene node
	return sceneNode;
}

//Sets the scene node
void Level::setSceneNode(scene::ISceneNode* node)
{
	//Sets the scene node
	sceneNode* = node;
}

//Gets the music file name
string Level::getMusicFileName()
{
	//Returns the music file name
	return musicFile;
}

//Sets the music file name
void Level::setMusicFileName(string mName)
{
	//Sets the music file name
	musicFile = mName;
}

//Gets the current time
int Level::getCurrentTime()
{
	//Returns the current level timer
	return time;
}

//Sets the current time
void Level::setCurrentTime(int ctime)
{
	//Sets the level timer
	time = ctime;
}

#endif LEVEL_CPP