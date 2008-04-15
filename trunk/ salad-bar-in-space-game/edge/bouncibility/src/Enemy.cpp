#include "irrlicht.h"
#include "irrnewt.hpp"
#include "LevelManager.h"
#include "Enemy.h"
#include "PhysicsManager.h"
#include "WorldEntityAIManager.h"

void Enemy::load(){

}

void Enemy::update(){
	const irr::core::vector3df position  = this->m_Physics_Body->getPosition();

	this->m_Physics_Body->setPosition(irr::core::vector3df(position.X,position.Y, 0));
	//this->m_Physics_Body->setVelocity(this->velocity);

	ai_script->callFunction(this);
}

WorldEntity* Enemy::clone(){
	Enemy* entity = new Enemy();
	entity->location = this->location;
	entity->id = -1;
	entity->currentState = this->currentState;
	if (m_Mesh){
		this->m_Mesh->grab();
	}
	entity->m_Mesh = this->m_Mesh;
	entity->m_Radius = this->m_Radius;
	entity->m_SceneNode = this->m_SceneNode->clone();

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
	entity->color = this->color;
	entity->weight = this->weight;
	entity->attack = this->attack;
	entity->bouncibility = this->bouncibility;
	entity->radius = this->radius;
	entity->velocity = this->velocity;

	return entity;
}

void Enemy::changeState(const std::string name){
}

void Enemy::changeVelocity(float x_speed, float y_speed){
}

Enemy* Enemy::EntityToEnemy(WorldEntity* entity){
	return dynamic_cast<Enemy*>(entity);
}

void Enemy::jump(int jump_type){

	irr::core::aabbox3d<irr::f32> box = this->m_Physics_Body->calculateAABB();

	irr::core::line3d<irr::f32> line(box.MinEdge,irr::core::vector3df());
	line.end = line.start;
	line.start.Y += .1f;
	line.end.Y -= 999999.9f;

	irr::newton::SIntersectionPoint i_point=
		this->m_Physics_Body->getWorld()->getCollisionManager()->getCollisionFirstPointEx(line);
	std::cout << "before checking if the point is null\n";
	if(i_point.body!= NULL) {

		irr::f32 distance = i_point.point.getDistanceFrom(line.start);
		std::cout << "before checking against distance which is " << distance << "\n";
		if(distance <= 6) {

			irr::core::vector3df net_applied_force = this->m_Physics_Body->getNetAppliedForce();

			/*
			//DEBUG
			core::stringc m;
			m+=(double)net_applied_force.Y;
			this->getWorld()->getIrrlichtDevice()->getLogger()->log(m.c_str());
			*/

			this->m_Physics_Body->getWorld()->getUtils()->round(net_applied_force,1);

			/*
			//DEBUG
			m="";
			m+=(double)net_applied_force.Y;
			this->getWorld()->getIrrlichtDevice()->getLogger()->log(m.c_str());
			*/
			std::cout << net_applied_force.Y << "\n";
			//if(net_applied_force.Y == 0.0f) {
				//can jump!!
				irr::core::vector3df vel = this->m_Physics_Body->getVelocity();
				vel.Y = 3.0f;
				this->m_Physics_Body->setVelocity(vel);			
				
			//}

		}//distance
	}//body!=NULL
}

void Enemy::setAi(Scripting::WorldEntityAIFunction* newAIScript){
					
	ai_script = newAIScript;

}

void Enemy::moveToPlayer(){

	const irr::core::vector3df playerLoc = GameSystem::getSingleton().getGravship()->getSceneNode()->getPosition;
	const irr::core::vector3df myLoc = this->getSceneNode()->getPosition();

	irr::core::vector3df conn = playLoc - myLoc;
	conn = conn.normalize();

	irr::core::vector3df vel = this->m_Physics_Body->getVelocity();
	float spd = vel.getLengthSQ();

	conn = conn * spd;

	this->m_Physical_Body->setVelocity(conn);

}

//void Enemy::moveRandomly(){
//
//	irr::core::vector3df dirct = 
//
//}
//
Enemy::Enemy() {
	color = '0';
}

Enemy::~Enemy(){
}