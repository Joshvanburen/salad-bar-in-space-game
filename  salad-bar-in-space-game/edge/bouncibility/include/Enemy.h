/* 
 * 
 * Enemy.h
 * 
 *
 */

#pragma once

#include "Common.h"
#include "WorldEntity.h"
#include "ScriptManager.h"
#include "WorldEntityAIManager.h"

namespace Scripting{
	class ScriptFunction;
	class WorldEntityAIFunction;
}
 

class GameSystem;
class Bullet;

class Enemy : public WorldEntity {

public:


	// Constructor and destructor
	Enemy();
	~Enemy();

	void load();


	static Enemy* EntityToEnemy(WorldEntity*);


	//Where story happens, check state, collision detect, physics staffs.
	void update();

	void draw();

	// Change Enemy state, all the other properties of the Enemy may be changed from here
	void changeState(const std::string name);

	// Change Enemy speed, no accleration right now.
	void changeVelocity(float x_speed, float y_speed);

	//Jump!! Yahooooo!
	//Right now we have two different kinds of jumps
	//jump will give velocity a y+ speed, which is depends on jump_type
	void jump(int jump_type);

	char getColor(){
		return color;
	}
	void setColor(std::string& newColor){
		color = newColor.c_str()[0];
	}

	float getRadius(){
		return radius;
	}

	void setRadius(float newRadius){
		radius = newRadius;
	}

	void setAi(Scripting::WorldEntityAIFunction*);

	void setSpeed(float spd) {speed = spd;};

	void setDest(irr::core::vector3df newDest) {dest = newDest;}
	void setTarget(irr::core::vector3df newTarget) {target = newTarget;}

	void moveToPlayer();

	int moveToDest();

	int moveAwayFromPlayer(float distance);

	void shootPlayer();

	void shootTarget();

	//irr::core::vector3df genRandomLoc();
	void genRandomLoc();

	//static Bullet* bulletSrc;

private: 
 	
	WorldEntity* clone();

	float radius;
	float weight; 

	char color;

	//How powerful the Enemy is
	int   attack;

	//2d speed and direction - Irrlicht probably has a better vector class for representing 2d vectors!
	irr::core::vector3df velocity;

	//I'll delete it in Enemy after it implemented in WorldEntity
	int currentState;

	//How much percents of speed left after bouncing
	int bouncibility;

	// point to the ai scripting function
	Scripting::WorldEntityAIFunction* ai_script;

	int timer;

	float speed;

	irr::core::vector3df dest;
	irr::core::vector3df target;

};


