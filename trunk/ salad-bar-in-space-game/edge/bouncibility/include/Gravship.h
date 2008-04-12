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
class Gravship : public WorldEntity {
	friend class Entity::GravshipFactory;
public:


	// Constructor and destructor
	Gravship();
	~Gravship();

	void load();


	static Gravship* EntityToGravship(WorldEntity*);


	//Where story happens, check state, collision detect, physics staffs.
	void update();

	void draw();

	void enableGravityField(bool enabled){
		m_GravityOn=enabled;
	}

	void reverseGravityField(bool enabled){
		if (enabled){
			this->m_GravitationalPull *= -1;
		}
		else{
			this->m_GravitationalPull = abs(this->m_GravitationalPull);
		}
	}
	// Change ball state, all the other properties of the ball may be changed from here
	void changeState(const std::string name);

	// Change speed, no accleration right now.
	void changeVelocity(float x_speed, float y_speed);

	void setGravityFieldRadius(float newRadius);
private: 
 	
	WorldEntity* clone();

	void updateOrbitingEntities();

	void applyGravityToOrbitingEntities();

	//This is the Gravship's gravity helper object
	WorldEntity* helper;

	//We are trying to use as few as possible parameters
	//These two parameter can describe many states
	//They always change together, unless in Helium state
	
	float weight; 


	//How powerful the ball is
	int   attack;

	//2d speed and direction - Irrlicht probably has a better vector class for representing 2d vectors!
	irr::core::vector3df velocity;

	//I'll delete it in ball after it implemented in WorldEntity
	std::string currentState;


	//Gravity field attributes

	std::set<WorldEntity*> m_OrbitingEntities;

	std::set<WorldEntity*>::iterator m_OrbitingEntitiesItr;

	std::map<int, WorldEntity*>::iterator m_EntityItr;

	bool m_GravityOn;

	float m_GravityFieldRadius;

	float m_GravitationalPull;

	float m_GravitationCentripetalForce;

	float m_OrbitRingRadius;

};
