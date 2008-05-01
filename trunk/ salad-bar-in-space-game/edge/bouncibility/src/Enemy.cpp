#include "Common.h"
#include "GameIncludes.h"


Bullet* Enemy::s_BulletSrc = NULL;

void Enemy::load(){
	if (!s_BulletSrc){
		s_BulletSrc = (Bullet*)&EntityManager::getSingleton().createEntity("bullet");
		s_BulletSrc->setLocation(-10000, -10000, -10000);
		s_BulletSrc->setVisible(false);
		s_BulletSrc->getPhysicsBody()->setFreeze(true);
		//EntityManager::getSingleton().claim(s_BulletSrc->getID());
	}
}

void Enemy::update(){
	WorldEntity::update();


	genRandomDest();

	//this->m_Physics_Body->setVelocity(this->velocity);

	if (m_EnableMovement){
		timer += GameSystem::getSingleton().getDeltaMillis();
		if (timer > 2000) {
			ai_script->callFunction(this);
			timer =  0;
		}
	}

	if (this->getSceneNode()->getPosition().Z > 800) {
		//EntityManager::getSingleton().remove(this->id);
		return;
	}


}

WorldEntity* Enemy::clone(){
	Enemy* entity = new Enemy();
	entity->location = this->location;
	entity->currentState = this->currentState;
	if (m_Mesh){
		this->m_Mesh->grab();
	}
	entity->splatImage = this->splatImage;
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

	body->addForceContinuous(irr::core::vector3df(0,0,PhysicsManager::getSingleton().getGravity()));
	entity->m_Physics_Body = body;
	entity->color = this->color;
	entity->weight = this->weight;
	entity->attack = this->attack;
	entity->bouncibility = this->bouncibility;
	entity->radius = this->radius;
	entity->velocity = this->velocity;

	entity->ai_script = this->ai_script;

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

	//genRandomNW();
	dest = GameSystem::getSingleton().getGravship()->getSceneNode()->getPosition();
    moveToDest();
}


bool Enemy::moveToDest() {
	
	const irr::core::vector3df myLoc = this->getSceneNode()->getPosition();


	irr::core::vector3df conn = myLoc - dest;

	if (conn.getLength() < 10) { return false;}

	conn = conn.normalize();

	conn = conn * speed;

	this->m_Physics_Body->setVelocity(conn);

	
	//this->m_Physics_Body->addForce(conn);
	
	irr::core::vector3df velocity = this->m_Physics_Body->getVelocity();
	if (velocity.getLengthSQ() > m_MaxSpeedSQ){
		this->m_Physics_Body->setVelocity(velocity.normalize() * m_MaxSpeed/2);
	}
	
	return true;

}

bool Enemy::moveNowhere() {
	
	const irr::core::vector3df myLoc = this->getSceneNode()->getPosition();


	irr::core::vector3df conn = myLoc - nowhere;

	if (conn.getLength() < 10) { return false;}

	conn = conn.normalize();

	conn = conn * speed;

	this->m_Physics_Body->setVelocity(conn);

	
	//this->m_Physics_Body->addForce(conn);
	
	irr::core::vector3df velocity = this->m_Physics_Body->getVelocity();
	if (velocity.getLengthSQ() > m_MaxSpeedSQ){
		this->m_Physics_Body->setVelocity(velocity.normalize() * m_MaxSpeed);
	}
	
	return true;

}


bool Enemy::moveAwayFromPlayer(float distance) {
	const irr::core::vector3df myLoc = this->getSceneNode()->getPosition();
	const irr::core::vector3df playerLoc = GameSystem::getSingleton().getGravship()->getSceneNode()->getPosition();

	irr::core::vector3df conn = playerLoc - myLoc;

	if (conn.getLength() > distance) {return false;}

	conn = conn.normalize();

	//irr::core::vector3df vel = this->m_Physics_Body->getVelocity();
	//float spd = 75;
	conn = conn * speed;

	//std::cout << speed;

	this->m_Physics_Body->addForce(conn);

	irr::core::vector3df velocity = this->m_Physics_Body->getVelocity();
	if (velocity.getLengthSQ() > m_MaxSpeedSQ){
		this->m_Physics_Body->setVelocity(velocity.normalize() * m_MaxSpeed);
	}

	return true;

}

void Enemy::shootPlayer() {
	
	target = GameSystem::getSingleton().getGravship()->getSceneNode()->getPosition();

	shootTarget();
}

void Enemy::shootTarget() {

	Bullet* newBullet = dynamic_cast<Bullet*>(&EntityManager::getSingleton().cloneEntity(Enemy::s_BulletSrc->getID()));
	newBullet->setLocation(this->m_Physics_Body->getPosition());
	target = GameSystem::getSingleton().getGravship()->getSceneNode()->getPosition();
	newBullet->dest = target;
	newBullet->moveToDest();

}

//irr::core::vector3df genRandomLoc() {
void Enemy::genRandomDest() {
	irr::core::vector3df newLoc = GameSystem::getSingleton().getGravship()->getSceneNode()->getPosition();

	newLoc.X = newLoc.X + 800 * rand()/(RAND_MAX + 1.0) - 400;
	newLoc.Y = newLoc.Y + 800 * rand()/(RAND_MAX + 1.0) - 400;
	newLoc.Z = 1000;

	setDest(newLoc);

	//return newLoc;

}

void Enemy::genRandomNW() {

	nowhere.X = 4000 * rand()/(RAND_MAX + 1.0) ;
	nowhere.Y = 4000 * rand()/(RAND_MAX + 1.0) ;
	nowhere.Z = 1000;


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

	splatTTL = 2000;
	splatScreen = NULL;
	genRandomNW();

	
	
	target = dest;

	splatImage = NULL;
}

Enemy::~Enemy(){
	
}

void Enemy::splat(){
	int x = rand() % 800 + 200;
	int y = rand() % 600 + 50;
	GameSystem::getSingleton().appendSplashScreen(GameSystem::getSingleton().getGUI()->addImage(splatImage, irr::core::position2d<irr::s32>(x, y), true));
	std::cout << "splat! " << splatImage->getName() << "\n";
}
