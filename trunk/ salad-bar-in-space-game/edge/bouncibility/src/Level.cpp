
#include "Common.h"
#include "GameIncludes.h"

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
	m_LevelDimensions.Height = 0;
	m_LevelDimensions.Width = 0;
	m_MinX = 0;
	m_MinY = 0;
	m_MaxX = 0;
	m_MaxY = 0;
	m_Camera = NULL;
	m_Material = NULL;
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
	
	m_Camera->drop();
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
	std::string materialName = "";

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
				m_LevelDimensions.Height = xml->getAttributeValueAsInt("height");
				m_LevelDimensions.Width = xml->getAttributeValueAsInt("width");
				PhysicsManager::getSingleton().setGravity(xml->getAttributeValueAsFloat("gravity"));
				materialName = xml->getAttributeValue("material");

			}
			else if (!strcmp("camera", xml->getNodeName())){
				float xloc, yloc, zloc, xlook, ylook, zlook;
				xloc = xml->getAttributeValueAsFloat("xloc");
				yloc = xml->getAttributeValueAsFloat("yloc");
				zloc = xml->getAttributeValueAsFloat("zloc");
				xlook = xml->getAttributeValueAsFloat("xlook");
				ylook = xml->getAttributeValueAsFloat("ylook");
				zlook = xml->getAttributeValueAsFloat("zlook");
				
				m_Camera = LevelManager::getSingleton().getSceneManager()->addCameraSceneNode(0, irr::core::vector3df(xloc, yloc, zloc), irr::core::vector3df(xlook, ylook, zlook));
				
				
			
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

				entity.setLocation((float)entityX,500,(float)entityY);
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

	m_SceneNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
	mapData.Node = this->m_SceneNode;
	
	mapData.TerrainLOD = 1;

	mapData.Type = newton::EBT_TREE_TERRAIN;

	m_Physics_Body = PhysicsManager::getSingleton().getPhysicsWorld()->createBody(mapData);

	m_Material = PhysicsManager::getSingleton().getMaterial(materialName);
	m_Physics_Body->setMaterial(m_Material);

	m_Music->play(true);

	m_MinY = 0;
	m_MaxY = m_LevelDimensions.Height;
	m_MinX = 0;
	m_MaxX = m_LevelDimensions.Width;

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