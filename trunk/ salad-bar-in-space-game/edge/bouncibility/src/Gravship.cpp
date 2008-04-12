#include "irrlicht.h"
#include "Common.h"
#include "irrnewt.hpp"
#include "LevelManager.h"
#include "EntityManager.h"
#include "Gravship.h"
#include "PhysicsManager.h"

void Gravship::load(){

}

void Gravship::update(){
	const irr::core::vector3df position  = this->m_Physics_Body->getPosition();
	this->m_Physics_Body->setPosition(irr::core::vector3df(position.X,position.Y, 0));
	//this->m_Physics_Body->setVelocity(this->velocity);

	//If gravity field is on, Apply force to any entities in our orbiting range
	if (this->m_GravityOn){
	}


	
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