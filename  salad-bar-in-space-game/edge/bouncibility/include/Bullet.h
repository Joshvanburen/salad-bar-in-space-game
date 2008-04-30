/* 
 * 
 * Bullet.h
 * 
 *
 */

#pragma once

#include <utility>	//For using pair
#include "Common.h"
#include "WorldEntity.h"
#include "ScriptManager.h"
#include "WorldEntityAIManager.h"

//namespace Scripting{
//	class ScriptFunction;
//	class WorldEntityAIFunction;
//}

//class GameSystem;

class Bullet : public WorldEntity {

public:


	// Constructor and destructor
	Bullet();
	~Bullet();

	void load();


	static Bullet* EntityToBullet(WorldEntity*);


	//Where story happens, check state, collision detect, physics staffs.
	void update();


	// Change Bullet state, all the other properties of the Bullet may be changed from here
	void changeState(const std::string name) {};

	// Change Bullet speed, no accleration right now.
	void changeVelocity(float x_speed, float y_speed);

	void setSpeed(float spd) { speed = spd;};

	void moveToPlayer();

	void moveTo(irr::core::vector3df loc);

	WorldEntity* clone();

private: 
 	

	float speed;

	int ttl;
	//2d speed and direction - Irrlicht probably has a better vector class for representing 2d vectors!
	irr::core::vector3df velocity;



};


