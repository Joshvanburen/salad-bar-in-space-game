#include "irrlicht.h"
#include "Common.h"
#include "irrnewt.hpp"
#include "LevelManager.h"
#include "EntityManager.h"
#include "GravshipHelper.h"
#include "Gravship.h"
#include "PhysicsManager.h"

void Gravship::load(){

}

void Gravship::update(){
	WorldEntity::update();


	//If gravity field is on, Apply force to any entities in our orbiting range
	if (this->m_GravityOn){
	}


	m_Helper->update();
	
}




WorldEntity* Gravship::clone(){
	Gravship* entity = new Gravship();
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
	entity->weight = this->weight;
	entity->attack = this->attack;
	entity->velocity = this->velocity;

	return entity;
}

void Gravship::enableGravityField(bool enabled){
	m_GravityOn = enabled;

	/*if (this->m_GravityOn){
		irr::core::line3d<irr::f32> line(this->m_Physics_Body->getPosition(),m_Helper->getPhysicsBody()->getPosition());

		irr::core::array<irr::newton::IBody*> orbiting_bodies;

		std::set<WorldEntity*> orbitingEntities = m_Helper->getOrbitingEntities();
		std::set<WorldEntity*>::iterator orbitingEntitiesItr = orbitingEntities.begin();

		for (; orbitingEntitiesItr != orbitingEntities.end(); orbitingEntitiesItr++){
			orbiting_bodies.push_back((*orbitingEntitiesItr)->getPhysicsBody());
		}

		irr::newton::SIntersectionPoint i_point = this->m_Physics_Body->getWorld()->getCollisionManager()->getCollisionFirstPointEx(line, orbiting_bodies);
		if (i_point.body){
			if (((WorldEntity*)(i_point.body->getUserData()))->getID() == m_Helper->getID()){
				m_Helper->enableGravityField(enabled);
			}
			else{
				//do something cool looking
			}
		}
	}
	else{*/
		m_Helper->enableGravityField(enabled);
	//}

}

void Gravship::changeState(const std::string name){
}

void Gravship::changeVelocity(float x_speed, float y_speed){
}

Gravship* Gravship::EntityToGravship(WorldEntity* entity){
	return dynamic_cast<Gravship*>(entity);
}

Gravship::Gravship() : m_GravityOn(false), m_Helper(NULL){

}

Gravship::~Gravship(){
}