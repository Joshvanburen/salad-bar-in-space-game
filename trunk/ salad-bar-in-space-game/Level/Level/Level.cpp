/*
CSCE 552
Level.cpp
Level class.
*/

#ifndef LEVEL_CPP
#define LEVEL_CPP

//Includes the irrlicht header and iostream
#include <irrlicht.h>
#include <iostream>

//Allows the use of vectors
#include <vector>

//Irrlicht namespace
using namespace irr;

//Links with the irrlicht library file
#pragma comment(lib, "Irrlicht.lib")

//Constructor
Level::Level(ISceneNode node, string lName, string mName, vector<WorldEntity> &wE)
{
	//Initializes the scene node, name of the level, music file, and vector of world entities
	sceneNode = node;
	levelName = lName;
	musicName = mName;
	worldEntities = *wE;
}

//Draws the level
void Level::drawLevel()
{
}

//Updates the level
void Level::update()
{
}

//Loads the level
void Level::load()
{
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
ISceneNode Level::getSceneNode()
{
	//Returns the scene node
	return sceneNode;
}

//Sets the scene node
void Level::setSceneNode(ISceneNode node)
{
	//Sets the scene node
	sceneNode = node;
}

//Gets the music file name
string Level::getMusicFileName()
{
	//Returns the music file name
	return musicName;
}

//Sets the music file name
void Level::setMusicFileName(string mName)
{
	//Sets the music file name
	musicName = mName;
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