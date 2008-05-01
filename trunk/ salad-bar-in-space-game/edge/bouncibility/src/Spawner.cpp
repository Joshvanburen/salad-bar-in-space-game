
#include "Common.h"
#include "GameIncludes.h"
#include "Spawner.h"

void Spawner::load(){
	
}

void Spawner::update(){
	WorldEntity::update();

	if (!m_Paused){
	//If enough time has passed, spawn SimulSpawn number of Entities
	m_AccumulatedTime += GameSystem::getSingleton().getDeltaMillis();

	//time left till next spawn
	int timeLeft = m_Frequency - m_AccumulatedTime;
	
	while(m_SpawnTimes.size() > 0 && m_SpawnTimes.back() > timeLeft){
		spawnEntity();
		m_SpawnTimes.pop_back();
		if(m_SpawnTimes.size() <= 0){
			break;
		}
	}

	//if random value between 0 and frequency is greater than time till next spawn then spawn
	if (m_AccumulatedTime > m_Frequency){
		irr::u32 uniform_rand_value = 0;
		for(int j = 0; j < m_SpawnTimes.size(); j++){
			spawnEntity();
		}
		m_SpawnTimes.clear();
		for(int i = 0; i < m_SimulSpawn; i++){
			uniform_rand_value  = (*m_RNG)();

			m_SpawnTimes.push_back(uniform_rand_value);

		}
		m_SpawnTimes.sort();
		m_AccumulatedTime = 0;
			
	}

	}
}

void Spawner::spawnEntity(){
	int xloc = (*m_RNG)();
	int yloc = (*m_RNG)();

	WorldEntity* entity;
	WorldEntity* cloneableEntity = m_Entities.get();
	entity = &EntityManager::getSingleton().cloneEntity(*cloneableEntity);
	entity->setLocation(this->getLocation().X, this->getLocation().Y, this->getLocation().Z);
	entity->setVisible(true);
	entity->getPhysicsBody()->setFreeze(false);

}
void Spawner::addEntity(const std::string& entityName, float probability){
	WorldEntity* entity = &EntityManager::getSingleton().createEntity(entityName);
	EntityManager::getSingleton().claim(entity->getID());
	entity->setLocation(-10000, -10000, -10000);
	entity->setVisible(true);
	entity->getPhysicsBody()->setFreeze(true);
	m_Entities.put(entity, probability);
}



void Spawner::setFrequency(int frequency){
	m_Frequency = frequency;
	UniformDistribution distribution(0, m_Frequency);

	delete m_RNG;
	m_RNG = new boost::variate_generator<RNGEngine&, UniformDistribution>(m_RandomEngine, distribution);

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

Spawner::Spawner()  : WorldEntity(){
	m_Frequency = 1000;
	m_AccumulatedTime = 0;
	UniformDistribution distribution(0, m_Frequency);

	m_RNG = new boost::variate_generator<RNGEngine&, UniformDistribution>(m_RandomEngine, distribution);

	m_Paused = false;
}


void Spawner::changeState(const std::string name){

}
Spawner::~Spawner(){
	hat<WorldEntity*>::iterator entityItr = m_Entities.begin();
/*	for (; entityItr != m_Entities.end(); entityItr++){
		delete (*entityItr);
	}
	m_Entities.clear();

	m_SpawnTimes.clear();*/
	delete m_RNG;
}