#include "Common.h"
#include "irrXML.h"	
#include "irrlicht.h"
#include "ScriptManager.h"
#include "LevelManager.h"
#include "irrnewt.hpp"
#include "WorldEntity.h"
#include "PhysicsManager.h"

using namespace irr;
using namespace io;







Physics::WorldEntityCollisionCallback::WorldEntityCollisionCallback(const std::string& material1, const std::string& material2) : IMaterialCollisionCallback(), entity1(NULL), entity2(NULL), m_Material1(material1), m_Material2(material2){


}

Physics::WorldEntityCollisionCallback::WorldEntityCollisionCallback(const std::string& material1, const std::string& material2, Scripting::ScriptFunction* scriptFunction) : IMaterialCollisionCallback(), entity1(NULL), entity2(NULL), m_Material1(material1), m_Material2(material2){
	m_CollisionHandlerScripts.insert(scriptFunction);
}

Physics::WorldEntityCollisionCallback::~WorldEntityCollisionCallback(){

}

void Physics::WorldEntityCollisionCallback::ContactEnd(irr::newton::IMaterialPair* material_pair){

}

int  Physics::WorldEntityCollisionCallback::ContactBegin (irr::newton::IMaterialPair *material_pair, irr::newton::IBody *body0, irr::newton::IBody *body1){

	//Each bodies user data is assumed to hold a pointer to a WorldEntity object.  This must be casted from a void* pointer.

	entity1 = (WorldEntity*)body0->getUserData();

	entity2 = (WorldEntity*)body1->getUserData();

	return 1;
}

int  Physics::WorldEntityCollisionCallback::ContactProcess (irr::newton::IMaterialPairAndContact *material_pair_and_contact){

	//Save anything relevant about the points that are in contact.

	m_ScriptItrEnd = m_CollisionHandlerScripts.end();

	for (m_ScriptItr = m_CollisionHandlerScripts.begin(); m_ScriptItr != m_ScriptItrEnd; ++m_ScriptItr){
		((Scripting::MaterialCollisionFunction*)(*m_ScriptItr))->callFunction(this->entity1, this->entity2);
		//Call the script collision function with entity1 and entity2 pointers.
	}

	return 1;

}
void Physics::WorldEntityCollisionCallback::addHandler(Scripting::ScriptFunction* scriptFunction){
	Physics::ScriptList::iterator scriptItrEnd = m_CollisionHandlerScripts.end();
	Physics::ScriptList::iterator scriptItr;

	scriptItr = m_CollisionHandlerScripts.find(scriptFunction);

	if (scriptItr != scriptItrEnd){
		throw DuplicateScript();
	}

	m_CollisionHandlerScripts.insert(scriptFunction);
}


irr::newton::IMaterial* PhysicsManager::getMaterial(const std::string& name){
	//Find the entity in the ID map for this entityID
	m_MaterialItr = m_MaterialMap.find(name);
	//If the material wasn't found, throw exception
	if (m_MaterialItr == m_MaterialMap.end())
		throw Physics::MaterialDoesntExist();
	return m_MaterialItr->second;
}

void PhysicsManager::shutdown(){

	Physics::StrCollisionCallbackMap::iterator callbackItrEnd = m_CallbackMap.end();

	for(m_CallbackItr = m_CallbackMap.begin(); m_CallbackItr != callbackItrEnd; ++m_CallbackItr){
		delete (*m_CallbackItr).second;
	}

	m_CallbackMap.clear();

	m_MaterialMap.clear();
	if (m_World){
		m_World->destroyWorld();
		m_World = NULL;
	}
}
	
void PhysicsManager::update(){
	
	m_World->update();
	
}

void PhysicsManager::clear(){

	this->shutdown();

	this->init(m_Device);


}

PhysicsManager::PhysicsManager() : m_World(NULL), m_Gravity(0), m_Device(NULL){
	
}

PhysicsManager::~PhysicsManager(){
	this->shutdown();
}

bool PhysicsManager::init(irr::IrrlichtDevice* device){
	m_Device = device;
	m_World = irr::newton::createPhysicsWorld(device);

	

	Scripting::MaterialCollisionFunction* script_function = new Scripting::MaterialCollisionFunction();

	ScriptManager::getSingleton().registerScriptFunction("material_collision", script_function);

	m_MaterialDefinition = "./res/physics/global.xml";

	return readInXML(m_MaterialDefinition);


}

bool PhysicsManager::readInXML(const std::string& XMLMaterialDefinition){
	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLMaterialDefinition.c_str());

	if (!xml){
		std::cout << "There was an error loading the xml file " << XMLMaterialDefinition << ".\n";
	}

	// strings for storing the data we want to get out of the file
	std::string name;
	std::string material1;
	std::string material2;
	irr::newton::IMaterial* material;
	irr::newton::IMaterial* second_material;
	float softness = 0;
	float kinetic_friction = 0;
	float static_friction = 0;
	float elasticity = 0;
	std::string scriptName;
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case irr::io::EXN_TEXT:
			//No text nodes
			break;

		case irr::io::EXN_ELEMENT:
			if (!strcmp("material", xml->getNodeName())){
				name = xml->getAttributeValue("name");
				material = m_World->createMaterial();

				m_MaterialItr = m_MaterialMap.find(name);
				//If the material is already in the map, print out and continue;
				if (m_MaterialItr != m_MaterialMap.end()){
					std::cout << "Material with name: " << name << "already exists. Continuing...\n";
				}
				else{
					m_MaterialMap.insert(std::make_pair(name, material));
				}

			}
			else if (!strcmp("collision", xml->getNodeName())){
				material1 = xml->getAttributeValue("material1");
				material2 = xml->getAttributeValue("material2");
				static_friction = xml->getAttributeValueAsFloat("static_friction");
				kinetic_friction = xml->getAttributeValueAsFloat("kinetic_friction");
				softness = xml->getAttributeValueAsFloat("softness");
				elasticity = xml->getAttributeValueAsFloat("elasticity");
				material = this->getMaterial(material1);
				second_material = this->getMaterial(material2);
				material->setFriction(second_material, static_friction, kinetic_friction);
				material->setElasticity(second_material, elasticity);
				material->setSoftness(second_material, softness);
			}
			else if (!strcmp("callback", xml->getNodeName())){
				material1 = xml->getAttributeValue("material1");
				material2 = xml->getAttributeValue("material2");
				scriptName = xml->getAttributeValue("script");

				Scripting::Script& script = ScriptManager::getSingleton().getScript(scriptName);
				if (!script.isLoaded()){
					script.load();
				}
				Scripting::ScriptFunction* callbackFunction = script.addFunction("material_collision");
				
				this->addObserver(callbackFunction, material1, material2);
			}
			break;
		}

	}
	delete xml;

	return true;
}
bool PhysicsManager::addNewDefinitions(const std::string& XMLMaterialDefinition){

	return readInXML(XMLMaterialDefinition);

}

bool PhysicsManager::addObserver(Scripting::ScriptFunction* scriptFunction, const std::string& material1, const std::string& material2){

	std::string key;
	if (material1.compare(material2) <= 0){
		key = material1 + material2;
	}
	else{
		key = material2 + material1;
	}

	//Find the entity in the ID map for this entityID
	m_CallbackItr = m_CallbackMap.find(key);
	//If the Entity wasn't found, add new entry.
	if (m_CallbackItr == m_CallbackMap.end()){
		Physics::WorldEntityCollisionCallback* callback = new Physics::WorldEntityCollisionCallback(material1, material2, scriptFunction);
		irr::newton::IMaterial* material = this->getMaterial(material1);
		irr::newton::IMaterial* second_material = this->getMaterial(material2);
		material->setCollisionCallback(second_material, callback);
		m_CallbackMap.insert(std::make_pair(key, callback));

	}
	else{
		(*m_CallbackItr).second->addHandler(scriptFunction);
	}
	return true;
}
