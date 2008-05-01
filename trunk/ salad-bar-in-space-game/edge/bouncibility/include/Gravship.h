/* 
 * 
 * Gravship.h
 * 
 *
 */

#pragma once

#include "Common.h"
#include "WorldEntity.h"

namespace Entity{
class GravshipFactory;
};
class GravshipHelper;

class Gravship : public WorldEntity {
	friend class Entity::GravshipFactory;
public:

	static Bullet* s_BulletSrc;


	// Constructor and destructor
	Gravship();
	~Gravship();

	void load();


	static Gravship* EntityToGravship(WorldEntity*);


	//Where story happens, check state, collision detect, physics staffs.
	void update();

	void shoot();

	void draw();

	void takeDamage(int howmuch);

	void enableGravityField(bool enabled);

	GravshipHelper* getHelper(){
		return m_Helper;
	}

	int getHealth(){
		return m_Health;
	}
	// Change Enemy state, all the other properties of the Enemy may be changed from here
	void changeState(const std::string name);

	// Change speed, no accleration right now.
	void changeVelocity(float x_speed, float y_speed);

private: 
 	
	WorldEntity* clone();


	int m_Health;
	//This is the Gravship's gravity helper object
	GravshipHelper* m_Helper;

	//We are trying to use as few as possible parameters
	//These two parameter can describe many states
	//They always change together, unless in Helium state
	
	float weight; 


	//How powerful the Enemy is
	int   attack;

	//2d speed and direction - Irrlicht probably has a better vector class for representing 2d vectors!
	irr::core::vector3df velocity;

	//I'll delete it in Enemy after it implemented in WorldEntity
	std::string currentState;


	//Gravity field attributes


	bool m_GravityOn;

	irr::core::vector3df target;
};
