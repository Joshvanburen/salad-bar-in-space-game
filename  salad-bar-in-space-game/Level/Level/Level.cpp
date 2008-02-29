
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
Level::Level(scene::ISceneManager* smgr) : m_SceneNode(NULL){
	m_Status = Level::WAITING_START;
	m_Time = 0;
}

Level::~Level(){
	this->shutdown();
}
//Updates the level
void Level::update()
{
	EntityVector::iterator worldEntityEnd = m_WorldEntities.end;
	for(m_WorldEntityItr = m_WorldEntities.begin(); m_WorldEntityItr != worldEntityEnd; m_WorldEntityItr++){
		(*m_WorldEntityItr).update();
	}

}

void Level::shutdown(){
	m_WorldEntities.clear();
	smgr = NULL;
	m_SceneNode = NULL;
	m_Status = Level::STOPPED;
}


std::ostream& operator << (std::ostream& os, const Level::Level& level){
	return os << "Current Level: " << level.getName() << ", Status: " << level.status();
}


bool Level::load(const std::string& LevelDefinition)
{
	LevelManager::getSingleton().getSceneManager->clear();
	IrrXMLReader* xml = createIrrXMLReader(XMLEntityDefinition);

	// strings for storing the data we want to get out of the file
	std::string entityName;
	int entityX;
	int entityY;
	std::string entityStartState;

	m_XmlFile = XMLEntityDefinition;

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_TEXT:
			//No text nodes
			break;

		case EXN_ELEMENT:
			if (!strcmp("level", xml->getNodeName())){
				m_LevelName = xml->getAttributeValue("name");
				m_MusicFile = xml->getAttributeValue("music");
				m_time = atoi(xml->getAttributeValue("time"));
				m_StartingX = atoi(xml->getAttributeValue("startingx"));
				m_StartingY = atoi(xml->getAttributeValue("startingy"));
				m_LevelFile = xml->getAttributeValue("map");

			}
			else if (!strcmp("entity")){
				entityName = xml->getAttributeValue("name");
				entityStartState = xml->getAttributeValue("startstate");
				entityX = atoi(xml->getAttributeValue("xloc"));
				entityY = atoi(xml->getAttributeValue("yloc"));

				WorldEntity& entity = EntityManager::getSingleton().createEntity(entityName);

				entity.setLocation(entityX, entityY, 0);
				entity.changeState(entityStartState);
				m_WorldEntities.push_back(entity);

				//WorldEntity is assumed to have been added to the sceneManager by this point by its corresponding factory.
				//We simply need to add it to as a child to the level scene node outside of the xml reading loop?

			}
			break;
		}
	}

	//Load in level now that we have the variables.
	LevelManager::getSingleton().getSceneManager->loadScene(m_LevelFile);
	
	//Adds a firt person shooter camera scene node
	LevelManager::getSingleton().getSceneManager->addCameraSceneNodeFPS();

	//Makes the mouse invisible
	device->getCursorControl()->setVisible(false);
}

//Gets the level name
string Level::getLevelName()
{
	//Returns the name of the level
	return m_LevelName;
}

//Gets the music file name
string Level::getMusicFileName()
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