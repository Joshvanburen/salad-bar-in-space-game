#include "Common.h"
#include "irrXML.h"	
#include ".\EntityManager.h"
#include "WorldEntity.h"

using namespace irr;
using namespace io;

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
		return false
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
		delete m_EntityItr->second;
	}
	m_FactoryMap.clear();
}

EntityFactory* Entity::Loader::getFactory(const std::string& type){
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
	
Entity::EntityInfo::destroy(){
}

Entity::EntityInfo::EntityInfo(const std::string& sName, const std::string& sType, const std::string& sXMLFile) : m_Name(sName), m_XMLFile(sXMLFile), mp_Factory(p_Factory){
}

Entity::EntityInfo::~EntityInfo(){
}

const std::string& Entity::EntityInfo::getName() const{
	return sName;
}

std::ostream& operator << (std::ostream& os, const Entity::Loader& loader){
	Entity::TypeFactoryMap::iterator factoryItr;
	Entity::TypeFactoryMap::iterator factoryItrEnd = loader.m_FactoryMap.end();
	
	os << "Have a factory for the following types:\n";
	for(factoryItr = loader.m_FactoryMap.begin(); factoryItr != factoryItrEnd; ++factoryItr)
	{
		os << factoryItr->first << "\n";
	}
	return os;
}

std::ostream& operator << (std::ostream& os, const Entity::Entity& entity){
	return os << entity.getName();
}


int EntityManager::next_ID = 1;


EntityManager::EntityManager(){

}

EntityManager::~EntityManager(){
	this->shutdown();
}

bool EntityManager::init(const std::string& XMLEntityDefinition){

	//Should register all the factories to each type here!!!!


	IrrXMLReader* xml = createIrrXMLReader(XMLEntityDefinition);

	// strings for storing the data we want to get out of the file
	std::string name;
	std::string xmlFile;
	std::string type;

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_TEXT:
			//No text nodes
			break;

		case EXN_ELEMENT:
			if (!strcmp("entity", xml->getNodeName())){
				name = xml->getAttributeValue("name");
				xmlFile = xml->getAttributeValue("file");
				type= xml->getAttributeValue("type");
			}
			break;
		}
		Entity::EntityFactory* factory = m_Loader.getFactory(type);
		if (factory){
			Entity::EntityInfo* entityInfo = new Entity::EntityInfo(name, xmlFile, factory)
				if ((m_EntityMap.insert(make_pair(name, entityInfo))).second){

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
}

WorldEntity& EntityManager::createEntity(const std::string name){
	//Find the Entity in the Str EntityInfo map for this entity name.
	Entity::StrEntityMap::iterator entityItr = m_EntityMap.find(name);
	//If the Entity wasn't found, return false
	if(entityItr == m_EntityMap.end())
		return NULL;
	
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
}

WorldEntity& EntityManager::getEntity(const int entityID) const{
	//Find the entity in the ID map for this entityID
	m_EntityItr = m_IdEntityMap.find(entityID);
	//If the Entity wasn't found, return NULL
	if (m_EntityItr == m_IdEntityMap.end())
		return NULL;
	return m_EntityItr->second;
}
