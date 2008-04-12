#include "irrlicht.h"
#include "Common.h"
#include "irrnewt.hpp"
#include "LevelManager.h"
#include "EntityManager.h"
#include "Gravship.h"
#include "PhysicsManager.h"

void Gravship::load(){
	helper = &(EntityManager::getSingleton().createEntity("gravship_helper");



	irr::scene::ISceneNode* node = LevelManager::getSceneManager()->addSphereSceneNode(irr::f32(this->m_Radius*.2));
		
	if (node){
		node->setScale(irr::core::vector3df(1.0f, 1.0f, 1.0f));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		
	}
	irr::newton::IMaterial* material = PhysicsManager::getSingleton().getMaterial("gravship_helper");

	irr::newton::SBodyFromNode physics_node;

	physics_node.Node = node;
	physics_node.Type = irr::newton::EBT_PRIMITIVE_ELLIPSOID;

	irr::newton::IBody* body = PhysicsManager::getSingleton().getPhysicsWorld()->createBody(physics_node);

	body->setContinuousCollisionMode(false);

	body->setMaterial(material);

	body->setUserData(entity);

	body->addForceContinuous(irr::core::vector3df(0,0, PhysicsManager::getSingleton().getGravity()));

	body->setMass(100);
	entity->setPhysicsBody(body);
}

void Gravship::update(){
	const irr::core::vector3df position  = this->m_Physics_Body->getPosition();
	this->m_Physics_Body->setPosition(irr::core::vector3df(position.X,position.Y, 0));
	//this->m_Physics_Body->setVelocity(this->velocity);

	//Find any new entities that are in our gravity field range and remove those who are no longer in it.
	updateOrbitingEntities();
	//If gravity field is on, Apply force to any entities in our orbiting range
	if (this->m_GravityOn){
		applyGravityToOrbitingEntities();
	}


	
}

void Gravship::applyGravityToOrbitingEntities(){
	for (m_OrbitingEntitiesItr = m_OrbitingEntities.begin(); m_OrbitingEntitiesItr != m_OrbitingEntities.end(); m_OrbitingEntitiesItr++){
		WorldEntity* otherEntity = (*m_OrbitingEntitiesItr);
		irr::core::vector3df  distance =  this->getSceneNode()->getPosition() - otherEntity->getSceneNode()->getPosition();

		//create the relative movement vector
		//irr::core::vector3df movevec(0, 0, 0);

	    //movevec += this->m_Physics_Body->getVelocity();
	    //movevec -= otherEntity->getPhysicsBody()->getVelocity();
		//float sumradii = (this->m_OrbitRingRadius + otherEntity->getBoundingSphereRadius() );
        //float sumradiisq = sumradii * sumradii;
		
		float distanceSQ = distance.getLengthSQ();
		//Normalise the movevec
		//irr::core::vector3df N(movevec);
		//N.normalize();


		//D = N dot C = ||C|| * cos(angle between N and C)
		//float D = N.dotProduct(distance);

		//F = length(C)^2 - D^2
		//float F = distanceSQ - (D*D);

		//Escape test: if the closest that A will get to B (sqrt(F) ) is more than the sum of their radii
		//then they won't collide
		irr::core::vector3df gravityForce(distance);
		gravityForce.normalize();
		gravityForce = gravityForce *  this->m_GravitationalPull * (this->getPhysicsBody()->getMass() * (*m_OrbitingEntitiesItr)->getPhysicsBody()->getMass()) / distanceSQ;
		if (this->m_GravitationalPull < 0){
			//multiplier for reverse gravity
			gravityForce = gravityForce * 2.5;
			
		}
		otherEntity->getPhysicsBody()->addForce(gravityForce);
	}
}

void Gravship::updateOrbitingEntities(){
	//Do collision test against all objects in range that are not currently in our orbiting list.
	Entity::IdEntityMap entities = EntityManager::getSingleton().getEntities();
	//Remove entities from the list of returned entities that we already know were orbiting us last frame by using their ID.
	for (m_OrbitingEntitiesItr = m_OrbitingEntities.begin(); m_OrbitingEntitiesItr != m_OrbitingEntities.end(); m_OrbitingEntitiesItr++){
		entities.erase((*m_OrbitingEntitiesItr)->getID());
	}
	for (m_EntityItr = entities.begin(); m_EntityItr != entities.end(); m_EntityItr++){

		//Check if current entity is colliding with the gravity field radius.
		        /*********************************************************************************************************/
       //Collision detection code based on "Pool Hall Lessons" by Joe van den Heuvel and Miles Jackson [18.1.02]
       //http://www.gamasutra.com/features/20020118/vandenhuevel_02.htm
	   WorldEntity* otherEntity = m_EntityItr->second;
       using irr::core::vector3df;

       //create the relative movement vector
       vector3df movevec(0, 0, 0);

	   movevec += this->m_Physics_Body->getVelocity();
	   movevec -= otherEntity->getPhysicsBody()->getVelocity();

       //Early escape test, if movevec is 0 - there aint no movement goin on
       if (movevec.getLengthSQ() == 0.0f)
               continue;

       //Early escape test, if the length of the movevec is less than the distance
       //between the closest edges of the circles, there's no way they can hit
	   float distsq = otherEntity->getSceneNode()->getPosition().getDistanceFromSQ(this->getSceneNode()->getPosition() );
	   float sumradii = (this->m_GravityFieldRadius + otherEntity->getBoundingSphereRadius() );
       float sumradiisq = sumradii * sumradii;

       distsq -= sumradiisq;
       if (movevec.getLengthSQ() < distsq)
               continue;

       //Normalise the movevec
       vector3df N(movevec);
       N.normalize();

       //Find C, the vector from the centre of circleA to the centre of circleB
       vector3df C = otherEntity->getSceneNode()->getPosition() - this->getSceneNode()->getPosition();

       //D = N dot C = ||C|| * cos(angle between N and C)
       float D = N.dotProduct(C);

       //Another Early Escape test: Make sure that A is moving towards B
       //If the dotproduct of N and C is 0, the sphere's are moving away from each other
       if (D <= 0.0f)
            continue;

       //F = length(C)^2 - D^2
       float F = C.getLengthSQ() - (D*D);

       //Escape test: if the closest that A will get to B (sqrt(F) ) is more than the sum of their radii
       //then they won't collide
       if (F > sumradiisq)
               continue;

       //Find the third side of the triangle between centre of the two circles when they touch,
       // and the closest they get to each other: sqrt(T)
       float T = sumradiisq - F;

       //If there is no right triangle with sides of length sumradii and sqrt(F), T will
       //probably be less than zero
       if (T < 0.0f)
               continue;

       float distance = D - sqrt(T);
       if (distance < 0.0f)
       {
           //the objects have already interpenetrated

		   this->m_OrbitingEntities.insert(otherEntity);
           continue;
       }

       //get the magnitude of the movement vector
       float mag = movevec.getLength();

       //Make sure the distance A has to move to touch B is not greater than
       //the magnitude of the movement vector
       if (mag < distance)
               continue;

       //IT IS CONFIRMED THAT THE OBJECTS WILL COLLIDE!!!

	   this->m_OrbitingEntities.insert(otherEntity);
	   continue;
       /*********************************************************************************************************/
	}

	//Make sure that orbiting entities are still in range.
	std::list<WorldEntity*> entitiesToRemove;
	for (m_OrbitingEntitiesItr = m_OrbitingEntities.begin(); m_OrbitingEntitiesItr != m_OrbitingEntities.end(); m_OrbitingEntitiesItr++){
		WorldEntity* otherEntity = *m_OrbitingEntitiesItr;
		using irr::core::vector3df;

		//create the relative movement vector
		vector3df movevec(0, 0, 0);

		movevec += this->m_Physics_Body->getVelocity();
		movevec -= otherEntity->getPhysicsBody()->getVelocity();

		//Early escape test, if movevec is 0 - there aint no movement goin on
		if (movevec.getLengthSQ() == 0.0f)
			   continue;

		//Early escape test, if the length of the movevec is less than the distance
		//between the closest edges of the circles, there's no way they can hit
	
		float sumradii = (this->m_GravityFieldRadius + otherEntity->getBoundingSphereRadius() );
		float sumradiisq = sumradii * sumradii;


		//Normalise the movevec
		vector3df N(movevec);
		N.normalize();

		//Find C, the vector from the centre of circleA to the centre of circleB
		vector3df C = otherEntity->getSceneNode()->getPosition() - this->getSceneNode()->getPosition();

		//D = N dot C = ||C|| * cos(angle between N and C)
		float D = N.dotProduct(C);

		//F = length(C)^2 - D^2
		float F = C.getLengthSQ() - (D*D);

		//Escape test: if the closest that A will get to B (sqrt(F) ) is more than the sum of their radii
		//then they won't collide
		if (F > sumradiisq)
			entitiesToRemove.push_back(*m_OrbitingEntitiesItr);
	}
	std::list<WorldEntity*>::iterator entitiesToRemoveItr;
	for (entitiesToRemoveItr = entitiesToRemove.begin(); entitiesToRemoveItr != entitiesToRemove.end(); entitiesToRemoveItr++){
		m_OrbitingEntities.erase((*entitiesToRemoveItr));
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
	entity->m_GravitationalPull = this->m_GravitationalPull;
	entity->m_GravitationCentripetalForce = this->m_GravitationCentripetalForce;
	entity->m_GravityFieldRadius = this->m_GravityFieldRadius;
	entity->m_OrbitRingRadius = this->m_OrbitRingRadius;

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

void Gravship::setGravityFieldRadius(float newRadius){
	this->m_GravityFieldRadius = newRadius;
}

void Gravship::changeState(const std::string name){
}

void Gravship::changeVelocity(float x_speed, float y_speed){
}

Gravship* Gravship::EntityToGravship(WorldEntity* entity){
	return dynamic_cast<Gravship*>(entity);
}

Gravship::Gravship() : m_GravityOn(false), helper(NULL){

}

Gravship::~Gravship(){
}