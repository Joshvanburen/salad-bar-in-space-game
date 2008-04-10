#include "Common.h"
#include "irrXML.h"	
#include "EntityManager.h"
#include "irrlicht.h"
#include "Level.h"
#include "irrnewt.hpp"
#include "PhysicsManager.h"
#include "SoundManager.h"
#include "GameSystem.h"
#include "ScriptManager.h"
#include "LevelManager.h"


using namespace irr;
using namespace video;
using namespace io;

irr::scene::ISceneManager* LevelManager::s_Smgr = NULL;
irr::IrrlichtDevice* LevelManager::s_Device = NULL;
irr::video::IVideoDriver* LevelManager::s_Driver = NULL;

LevelManager::LevelManager() : m_CurrentLevel(NULL){

}

LevelManager::~LevelManager(){
	this->shutdown();
}

bool LevelManager::startGame(){
	m_CurrentLevel = new Level();


	m_LevelItr = m_Levels.begin();

	m_CurrentLevel->load(*(m_LevelItr));

	GameSystem::getSingleton().startGame();

	return true;
}

bool LevelManager::init(irr::IrrlichtDevice* device, const std::string& XMLScenarioDefinition){

	s_Device = device;

	s_Smgr = device->getSceneManager();

	s_Driver = device->getVideoDriver();




	scenarioDefinition =  XMLScenarioDefinition;
	EntityManager::getSingleton().init("./res/entities/global.xml");

	IrrXMLReader* xml = createIrrXMLReader(XMLScenarioDefinition.c_str());


	if (!xml){
		std::cout << "There was an error loading the xml file " << XMLScenarioDefinition << ".\n";
	}

	// strings for storing the data we want to get out of the file
	std::string xmlFile;

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_TEXT:
			//No text nodes
			break;

		case EXN_ELEMENT:
			if (!strcmp("entities", xml->getNodeName())){
				xmlFile = xml->getAttributeValue("file");
				EntityManager::getSingleton().addNewDefinitions(xmlFile);
			}
			if (!strcmp("level", xml->getNodeName())){
				xmlFile = xml->getAttributeValue("file");
				m_Levels.push_back(xmlFile);
			}
			if (!strcmp("physics", xml->getNodeName())){
				xmlFile = xml->getAttributeValue("file");
				PhysicsManager::getSingleton().addNewDefinitions(xmlFile);
			}
			if (!strcmp("scripts", xml->getNodeName())){
				ScriptManager::getSingleton().addNewDefinitions(xml->getAttributeValue("file"));
			}
			if (!strcmp("sounds", xml->getNodeName())){
				SoundManager::getSingleton().addNewSounds(xml->getAttributeValue("file"));
			}
			break;
		}
	}
	m_LevelItr = m_Levels.begin();



	delete xml;
	return true;
}

void LevelManager::shutdown(){
	//shutdown  and delete current level
	m_CurrentLevel->shutdown();

	if (m_CurrentLevel){
		delete m_CurrentLevel;
	}

	m_CurrentLevel = NULL;

	m_Levels.clear();
}



void LevelManager::reset(){
	this->shutdown();
	this->init(s_Device, this->scenarioDefinition);
}

void LevelManager::repeat(){
	m_CurrentLevel->shutdown();

	delete m_CurrentLevel;

	m_CurrentLevel = new Level();

	m_CurrentLevel->load(*(m_LevelItr));

	GameSystem::getSingleton().recoverAfterLevelChange();
}

bool LevelManager::goToNext(){
	if (m_CurrentLevel){
		m_CurrentLevel->shutdown();

		delete m_CurrentLevel;
	}
	if (m_LevelItr != m_Levels.end()){
		m_CurrentLevel = new Level();

		m_CurrentLevel->load(*(++m_LevelItr));

		return true;
		GameSystem::getSingleton().recoverAfterLevelChange();
	}
	else{
		return false;
	}

}

void LevelManager::reset(const std::string& XMLScenarioDefinition){
	this->shutdown();
	this->init(s_Device, XMLScenarioDefinition);
}

Level& LevelManager::getCurrentLevel() const{
	return *m_CurrentLevel;
}


void LevelManager::update(){
	//need to check if there needs to a level change.
	//isCompleted should return Level::WAITING_REPEAT or Level::FINISHED or Level::RUNNING or Level::WAITING_START
	m_CurrentLevel->update();

	int status = m_CurrentLevel->status();
	/*switch (status){
		case Level::WAITING_REPEAT:
			this->repeat();
			break;
		case Level::WAITING_START:
			//Do nothing for now
			break;
		case Level::FINISHED:
			//Need to add something if there is no next.
			this->goToNext();
			break;
		case Level::RUNNING:
			//Do nothing for now
			break;
	}*/
}

