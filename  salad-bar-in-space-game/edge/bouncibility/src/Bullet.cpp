#include "Common.h"
#include "GameIncludes.h"


void Bullet::load(){
}

void Bullet::update(){
	WorldEntity::update();

	//this->m_Physics_Body->setVelocity(this->velocity);

	ttl -= GameSystem::getSingleton().getDeltaMillis();
	if (ttl < 0){
		EntityManager::getSingleton().remove(this->id);
		return;
	}
	if (m_EnableMovement){
		//ai_script->callFunction(this);
	}


}



WorldEntity* Bullet::clone(){
	Bullet* entity = new Bullet();
	entity->location = this->location;


	if (m_Mesh){
		this->m_Mesh->grab();
	}
	entity->m_Mesh = this->m_Mesh;
	entity->m_Radius = this->m_Radius;
	entity->m_BulletRadius = this->m_BulletRadius;
	//Clone isn't working for the animated scene node.  Should just make from scratch.
	irr::scene::ISceneNode* node = LevelManager::getSceneManager()->addSphereSceneNode(this->m_BulletRadius);

	if (node){
		node->setScale(irr::core::vector3df(this->m_SceneNode->getScale()));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		
	}
	entity->m_SceneNode = node;

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

	irr::core::vector3df conn = myLoc - playerLoc;
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

	irr::core::vector3df conn = myLoc - loc;
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
	m_BulletRadius = 1;
	ttl = 10000;
}

Bullet::~Bullet(){
}