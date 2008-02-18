/*
CSCE 552
Level.h
Header file for the level class.
*/

#ifndef LEVEL_H
#define LEVEL_H

//Includes the irrlicht header and iostream
#include <irrlicht.h>
#include <iostream>

//Allows the use of vectors
#include <vector>

//Irrlicht namespace and standard namespace
using namespace irr;
using namespace std;

//Links with the irrlicht library file
#pragma comment(lib, "Irrlicht.lib")

//Class definition
class Level
{
	//Private methods and variables
	private:
			//Scene node
			scene::ISceneNode* sceneNode;
			//Scene manager
			scene::ISceneManager* smgr;
			//Irrlicht device
			IrrlichtDevice *device;
			//Video driver
			video::IVideoDriver* driver;
			//Vector to hold all world entities
			vector<WorldEntity> worldEntities;
			//Level name
			string levelName;
			//Level file name
			string levelFileName;
			//Mesh file name
			string meshFileName;
			//Filename of music file
			string musicFile;
			//Level timer;
			int time;
	//Public methods and variables
	public:
			//Constructor
			Level(string lName, string mName, string lfName, string mfName, vector<WorldEntity> wE);
			//Draws the level
			void drawLevel();
			//Updates the level and all children
			void update();
			//Gets the level name
			string getLevelName();
			//Sets the level name
			void setLevelName(string lName);
			//Gets the scene node
			scene::ISceneNode* getSceneNode();
			//Sets the scene node
			void setSceneNode(scene::ISceneNode* node);
			//Gets the music file name
			string getMusicFileName();
			//Sets the music file name
			void setMusicFileName(string mName);
			//Gets the current level time
			int getCurrentTime();
			//Sets the current level time
			void setCurrentTime(int ctime);
			//Loads the level
			void load();
};


#endif