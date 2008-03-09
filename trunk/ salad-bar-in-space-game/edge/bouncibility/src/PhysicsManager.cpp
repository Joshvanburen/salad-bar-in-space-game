#include "Common.h"
#include "irrXML.h"	
#include "irrlicht.h"
#include "LevelManager.h"
#include "irrnewt.hpp"
#include "WorldEntity.h"
#include "PhysicsManager.h"

using namespace irr;
using namespace io;


WorldEntity& Entity::BallFactory::loadEntity(const std::string& XMLFilename){

	std::cout << "in ball factory opening xml file " << XMLFilename << "\n";

	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLFilename.c_str());

	// strings for storing the data we want to get out of the file
	std::string name;
	std::string meshFile;
	std::string textureFile;
	std::string startState;
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_TEXT:
			//No text nodes
			break;

		case EXN_ELEMENT:
			if (!strcmp("ball", xml->getNodeName())){
				name = xml->getAttributeValue("name");
				meshFile = xml->getAttributeValue("mesh");
				textureFile = xml->getAttributeValue("texture");
				startState = xml->getAttributeValue("start_state");
			}
			break;
		}
	}
	irr::scene::ISceneManager* smgr = LevelManager::getSceneManager();
	//irr::scene::IAnimatedMesh* mesh = smgr->getMesh(meshFile.c_str());	
	//irr::scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	irr::scene::ISceneNode* node = smgr->addSphereSceneNode(22);
	if (node)
	{
		node->setScale(core::vector3df(1.0f,1.0f,1.0f));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		//node->setMD2Animation ( irr::scene::EMAT_STAND );
		//node->setMaterialTexture( 0, LevelManager::getDriver()->getTexture(textureFile.c_str()) );
	}
	node->setPosition(core::vector3df(-150,50,0));

	WorldEntity* entity = new Ball();

	//create physics for ball


	entity->setMesh(NULL);
	irr::newton::IMaterial* material = LevelManager::getPhysicsWorld()->createMaterial();
	entity->setBodyMaterial(material);
	irr::newton::SBodyFromNode physics_node;
	physics_node.Node = node;
	physics_node.Type = irr::newton::EBT_PRIMITIVE_ELLIPSOID;

	irr::newton::ICharacterController* body = LevelManager::getPhysicsWorld()->createCharacterController(LevelManager::getPhysicsWorld()->createBody(physics_node));
	body->setRotationUpdate(true);
	body->setContinuousCollisionMode(true);

	body->setMaterial(material);

	
	body->addForceContinuous(irr::core::vector3df(0,-2.0f,0));

	entity->setPhysicsBody(body);
	entity->setSceneNode(node);

	//entity->changeState(startState)
	return *entity;
}

Entity::EntityFactory::EntityFactory(){
}

Entity::EntityFactory::~EntityFactory(){
}

Entity::Loader::Loader(){
}

Entity::Loader::~Loader(){
	removeAll();
}
	
bool Entity::Loader::remove(const std::string& type){
	//Find the Factory in the map for this type.
	Entity::TypeFactoryMap::iterator factoryItr = m_FactoryMap.find(type);
	//If the type isn't already in the map, return false
	if(factoryItr == m_FactoryMap.end()){
		return false;
	}
	delete factoryItr->second;
	m_FactoryMap.erase(factoryItr);
	return true;
}

void Entity::Loader::removeAll(){
	Entity::TypeFactoryMap::iterator factoryItr;
	Entity::TypeFactoryMap::iterator factoryItrEnd = m_FactoryMap.end();

	for(factoryItr = m_FactoryMap.begin(); factoryItr != factoryItrEnd; ++factoryItr)
	{
		delete factoryItr->second;
	}
	m_FactoryMap.clear();
}

Entity::EntityFactory* Entity::Loader::getFactory(const std::string& type){
	//Find the Factory in the map for this type.
	Entity::TypeFactoryMap::iterator factoryItr = m_FactoryMap.find(type);
	//If the type isn't already in the map, insert and return true
	if(factoryItr == m_FactoryMap.end()){
		return NULL;
	}
	return factoryItr->second;
}


WorldEntity& Entity::Loader::loadEntity(EntityInfo* entityInfo){
	std::cout << "calling factory for " << entityInfo->m_Name << " with xml file " << entityInfo->m_XMLFile << "\n";
	std::cout.flush();
	return entityInfo->mp_Factory->loadEntity(entityInfo->m_XMLFile);
}



bool Entity::Loader::registerFactory(const std::string& type, EntityFactory* factory){
	//Find the Factory in the map for this type.
	Entity::TypeFactoryMap::iterator factoryItr = m_FactoryMap.find(type);
	//If the type isn't already in the map, insert and return true
	if(factoryItr == m_FactoryMap.end()){
		m_FactoryMap.insert(make_pair(type, factory));
		return true;
	}
	//Type was already associated with some factory
	return false;
}
	
void Entity::EntityInfo::destroy(){
}

Entity::EntityInfo::EntityInfo(const std::string& sName, const std::string& sXMLFile,  EntityFactory* p_Factory) : m_Name(sName), m_XMLFile(sXMLFile), mp_Factory(p_Factory){


}

Entity::EntityInfo::~EntityInfo(){
}

const std::string& Entity::EntityInfo::getName() const{
	return m_Name;
}

std::ostream& operator << (std::ostream& os, const Entity::EntityInfo& entity){
	return os << entity.getName();
}


int EntityManager::next_ID = 1;


EntityManager::EntityManager(){

}

EntityManager::~EntityManager(){
	this->shutdown();
}

bool EntityManager::addNewDefinitions(const std::string& XMLEntityDefinition){
	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLEntityDefinition.c_str());

	// strings for storing the data we want to get out of the file
	std::string name;
	std::string xmlFile;
	std::string type;

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case irr::io::EXN_TEXT:
			//No text nodes
			break;

		case irr::io::EXN_ELEMENT:
			if (!strcmp("entity", xml->getNodeName())){
				name = xml->getAttributeValue("name");
				xmlFile = xml->getAttributeValue("file");
				type= xml->getAttributeValue("type");
			}
			break;
		}
		Entity::EntityFactory* factory = m_Loader.getFactory(type);
		if (factory){
			Entity::EntityInfo* entityInfo = new Entity::EntityInfo(name, xmlFile, factory);
				if ((m_EntityMap.insert(make_pair(name, entityInfo))).second){

				}
				else{
					m_EntityMap.clear();
					return false;
				}
		}
		else{
			return false;
		}
	}
	delete xml;
	return true;
}

bool EntityManager::init(const std::string& XMLEntityDefinition){
	std::cout << "loading xml file with name " << XMLEntityDefinition << "\n";
	//Should register all the factories to each type here!!!!

	this->m_Loader.registerFactory("ball", new Entity::BallFactory());

	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLEntityDefinition.c_str());

	if (!xml){
		std::cout << "There was an error loading the xml file " << XMLEntityDefinition << ".\n";
	}
	// strings for storing the data we want to get out of the file
	std::string name;
	std::string xmlFile;
	std::string type;

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case irr::io::EXN_TEXT:
			//No text nodes
			break;

		case irr::io::EXN_ELEMENT:
			if (!strcmp("entity", xml->getNodeName())){
				Entity::EntityFactory* factory = m_Loader.getFactory(xml->getAttributeValue("type"));
				if (factory){
					Entity::EntityInfo* entityInfo = new Entity::EntityInfo(xml->getAttributeValue("name"), xml->getAttributeValue("file"), factory);
					if ((m_EntityMap.insert(make_pair(std::string(xml->getAttributeValue("name")), entityInfo))).second){
						std::cout << "added entity with name " << std::string(xml->getAttributeValue("name")) << "\n";

						}
						else{
							m_EntityMap.clear();
							return false;
						}
				}
				else{
					m_EntityMap.clear();
					return false;
				}
					}
					break;
				}

	}
	delete xml;
	return true;
}

void EntityManager::shutdown(){
	//Remove all instantiated entities
	removeAll();
	//Remove all EntityInfo
	Entity::StrEntityMap::iterator entityItrEnd = m_EntityMap.end();
	Entity::StrEntityMap::iterator entityItr;
	for(entityItr = m_EntityMap.begin(); entityItr != entityItrEnd; ++entityItr)
	{
		entityItr->second->destroy();
		delete entityItr->second;
	}

	//Remove everything inside the loader
	m_Loader.removeAll();

	m_EntityMap.clear();
}

WorldEntity& EntityManager::createEntity(const std::string name){
	//Find the Entity in the Str EntityInfo map for this entity name.
	std::cout << "finding entity with name " << name << "\n";
	Entity::StrEntityMap::iterator entityItr = m_EntityMap.find(name);
	//If the Entity wasn't found, return false
	if(entityItr == m_EntityMap.end()){
		std::cout << "entity not found. throwing exception!\n";
		throw Entity::EntityCreationFailed();
	}
	return(m_Loader.loadEntity(entityItr->second));
}

bool EntityManager::remove(const int entityID){
	//Find the Entity in the ID map for this entityID.
	m_EntityItr = m_IdEntityMap.find(entityID);
	//If the Entity wasn't found, return false
	if(m_EntityItr == m_IdEntityMap.end())
		return false;

	delete m_EntityItr->second;
	m_IdEntityMap.erase(m_EntityItr);
	return true;
}

void EntityManager::removeAll(){
	Entity::IdEntityMap::iterator entityItrEnd = m_IdEntityMap.end();
	for(m_EntityItr = m_IdEntityMap.begin(); m_EntityItr != entityItrEnd; ++m_EntityItr)
	{
		delete m_EntityItr->second;
	}
	m_IdEntityMap.clear();
}

WorldEntity& EntityManager::getEntity(const int entityID){
	//Find the entity in the ID map for this entityID
	m_EntityItr = m_IdEntityMap.find(entityID);
	//If the Entity wasn't found, return NULL
	if (m_EntityItr == m_IdEntityMap.end())
		throw Entity::EntityDoesntExist();
	return *(m_EntityItr->second);
}

irr::newton::IMaterial* PhysicsManager::getMaterial(const std::string& name);
