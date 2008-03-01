#include "Common.h"
#include "irrXML.h"	
#include "EntityManager.h"
#include "Level.h"
#include "LevelManager.h"

using namespace irr;
using namespace io;


LevelManager::LevelManager() : m_CurrentLevel(NULL), m_Smgr(NULL){

}

LevelManager::~LevelManager(){
	this->shutdown();
}

bool LevelManager::init(scene::ISceneManager* smgr, const std::string& XMLScenarioDefinition){

	m_Smgr = smgr;

	scenarioDefinition = XMLScenarioDefinition;

	EntityManager::getSingleton().init("res/entities/global.xml");

	IrrXMLReader* xml = createIrrXMLReader(XMLScenarioDefinition);

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
			break;
		}
	}
	m_LevelItr = m_Levels.begin();
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
	this->init(this->scenarioDefinition);
}

void LevelManager::repeat(){
	m_CurrentLevel->shutdown();

	delete m_CurrentLevel;

	m_CurrentLevel = new Level(*(m_LevelItr));

	m_CurrentLevel->init();
}

bool LevelManager::goToNext(){
	if (m_CurrentLevel){
		m_CurrentLevel->shutdown();

		delete m_CurrentLevel;
	}
	if (m_LevelItr != m_Levels.end()){
		m_CurrentLevel = new Level(*(++m_LevelItr));

		m_CurrentLevel->init();

		return true;
	}
	else{
		return false;
	}

}

void LevelManager::reset(const std::string& XMLScenarioDefinition){
	this->shutdown();
	this->init(XMLScenarioDefinitio);
}

Level& LevelManager::getCurrentLevel() const{
	return *m_CurrentLevel;
}

void LevelManager::draw(){
	m_CurrentLevel->draw();
}

void LevelManager::update(){
	//need to check if there needs to a level change.
	//isCompleted should return Level::WAITING_REPEAT or Level::FINISHED or Level::RUNNING or Level::WAITING_START
	m_CurrentLevel->update();

	int status = m_CurrentLevel->status();
	switch (status){
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
	}
}

