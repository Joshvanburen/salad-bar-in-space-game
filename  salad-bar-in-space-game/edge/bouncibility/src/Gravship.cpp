#include "Common.h"
#include "GameIncludes.h"

Bullet* Gravship::s_BulletSrc = NULL;
void Gravship::load(){

	if (!s_BulletSrc){
		s_BulletSrc = (Bullet*)&EntityManager::getSingleton().createEntity("gravshipb");
		s_BulletSrc->setLocation(-19999, -19999, -19999);
		s_BulletSrc->setVisible(false);
		s_BulletSrc->getPhysicsBody()->setFreeze(true);
		//EntityManager::getSingleton().claim(s_BulletSrc->getID());
	}
}

void Gravship::shoot() {

	Bullet* newBullet = dynamic_cast<Bullet*>(&EntityManager::getSingleton().cloneEntity(Gravship::s_BulletSrc->getID()));
	newBullet->setLocation(this->m_Physics_Body->getPosition().X, this->m_Physics_Body->getPosition().Y, this->m_Physics_Body->getPosition().Z - 50);
	target.X = this->m_Physics_Body->getPosition().X;
	target.Y = this->m_Physics_Body->getPosition().Y;
	target.Z = -10000;
	newBullet->dest = target;
	newBullet->moveToDest();

}



void Gravship::update(){
	WorldEntity::update();


	//If gravity field is on, Apply force to any entities in our orbiting range
	if (this->m_GravityOn){
	}

	//shoot();

	m_Helper->update();
	
}


void Gravship::takeDamage(int howmuch){
	m_Health -= howmuch;
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

/*
void Gravship::shoot(){
	WorldEntity* bullet = &EntityManager::getSingleton().cloneEntity(*GameSystem::bulletSrc);
	bullet->setLocation(this->getLocation());
	bullet->getPhysicsBody()->setVelocity(irr::core::vector3df(0.0f, 0.0f, 20.0f));

}
*/

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
	m_Health = 225;
}

Gravship::~Gravship(){
}