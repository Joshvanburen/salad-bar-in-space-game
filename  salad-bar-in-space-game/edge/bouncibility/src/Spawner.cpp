
#include "Common.h"
#include "GameIncludes.h"
#include "Spawner.h"

void Spawner::load(){
	
}

void Spawner::update(){
	WorldEntity::update();

	//If enough time has passed, spawn SimulSpawn number of Entities
	m_AccumulatedTime += GameSystem::getSingleton().getDeltaMillis();

	WorldEntity* entity;
	if (m_AccumulatedTime > m_Frequency){
		for(int i = 0; i < m_SimulSpawn; i++){
			WorldEntity* cloneableEntity = entities.get();
			entity = &EntityManager::getSingleton().cloneEntity(*cloneableEntity);
			entity->setLocation(this->getLocation());
			entity->setVisible(true);
			entity->getPhysicsBody()->setFreeze(false);
		}
		m_AccumulatedTime = 0;

	}
}

void Spawner::addEntity(const std::string& entityName, float probability){
	WorldEntity* entity = &EntityManager::getSingleton().createEntity(entityName);
	EntityManager::getSingleton().claim(entity->getID());
	entity->setLocation(-10000, -10000, -10000);
	entity->setVisible(true);
	entity->getPhysicsBody()->setFreeze(true);
	entities.put(entity, probability);
}





WorldEntity* Spawner::clone(){
	Spawner* entity = new Spawner();
	entity->location = this->location;
	entity->id = -1;
	entity->currentState = this->currentState;
	if (m_Mesh){
		this->m_Mesh->grab();
	}
	entity->m_Mesh = this->m_Mesh;
	entity->m_Radius = this->m_Radius;
	entity->m_SceneNode = this->m_SceneNode->clone();
	entity->m_Mass = this->m_Mass;

	irr::newton::SBodyFromNode physics_node;
	physics_node.Node = entity->m_SceneNode;
	physics_node.Type = irr::newton::EBT_PRIMITIVE_ELLIPSOID;

	irr::newton::ICharacterController* body = m_Physics_Body->getWorld()->createCharacterController(m_Physics_Body->getWorld()->createBody(physics_node));
	body->setRotationUpdate(true);
	body->setContinuousCollisionMode(true);

	body->setMaterial(m_Physics_Body->getMaterial());

	body->setUserData(entity);

	body->addForceContinuous(irr::core::vector3df(0,PhysicsManager::getSingleton().getGravity(),0));
	entity->m_Physics_Body = body;

	return entity;
}




Spawner* Spawner::EntityToSpawner(WorldEntity* entity){
	return dynamic_cast<Spawner*>(entity);
}

Spawner::Spawner() {
	m_Frequency = 1000;
	m_AccumulatedTime = 0;
}


void Spawner::changeState(const std::string name){

}
Spawner::~Spawner(){
	hat<WorldEntity*>::iterator entityItr = entities.begin();
	for (; entityItr != entities.end(); entityItr++){
		delete (*entityItr);
	}
	entities.clear();
}