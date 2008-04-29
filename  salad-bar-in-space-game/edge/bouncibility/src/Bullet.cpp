#include "Common.h"
#include "GameIncludes.h"


void Bullet::load(){

}

void Bullet::update(){
	WorldEntity::update();

	//this->m_Physics_Body->setVelocity(this->velocity);

	if (m_EnableMovement){
		//ai_script->callFunction(this);
	}


}

WorldEntity* Bullet::clone(){
	Bullet* entity = new Bullet();
	entity->location = this->location;
	entity->id = -1;
	//entity->currentState = this->currentState;
	if (m_Mesh){
		this->m_Mesh->grab();
	}
	entity->m_Mesh = this->m_Mesh;
	//entity->m_Radius = this->m_Radius;
	entity->m_SceneNode = this->m_SceneNode->clone();

	irr::newton::SBodyFromNode physics_node;
	physics_node.Node = entity->m_SceneNode;
	physics_node.Type = irr::newton::EBT_PRIMITIVE_ELLIPSOID;

	irr::newton::ICharacterController* body = m_Physics_Body->getWorld()->createCharacterController(m_Physics_Body->getWorld()->createBody(physics_node));
	body->setRotationUpdate(false);
	body->setContinuousCollisionMode(false);

	body->setMaterial(m_Physics_Body->getMaterial());

	body->setUserData(entity);

	body->addForceContinuous(irr::core::vector3df(0,PhysicsManager::getSingleton().getGravity(),0));
	entity->m_Physics_Body = body;
	//entity->color = this->color;
	//entity->weight = this->weight;
	//entity->attack = this->attack;
	//entity->bouncibility = this->bouncibility;
	//entity->radius = this->radius;
	entity->velocity = this->velocity;

		moveToPlayer();


	return entity;
}


void Bullet::changeVelocity(float x_speed, float y_speed){
}

Bullet* Bullet::EntityToBullet(WorldEntity* entity){
	return dynamic_cast<Bullet*>(entity);
}



void Bullet::moveToPlayer(){

	const irr::core::vector3df playerLoc = GameSystem::getSingleton().getGravship()->getSceneNode()->getPosition();
	const irr::core::vector3df myLoc = this->getSceneNode()->getPosition();

	irr::core::vector3df conn = playerLoc - myLoc;
	conn = conn.normalize();

	//irr::core::vector3df vel = this->m_Physics_Body->getVelocity();
	//float spd = 75;
	conn = conn * speed;


	this->m_Physics_Body->addForce(conn);

	irr::core::vector3df velocity = this->m_Physics_Body->getVelocity();
	if (velocity.getLengthSQ() > m_MaxSpeedSQ){
		this->m_Physics_Body->setVelocity(velocity.normalize() * m_MaxSpeed);
	}
}


void Bullet::moveTo(irr::core::vector3df loc) {
	
	const irr::core::vector3df myLoc = this->getSceneNode()->getPosition();

	irr::core::vector3df conn = loc - myLoc;
	conn = conn.normalize();

	//irr::core::vector3df vel = this->m_Physics_Body->getVelocity();
	//float spd = 75;
	conn = conn * speed;


	this->m_Physics_Body->addForce(conn);

	irr::core::vector3df velocity = this->m_Physics_Body->getVelocity();
	if (velocity.getLengthSQ() > m_MaxSpeedSQ){
		this->m_Physics_Body->setVelocity(velocity.normalize() * m_MaxSpeed);
	}

}



//void Bullet::moveRandomly(){
//
//	irr::core::vector3df dirct = 
//
//}
//
Bullet::Bullet() : WorldEntity(){

}

Bullet::~Bullet(){
}