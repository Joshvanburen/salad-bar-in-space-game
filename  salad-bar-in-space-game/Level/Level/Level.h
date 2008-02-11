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

//Irrlicht namespace
using namespace irr;

//Links with the irrlicht library file
#pragma comment(lib, "Irrlicht.lib")

//Class definition
class Level
{
	//Private methods and variables
	private:
			//Scene node
			ISceneNode sceneNode;
			//Vector to hold all world entities
			vector<WorldEntity> worldEntities;
			//Level name
			string levelName;
			//Filename of music file
			string musicFile;
	//Public methods and variables
	public:
			//Constructor
			Level(ISceneNode node, string lName, string mName);
			//Draws the level
			void drawLevel();
			//Updates the level and all children
			void update();
			//Gets the level name
			string getLevelName();
			//Sets the level name
			void setLevelName(string lName);
			//Gets the scene node
			ISceneNode getSceneNode();
			//Sets the scene node
			void setSceneNode(ISceneNode node);
			//Gets the music file name
			string getMusicFileName();
			//Sets the music file name
			void setMusicFileName(string mName);
			//Gets the current level time
			int getCurrentTime();
}


#endif