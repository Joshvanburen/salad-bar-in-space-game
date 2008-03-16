#include "Common.h"
#include "irrXML.h"	
#include "irrlicht.h"
#include ".\EntityManager.h"
#include "LevelManager.h"
#include "irrnewt.hpp"
#include "ScriptManager.h"
#include "PhysicsManager.h"
#include "ball.h"
#include "WorldEntity.h"

using namespace irr;
using namespace io;


WorldEntity& Entity::BallFactory::loadEntity(const std::string& XMLFilename){


	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLFilename.c_str());

	// strings for storing the data we want to get out of the file
	std::string name;
	std::string meshFile;
	std::string textureFile;
	std::string startState;

	int physics_enabled = 0;
	int gravity_enabled = 0;
	std::string materialName;
	irr::newton::IMaterial* material;
	float radius = 1;
	WorldEntity* entity = NULL;

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
				//startState = xml->getAttributeValue("start_state");
				physics_enabled = xml->getAttributeValueAsInt("enable_physics");
				radius = xml->getAttributeValueAsFloat("radius");

				irr::scene::ISceneNode* node = LevelManager::getSceneManager()->addSphereSceneNode(irr::f32(radius));

				if (node){
					node->setScale(irr::core::vector3df(1.0f, 1.0f, 1.0f));
					node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
				}

				entity = new Ball();

				entity->setMesh(NULL);

				entity->setSceneNode(node);
				node->setMaterialTexture(0,LevelManager::getSingleton().getDriver()->getTexture(textureFile.c_str()));
				if (physics_enabled){
					gravity_enabled = xml->getAttributeValueAsInt("enable_gravity");
					materialName = xml->getAttributeValue("material");


					material = PhysicsManager::getSingleton().getMaterial(materialName);


					irr::newton::SBodyFromNode physics_node;
					physics_node.Node = node;
					physics_node.Type = irr::newton::EBT_PRIMITIVE_ELLIPSOID;

					irr::newton::ICharacterController* body = PhysicsManager::getSingleton().getPhysicsWorld()->createCharacterController(PhysicsManager::getSingleton().getPhysicsWorld()->createBody(physics_node));
					body->setRotationUpdate(true);
					body->setContinuousCollisionMode(true);

					body->setMaterial(material);

					body->setUserData(entity);

					if (gravity_enabled){
						body->addForceContinuous(irr::core::vector3df(0,PhysicsManager::getSingleton().getGravity(),0));
					}

					entity->setPhysicsBody(body);

			
				}
	

			//entity->changeState(startState)
							
			}
			break;
		}
	}


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
	std::string script;

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
	std::string script;

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
						if (xml->getAttributeValue("script")){
							std::cout << "loading script.\n";
							script = xml->getAttributeValue("script");
							ScriptManager::getSingleton().loadScript(script);
						}
						
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
