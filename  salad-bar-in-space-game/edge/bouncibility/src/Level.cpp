
#include <irrlicht.h>
#include "Common.h"
#include "EntityManager.h"
#include "irrnewt.hpp"
#include "LevelManager.h"
#include "WorldEntity.h"
#include "PhysicsManager.h"
#include "SoundManager.h"
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
Level::Level() : m_SceneNode(NULL), m_Mesh(NULL), m_Physics_Body(NULL){
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
	m_Music->stop();
	if (m_Physics_Body){
		m_Physics_Body->remove();
		m_Physics_Body = NULL;
	}
	m_Status = Level::STOPPED;
}


std::ostream& operator << (std::ostream& os, const Level& level){
	return os << "Current Level: " << level.m_LevelName << ", Status: " << level.m_Status;
}


bool Level::load(const std::string& LevelDefinition)
{
	LevelManager::getSingleton().getSceneManager()->clear();
	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(LevelDefinition.c_str());

	// strings for storing the data we want to get out of the file
	std::string entityName;
	int entityX;
	int entityY;
	std::string entityStartState;
	std::string handle = "";

	m_XmlFile = LevelDefinition;

	while(xml && xml->read())
	{
		handle = "";
		switch(xml->getNodeType())
		{
		case irr::io::EXN_TEXT:
			//No text nodes
			break;

		case irr::io::EXN_ELEMENT:
			if (!strcmp("level", xml->getNodeName())){
				m_LevelName = xml->getAttributeValue("name");
				m_MusicName = xml->getAttributeValue("music");
				m_Music = SoundManager::getSingleton().getSound(m_MusicName);
				m_Time = xml->getAttributeValueAsInt("time");
				m_StartingX = xml->getAttributeValueAsInt("startingx");
				m_StartingY = xml->getAttributeValueAsInt("startingy");
				m_LevelFile = xml->getAttributeValue("map");
				PhysicsManager::getSingleton().setGravity(xml->getAttributeValueAsFloat("gravity"));

			}
			else if (!strcmp("entity", xml->getNodeName())){
				entityName = xml->getAttributeValue("name");
				entityStartState = xml->getAttributeValue("startstate");
				entityX = xml->getAttributeValueAsInt("xloc");
				entityY = xml->getAttributeValueAsInt("yloc");
				if (xml->getAttributeValue("handle")){
					handle = xml->getAttributeValue("handle");
				}
				WorldEntity& entity = EntityManager::getSingleton().createEntity(entityName, handle);

				entity.setLocation((float)entityX, (float)entityY, 0);
				entity.changeState(entityStartState);
				m_WorldEntities.push_back(&entity);

				//WorldEntity is assumed to have been added to the sceneManager by this point by its corresponding factory.
				//We simply need to add it to as a child to the level scene node outside of the xml reading loop?

			}
			break;
		}
	}

	//Load in level now that we have the variables.
	LevelManager::getSingleton().getSceneManager()->loadScene(m_LevelFile.c_str());

	this->m_SceneNode = (irr::scene::IMeshSceneNode*)(LevelManager::getSingleton().getSceneManager()->getSceneNodeFromName("level_main"));
	m_Mesh = m_SceneNode->getMesh();
	
	irr::newton::SBodyFromNode mapData;

	mapData.Node = this->m_SceneNode;

	mapData.Mesh = this->m_Mesh;

	mapData.Type = newton::EBT_TREE;

	m_Physics_Body = PhysicsManager::getSingleton().getPhysicsWorld()->createBody(mapData);

	m_Music->play(true);

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
const std::string& Level::getMusicName()
{
	//Returns the music file name
	return m_MusicName;
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