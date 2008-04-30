#include "Common.h"
#include "GameIncludes.h"

using namespace irr;
using namespace io;

int EntityManager::Next_Available_ID = 1;

WorldEntity& Entity::SpawnerFactory::loadEntity(const std::string &XMLFilename){
	std::string name;
	int frequency = 0;
	int simul_spawn = 1;
	std::string entityName;
	float probability = 1.0;
	WorldEntity* entity;
	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLFilename.c_str());

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_TEXT:
			//No text nodes
			break;

		case EXN_ELEMENT:
			if (!strcmp("spawner", xml->getNodeName())){
				name = xml->getAttributeValue("name");
				frequency = xml->getAttributeValueAsInt("frequency");
				simul_spawn = xml->getAttributeValueAsInt("simultaneous_spawn");


				irr::scene::ISceneNode* node = LevelManager::getSceneManager()->addEmptySceneNode();
		
				

				if (node){
					node->setScale(irr::core::vector3df(1.0f, 1.0f, 1.0f));
					node->setMaterialFlag(irr::video::EMF_LIGHTING, false );
					node->setVisible(false);	
				}
				
				
			
				entity = new Spawner();
				
				((Spawner*)entity)->setFrequency(frequency);
				((Spawner*)entity)->setSimulSpawn(simul_spawn);
				
				entity->setSceneNode(node);
		

			//entity->changeState(startState)
							
			}
			else if(!strcmp("entity", xml->getNodeName())){
				entityName = xml->getAttributeValue("name");
				probability = xml->getAttributeValueAsFloat("probability");
				((Spawner*)entity)->addEntity(entityName, probability);
			}
			break;
		}
	}

	entity->load();
	return *entity;

}
WorldEntity& Entity::GravshipHelperFactory::loadEntity(const std::string& XMLFilename){

	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLFilename.c_str());

	// strings for storing the data we want to get out of the file
	std::string name;
	std::string meshFile;
	std::string textureFile;
	std::string startState;
	std::string materialName;

	irr::newton::IMaterial* material;

	float radius = 1;
	float orbitRadius = 1;
	float fieldRadius = 1;
	float gravityPull = 1;
	int mass = 1;
	float rotX = 0;
	float rotY = 0;
	float rotZ = 0;
	float maxOrbitSpeed = 1;
	float maxForce = 1;
	WorldEntity* entity = NULL;

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_TEXT:
			//No text nodes
			break;

		case EXN_ELEMENT:
			if (!strcmp("gravship_helper", xml->getNodeName())){
				name = xml->getAttributeValue("name");
				meshFile = xml->getAttributeValue("mesh");
				textureFile = xml->getAttributeValue("texture");
				//startState = xml->getAttributeValue("start_state");
				radius = xml->getAttributeValueAsFloat("radius");
				fieldRadius = xml->getAttributeValueAsFloat("field_radius");
				orbitRadius = xml->getAttributeValueAsFloat("orbit_radius");
				gravityPull = xml->getAttributeValueAsFloat("gravity_pull");
				mass = xml->getAttributeValueAsInt("mass");
				rotX = xml->getAttributeValueAsFloat("rotX");
				rotY = xml->getAttributeValueAsFloat("rotY");
				rotZ = xml->getAttributeValueAsFloat("rotZ");
				maxOrbitSpeed = xml->getAttributeValueAsFloat("max_orbit_speed");
				maxForce = xml->getAttributeValueAsFloat("max_gravity_force");
				irr::scene::ISceneNode* node = LevelManager::getSceneManager()->addSphereSceneNode(irr::f32(radius));
		
				

				if (node){
					node->setScale(irr::core::vector3df(1.0f, 1.0f, 1.0f));
					node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
					
				}
				
				
			
				entity = new GravshipHelper();
				
				entity->setMesh(NULL);
				entity->setRotation(irr::core::vector3df(rotX, rotY, rotZ));
				
				((GravshipHelper*)entity)->m_GravityFieldRadius = fieldRadius;
				((GravshipHelper*)entity)->m_OrbitRingRadius = orbitRadius;
				((GravshipHelper*)entity)->m_GravitationalPull = gravityPull;
				((GravshipHelper*)entity)->setMaxOrbitSpeed(maxOrbitSpeed);
				((GravshipHelper*)entity)->setMaxForce(maxForce);
				entity->setSceneNode(node);
				node->setMaterialTexture(0,LevelManager::getSingleton().getDriver()->getTexture(textureFile.c_str()));


				materialName = xml->getAttributeValue("material");
			
				
				material = PhysicsManager::getSingleton().getMaterial(materialName);
				

				
				irr::newton::SBodyFromNode physics_node;

				physics_node.Node = node;
				physics_node.Type = irr::newton::EBT_PRIMITIVE_ELLIPSOID;

				irr::newton::IBody* body = PhysicsManager::getSingleton().getPhysicsWorld()->createBody(physics_node);
				
				body->setContinuousCollisionMode(true);

				body->setMaterial(material);

				((GravshipHelper*)entity)->m_Material = material;


				body->setUserData(entity);

				//body->addForceContinuous(irr::core::vector3df(0,PhysicsManager::getSingleton().getGravity(),0 ));

				body->setMass(irr::f32(mass));

				entity->setPhysicsBody(body);
				

			
				
			

			//entity->changeState(startState)
							
			}
			break;
		}
	}

	entity->load();
	return *entity;
}

WorldEntity& Entity::GravshipFactory::loadEntity(const std::string& XMLFilename){

	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLFilename.c_str());

	// strings for storing the data we want to get out of the file
	std::string name;
	std::string meshFile;
	std::string textureFile;
	std::string startState;
	std::string materialName;
	std::string helper;
	irr::newton::IMaterial* material;
	float scale = 1;
	float radius = 1;
	int mass = 1;
	float rotX = 0;
	float rotY = 0;
	float rotZ = 0;

	WorldEntity* entity = NULL;

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_TEXT:
			//No text nodes
			break;

		case EXN_ELEMENT:
			if (!strcmp("gravship", xml->getNodeName())){
				name = xml->getAttributeValue("name");
				meshFile = xml->getAttributeValue("mesh");
				textureFile = xml->getAttributeValue("texture");
				//startState = xml->getAttributeValue("start_state");
				radius = xml->getAttributeValueAsFloat("radius");
				mass = xml->getAttributeValueAsInt("mass");
				helper = xml->getAttributeValue("helper");
				scale = xml->getAttributeValueAsFloat("scale");
								rotX = xml->getAttributeValueAsFloat("rotX");
				rotY = xml->getAttributeValueAsFloat("rotY");
				rotZ = xml->getAttributeValueAsFloat("rotZ");

				irr::scene::IAnimatedMesh* mesh = LevelManager::getSceneManager()->getMesh(meshFile.c_str());
				irr::scene::IAnimatedMeshSceneNode* node = LevelManager::getSceneManager()->addAnimatedMeshSceneNode(mesh);

				
			

				if (node){

					node->setScale(irr::core::vector3df(scale, scale, scale));
					node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
					
					irr::scene::ILightSceneNode* light = LevelManager::getSceneManager()->addLightSceneNode(node, irr::core::vector3d<float>(0.0f, 50.0f, 0.0f));
					irr::video::SLight lightParams;

					lightParams.Radius = 600.0f;
					lightParams.Direction = irr::core::vector3df(0.0, -10.0f, 0.0f);
					lightParams.Type = irr::video::ELT_SPOT;
					lightParams.InnerCone = 100.0f;
					lightParams.OuterCone = 180.0f;
					light->setLightData(lightParams);
					//lightParams.Falloff = 0.5f;
					
				}
				
				node->setAnimationSpeed(5);

				node->setFrameLoop(2, 21);
			
				entity = new Gravship();
				
				entity->setMesh(mesh);
				
				entity->setRotation(irr::core::vector3df(rotX, rotY, rotZ));
				entity->setSceneNode(node);
				//node->setMaterialTexture(0,LevelManager::getSingleton().getDriver()->getTexture(textureFile.c_str()));

				materialName = xml->getAttributeValue("material");

				material = PhysicsManager::getSingleton().getMaterial(materialName);

				((Gravship*)entity)->m_Helper = (GravshipHelper*)&(EntityManager::getSingleton().createEntity(helper));
				((Gravship*)entity)->m_Helper->setLocation(entity->getLocation().X, entity->getLocation().Y + 100.0f, entity->getLocation().Z);
				

				irr::newton::SBodyFromNode physics_node;
				physics_node.Mesh = mesh->getMesh(2);
				physics_node.Node = node;
				physics_node.Type = irr::newton::EBT_PRIMITIVE_BOX;

				irr::newton::IBody* body = PhysicsManager::getSingleton().getPhysicsWorld()->createBody(physics_node);
				
				body->setContinuousCollisionMode(false);

				body->setMaterial(material);

				body->setUserData(entity);
				

				body->addForceContinuous(irr::core::vector3df(0,0,PhysicsManager::getSingleton().getGravity() ));

				body->setMass(irr::f32(mass));
				entity->setPhysicsBody(body);
				material->setCollidable(PhysicsManager::getSingleton().getMaterial("level"), true);
				

			
				
			

			//entity->changeState(startState)
							
			}
			break;
		}
	}

	entity->load();
	return *entity;
}
WorldEntity& Entity::EnemyFactory::loadEntity(const std::string& XMLFilename){


	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLFilename.c_str());

	// strings for storing the data we want to get out of the file
	std::string name;
	std::string meshFile;
	std::string textureFile;
	std::string startState;
	std::string color;
	std::string ai_type;
	int physics_enabled = 0;
	int gravity_enabled = 0;
	std::string materialName;
	irr::newton::IMaterial* material;
	float radius = 1;
	float mass = 1;
	float scale = 1;
	float maxSpeed = 1;
		float rotX = 0;
	float rotY = 0;
	float rotZ = 0;
	Enemy* entity = NULL;

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_TEXT:
			//No text nodes
			break;

		case EXN_ELEMENT:
			if (!strcmp("enemy", xml->getNodeName())){
				name = xml->getAttributeValue("name");
				meshFile = xml->getAttributeValue("mesh");
				textureFile = xml->getAttributeValue("texture");
				//startState = xml->getAttributeValue("start_state");
				physics_enabled = xml->getAttributeValueAsInt("enable_physics");
				radius = xml->getAttributeValueAsFloat("radius");
				color = xml->getAttributeValue("color");
				ai_type = xml->getAttributeValue("ai");
				mass = xml->getAttributeValueAsFloat("mass");
				scale = xml->getAttributeValueAsFloat("scale");
				rotX = xml->getAttributeValueAsFloat("rotX");
				rotY = xml->getAttributeValueAsFloat("rotY");
				rotZ = xml->getAttributeValueAsFloat("rotZ");
				maxSpeed = xml->getAttributeValueAsFloat("max_speed");
				irr::scene::IAnimatedMesh* mesh = LevelManager::getSceneManager()->getMesh(meshFile.c_str());
				irr::scene::IAnimatedMeshSceneNode* node = LevelManager::getSceneManager()->addAnimatedMeshSceneNode(mesh);

				if (node){
					node->setScale(irr::core::vector3df(scale, scale, scale));
					node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
					
				}
				
				entity = new Enemy();
				
	
				entity->setRotation(irr::core::vector3df(rotX, rotY, rotZ));
				entity->setMesh(mesh);
				
				((Enemy*)entity)->setColor(color);
				((Enemy*)entity)->setRadius(radius);
				entity->setSceneNode(node);
				//node->setMaterialTexture(0,LevelManager::getSingleton().getDriver()->getTexture(textureFile.c_str()));
				if (physics_enabled){
					gravity_enabled = xml->getAttributeValueAsInt("enable_gravity");
					materialName = xml->getAttributeValue("material");


					material = PhysicsManager::getSingleton().getMaterial(materialName);


					irr::newton::SBodyFromNode physics_node;
					physics_node.Mesh = mesh->getMesh(2);
					physics_node.Node = node;
					physics_node.Type = irr::newton::EBT_PRIMITIVE_BOX;

					irr::newton::ICharacterController* body = PhysicsManager::getSingleton().getPhysicsWorld()->createCharacterController(PhysicsManager::getSingleton().getPhysicsWorld()->createBody(physics_node));
					body->setRotationUpdate(false);
					body->setContinuousCollisionMode(false);

					body->setAutoFreeze(true);
					body->setFreezeTreshold(1.0, 1.0, 10);
					body->setMaterial(material);

					body->setUserData(entity);

					body->setMass(mass);
					if (gravity_enabled){
						body->addForceContinuous(irr::core::vector3df(PhysicsManager::getSingleton().getGravity() ,0,0));
					}

					entity->setPhysicsBody(body);

			
				}

				Scripting::WorldEntityAIFunction* ai_script = ((Scripting::WorldEntityAIFunction*)(WorldEntityAIManager::getSingleton().getAI(ai_type)));

				entity->setAi(ai_script);

				entity->setSpeed(maxSpeed);

			//entity->changeState(startState)
							
			}
			break;
		}
	}


	return *entity;
}


WorldEntity& Entity::BulletFactory::loadEntity(const std::string& XMLFilename){


	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLFilename.c_str());

	// strings for storing the data we want to get out of the file
	std::string name;
	//std::string meshFile;
	std::string textureFile;
	//std::string startState;
	//std::string color;
	//std::string ai_type;
	//int physics_enabled = 0;
	//int gravity_enabled = 0;
	std::string materialName;
	irr::newton::IMaterial* material;
	float speed = 1;
	float mass = 1;
	//float scale = 1;
	//float maxSpeed = 1;
	//	float rotX = 0;
	//float rotY = 0;
	//float rotZ = 0;
	Bullet* entity = NULL;

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_TEXT:
			//No text nodes
			break;

		case EXN_ELEMENT:
			if (!strcmp("bullet", xml->getNodeName())){
				name = xml->getAttributeValue("name");
				//meshFile = xml->getAttributeValue("mesh");
				textureFile = xml->getAttributeValue("texture");
				materialName = xml->getAttributeValue("material");
				//physics_enabled = xml->getAttributeValueAsInt("enable_physics");
				//radius = xml->getAttributeValueAsFloat("radius");
				//color = xml->getAttributeValue("color");
				//ai_type = xml->getAttributeValue("ai");
				//mass = xml->getAttributeValueAsFloat("mass");
				speed = xml->getAttributeValueAsFloat("speed");
				//rotX = xml->getAttributeValueAsFloat("rotX");
				//rotY = xml->getAttributeValueAsFloat("rotY");
				//rotZ = xml->getAttributeValueAsFloat("rotZ");
				//maxSpeed = xml->getAttributeValueAsFloat("max_speed");

				irr::scene::ISceneNode* node = LevelManager::getSceneManager()->addSphereSceneNode(18);
				//irr::scene::IAnimatedMeshSceneNode* node = LevelManager::getSceneManager()->addAnimatedMeshSceneNode(mesh);

				//if (node){
				//	node->setScale(irr::core::vector3df(scale, scale, scale));
				//	node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
				//	
				//}
				
				entity = new Bullet();
				
				//entity->setRotation(irr::core::vector3df(rotX, rotY, rotZ));
				//entity->setMesh(mesh);
				//
				//((Enemy*)entity)->setColor(color);
				//((Enemy*)entity)->setRadius(radius);
				entity->setSceneNode(node);
				//node->setMaterialFlag();
				node->setMaterialTexture(0,LevelManager::getSingleton().getDriver()->getTexture(textureFile.c_str()));
				//if (physics_enabled){
					//gravity_enabled = xml->getAttributeValueAsInt("enable_gravity");
					


					material = PhysicsManager::getSingleton().getMaterial(materialName);


					irr::newton::SBodyFromNode physics_node;
					//physics_node.Mesh = mesh->getMesh(2);
					physics_node.Node = node;
					physics_node.Type = irr::newton::EBT_PRIMITIVE_ELLIPSOID;

					irr::newton::ICharacterController* body = PhysicsManager::getSingleton().getPhysicsWorld()->createCharacterController(PhysicsManager::getSingleton().getPhysicsWorld()->createBody(physics_node));
					body->setRotationUpdate(false);
					body->setContinuousCollisionMode(true);

					body->setMaterial(material);

					body->setUserData(entity);

					//body->setMass(mass);
					//if (gravity_enabled){
					//	body->addForceContinuous(irr::core::vector3df(0,PhysicsManager::getSingleton().getGravity(),0 ));
					//}

					entity->setPhysicsBody(body);

					
			
				//}

				//Scripting::WorldEntityAIFunction* ai_script = ((Scripting::WorldEntityAIFunction*)(WorldEntityAIManager::getSingleton().getAI(ai_type)));

				entity->setSpeed(speed);

			//entity->changeState(startState)
							
			}
			break;
		}
	}


	return *entity;
}


WorldEntity& Entity::ObstacleFactory::loadEntity(const std::string& XMLFilename){
	
	//Creates a pointer to the XML file reader
	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLFilename.c_str());

	//Strings for storing the data we want to get out of the file
	std::string name;
	std::string meshFile;
	std::string textureFile;
	std::string startState;
	std::string materialName;

	//Physics material
	irr::newton::IMaterial* material;

	//Radius of the object
	float radius = 1;
	
	//Attributes of the obstacle
	float width = 1;
	float height = 1;
	float depth = 1;
	bool destructable = false;
	
	//Pointer to the object
	WorldEntity* entity = NULL;

	//While loop that reads from the xml file
	while(xml && xml->read())
	{
		//Switch based on the node type
		switch(xml->getNodeType())
		{
			//If the node is text
			case EXN_TEXT:
				//No text nodes
				break;
			
			//If the node is an elemet
			case EXN_ELEMENT:
				//If the node is not named obstacle
				if (!strcmp("obstacle", xml->getNodeName()))
				{
					//Gets the name 
					name = xml->getAttributeValue("name");
					
					//Gets the location of the mesh file
					meshFile = xml->getAttributeValue("mesh");
					
					//Gets the location of the texture
					textureFile = xml->getAttributeValue("texture");
					
					//Gets the start state of the object
					
					//Gets the radius
					radius = 10;
					
					//Gets the width
					width = xml->getAttributeValueAsFloat("width");
					
					//Gets the height
					height = xml->getAttributeValueAsFloat("height");
					
					//Gets the depth
					depth = xml->getAttributeValueAsFloat("depth");
					
					//Gets the scene node
					irr::scene::ISceneNode* node = LevelManager::getSceneManager()->addSphereSceneNode(irr::f32(radius));
			
					//If the node exitst
					if (node)
					{
						//Sets the scale and material flag
						node->setScale(irr::core::vector3df(1.0f, 1.0f, 1.0f));
						node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
						
					}

					//Creates the entity
					entity = new Obstacle();
					
					//Sets the mesh of the entity
					entity->setMesh(NULL);
					
					//Sets the attributes of the obstacle
					((Obstacle*)entity)->height = height;
					((Obstacle*)entity)->width = width;
					((Obstacle*)entity)->depth = depth;
					((Obstacle*)entity)->destructable = destructable;

					//Sets the scene node
					entity->setSceneNode(node);
					
					//Sets the texture
					node->setMaterialTexture(0,LevelManager::getSingleton().getDriver()->getTexture(textureFile.c_str()));
					
					//Gets the material name
					materialName = xml->getAttributeValue("material");

					//Sets the physics material
					material = PhysicsManager::getSingleton().getMaterial(materialName);

					//Physics node
					irr::newton::SBodyFromNode physics_node;

					//Sets the physics node
					physics_node.Node = node;
					
					//Sets the type of node
					physics_node.Type = irr::newton::EBT_PRIMITIVE_ELLIPSOID;
					
					//Sets the physics body
					irr::newton::IBody* body = PhysicsManager::getSingleton().getPhysicsWorld()->createBody(physics_node);
					
					//Sets continuous collision on
					body->setContinuousCollisionMode(false);

					//Sets the material
					body->setMaterial(material);

					//Sets the user data
					body->setUserData(entity);

					//Adds gravity
					body->addForceContinuous(irr::core::vector3df(0,0,PhysicsManager::getSingleton().getGravity()));
					
					//Sets the mass
					body->setMass(15000);
					
					//Sets the physics body of the entity
					entity->setPhysicsBody(body);
								
				}
				
				//Breaks the case
				break;
		}
	}

	//Returns a pointer to the entity
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

	this->m_Loader.registerFactory("enemy", new Entity::EnemyFactory());
	this->m_Loader.registerFactory("gravship", new Entity::GravshipFactory());
	this->m_Loader.registerFactory("gravship_helper", new Entity::GravshipHelperFactory());
	this->m_Loader.registerFactory("obstacle", new Entity::ObstacleFactory());
	this->m_Loader.registerFactory("spawner", new Entity::SpawnerFactory());

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
							script = xml->getAttributeValue("script");
							std::cout << "loading script: " << script << "\n";
							
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

	m_EntityHandleIDMap.clear();
}

WorldEntity& EntityManager::createEntity(const std::string& name, const std::string& handle){
	//Find the Entity in the Str EntityInfo map for this entity name.
	Entity::StrEntityMap::iterator entityItr = m_EntityMap.find(name);
	//If the Entity wasn't found, return false
	if(entityItr == m_EntityMap.end()){
		std::cout << "entity \"" << name << "\" not found. throwing exception!\n";
		throw Entity::EntityCreationFailed();
	}
	WorldEntity* entity = &(m_Loader.loadEntity(entityItr->second));
	this->m_IdEntityMap.insert(std::make_pair(Next_Available_ID, entity));

	if (handle.length() > 0){
		this->m_EntityHandleIDMap.insert(std::make_pair(handle, Next_Available_ID));
	}
	entity->id = Next_Available_ID;
	Next_Available_ID++;
	return(*entity);
}



WorldEntity* EntityManager::claim(const int entityID){
	//Find the Entity in the ID map for this entityID.
	m_EntityItr = m_IdEntityMap.find(entityID);
	//If the Entity wasn't found, return null
	if(m_EntityItr == m_IdEntityMap.end())
		return NULL;
	WorldEntity* entity = (m_EntityItr)->second;
	m_IdEntityMap.erase(m_EntityItr);
	return entity;
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
	this->m_EntityHandleIDMap.clear();
}

WorldEntity* EntityManager::getEntity(const int entityID){
	//Find the entity in the ID map for this entityID
	m_EntityItr = m_IdEntityMap.find(entityID);
	//If the Entity wasn't found, return NULL
	if (m_EntityItr == m_IdEntityMap.end())
		throw Entity::EntityDoesntExist();
	return (m_EntityItr->second);
}

int EntityManager::getEntityID(const std::string& handle){
	//Find the ID in the handle map for this handle
	std::map<std::string, int>::iterator m_HandleItr = m_EntityHandleIDMap.find(handle);
	//If the Entity wasn't found, return -1;
	if (m_HandleItr == m_EntityHandleIDMap.end())
		return -1;
	return (m_HandleItr->second);
}

WorldEntity& EntityManager::cloneEntity(const int entityID){
	
	WorldEntity* newEntity = this->getEntity(entityID)->clone();

	this->m_IdEntityMap.insert(std::make_pair(Next_Available_ID, newEntity));
	newEntity->id = Next_Available_ID;
	Next_Available_ID++;
	return(*newEntity);
}

WorldEntity& EntityManager::cloneEntity(WorldEntity& entity){

	WorldEntity* newEntity = entity.clone();

	this->m_IdEntityMap.insert(std::make_pair(Next_Available_ID, newEntity));
	newEntity->id = Next_Available_ID;
	Next_Available_ID++;
	return(*newEntity);
}
