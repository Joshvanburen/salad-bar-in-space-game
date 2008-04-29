#include "Common.h"
#include "GameIncludes.h"


void Enemy::load(){

}

void Enemy::update(){
	WorldEntity::update();

	//this->m_Physics_Body->setVelocity(this->velocity);

	if (m_EnableMovement){
		timer += GameSystem::getSingleton().getMillis();
		if (timer > 1000) {
			ai_script->callFunction(this);
			timer =  0;
		}
	}


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
	//Clone isn't working for the animated scene node.  Should just make from scratch.
	irr::scene::IAnimatedMesh* mesh = entity->m_Mesh;
	irr::scene::IAnimatedMeshSceneNode* node = LevelManager::getSceneManager()->addAnimatedMeshSceneNode(mesh);

	if (node){
		node->setScale(irr::core::vector3df(this->m_SceneNode->getScale()));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		
	}
	entity->m_SceneNode = node;

	irr::newton::SBodyFromNode physics_node;
	physics_node.Mesh = m_Mesh->getMesh(2);
	physics_node.Node = entity->m_SceneNode;
	physics_node.Type = irr::newton::EBT_PRIMITIVE_BOX;

	//irr::newton::IBody* body = this->m_Physics_Body;
	//PhysicsManager::getSingleton().getPhysicsWorld()->createBody(physics_node);
	irr::newton::ICharacterController* body = PhysicsManager::getSingleton().getPhysicsWorld()->createCharacterController(PhysicsManager::getSingleton().getPhysicsWorld()->createBody(physics_node));
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

	dest = GameSystem::getSingleton().getGravship()->getSceneNode()->getPosition();
    moveToDest();
}


int Enemy::moveToDest() {
	
	const irr::core::vector3df myLoc = this->getSceneNode()->getPosition();

	irr::core::vector3df conn = dest - myLoc;

	if (conn.getLength() < 10) { return 0;}

	conn = conn.normalize();

	conn = conn * speed;


	this->m_Physics_Body->addForce(conn);

	irr::core::vector3df velocity = this->m_Physics_Body->getVelocity();
	if (velocity.getLengthSQ() > m_MaxSpeedSQ){
		this->m_Physics_Body->setVelocity(velocity.normalize() * m_MaxSpeed);
	}

	return 1;

}

int Enemy::moveAwayFromPlayer(float distance) {

	const irr::core::vector3df myLoc = this->getSceneNode()->getPosition();
	const irr::core::vector3df playerLoc = GameSystem::getSingleton().getGravship()->getSceneNode()->getPosition();

	irr::core::vector3df conn = myLoc - playerLoc;

	if (conn.getLength() > distance) {return 0;}

	conn = conn.normalize();

	//irr::core::vector3df vel = this->m_Physics_Body->getVelocity();
	//float spd = 75;
	conn = conn * speed;


	this->m_Physics_Body->addForce(conn);

	irr::core::vector3df velocity = this->m_Physics_Body->getVelocity();
	if (velocity.getLengthSQ() > m_MaxSpeedSQ){
		this->m_Physics_Body->setVelocity(velocity.normalize() * m_MaxSpeed);
	}

	return 1;

}

void Enemy::shootPlayer() {
	
	target = GameSystem::getSingleton().getGravship()->getSceneNode()->getPosition();

	shootTarget();
}

void Enemy::shootTarget() {

	Bullet* newBullet = dynamic_cast<Bullet*>(GameSystem::bulletSrc->clone());
	newBullet->setLocation(this->getLocation());
	newBullet->moveTo(this->target);

}

//irr::core::vector3df genRandomLoc() {
void Enemy::genRandomLoc() {
	irr::core::vector3df newLoc;

	newLoc.X = 1000 * rand()/(RAND_MAX + 1.0);
	newLoc.Y = 1000 * rand()/(RAND_MAX + 1.0);
	newLoc.Z = 1000 * rand()/(RAND_MAX + 1.0);

	setDest(newLoc);

	//return newLoc;

}

//void Enemy::moveRandomly(){
//
//	irr::core::vector3df dirct = 
//
//}
//
Enemy::Enemy() : WorldEntity(){
	color = '0';
	timer = 0;

	dest.X = 0;
	dest.Y = 0;
	dest.Z = 0;

	//bulletSrc = GameSystem::getBulletSrc();
	target = dest;
}

Enemy::~Enemy(){
}