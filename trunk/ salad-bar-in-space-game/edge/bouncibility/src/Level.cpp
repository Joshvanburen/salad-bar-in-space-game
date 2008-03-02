
#include <irrlicht.h>
#include "Common.h"
#include "EntityManager.h"
#include "LevelManager.h"
#include "WorldEntity.h"
#include "irrlicht.h"
#include "Level.h"

//Irrlicht namespace and standard namespace
using namespace irr;

//Links with the irrlicht library file
#pragma comment(lib, "Irrlicht.lib")

const int Level::WAITING_REPEAT = 1;
const int Level::RUNNING = 2;
const int Level::WAITING_START = 3;
const int Level::FINISHED = 4;
const int Level::STOPPED = 5;

//Constructor
Level::Level(){
	m_Status = Level::WAITING_START;
	m_Time = 0;
	m_StartingX = 0;
	m_StartingY = 0;
}

Level::~Level(){
	this->shutdown();
}
//Updates the level
void Level::update()
{
	::EntityVector::iterator worldEntityEnd = m_WorldEntities.end();
	for(m_WorldEntityItr = m_WorldEntities.begin(); m_WorldEntityItr != worldEntityEnd; m_WorldEntityItr++){
		(*m_WorldEntityItr)->update();
	}

}

void Level::shutdown(){
	m_WorldEntities.clear();
	m_Status = Level::STOPPED;
}


std::ostream& operator << (std::ostream& os, const Level& level){
	return os << "Current Level: " << level.m_LevelName << ", Status: " << level.m_Status;
}


bool Level::load(const std::string& LevelDefinition)
{
	LevelManager::getSingleton().getSceneManager()->clear();

	std::cout << "loading xml file for a level " << LevelDefinition << "\n";
	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(LevelDefinition.c_str());

	// strings for storing the data we want to get out of the file
	std::string entityName;
	int entityX;
	int entityY;
	std::string entityStartState;

	m_XmlFile = LevelDefinition;

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case irr::io::EXN_TEXT:
			//No text nodes
			break;

		case irr::io::EXN_ELEMENT:
			if (!strcmp("level", xml->getNodeName())){
				m_LevelName = xml->getAttributeValue("name");
				m_MusicFile = xml->getAttributeValue("music");
				m_Time = xml->getAttributeValueAsInt("time");
				m_StartingX = xml->getAttributeValueAsInt("startingx");
				m_StartingY = xml->getAttributeValueAsInt("startingy");
				m_LevelFile = xml->getAttributeValue("map");

			}
			else if (!strcmp("entity", xml->getNodeName())){
				entityName = xml->getAttributeValue("name");
				entityStartState = xml->getAttributeValue("startstate");
				entityX = xml->getAttributeValueAsInt("xloc");
				entityY = xml->getAttributeValueAsInt("yloc");
				std::cout << "loaded in entity attributes for entity " << entityName << "\n";

				WorldEntity& entity = EntityManager::getSingleton().createEntity(entityName);

				std::cout << "created entity\n";
				entity.setLocation((float)entityX, (float)entityY, 0);
				entity.changeState(entityStartState);
				std::cout << "changed state\n";
				m_WorldEntities.push_back(&entity);

				//WorldEntity is assumed to have been added to the sceneManager by this point by its corresponding factory.
				//We simply need to add it to as a child to the level scene node outside of the xml reading loop?

			}
			break;
		}
	}

	//Load in level now that we have the variables.
	LevelManager::getSingleton().getSceneManager()->loadScene(m_LevelFile.c_str());
	
	//Adds a firt person shooter camera scene node
	LevelManager::getSingleton().getSceneManager()->addCameraSceneNodeFPS();

	delete xml;
	return true;
}

//Gets the level name
const std::string& Level::getName()
{
	//Returns the name of the level
	return m_LevelName;
}

//Gets the music file name
const std::string& Level::getMusicFileName()
{
	//Returns the music file name
	return m_MusicFile;
}

//Gets the current time
int Level::getCurrentTime()
{
	//Returns the current level timer
	return m_Time;
}

//Sets the current time
void Level::setCurrentTime(int ctime)
{
	//Sets the level timer
	m_Time = ctime;
}

int Level::status(){
	return m_Status;
}

void Level::setStatus(int newStatus){
	m_Status = newStatus;
}