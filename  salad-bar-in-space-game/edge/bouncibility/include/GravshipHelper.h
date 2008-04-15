/* 
 * 
 * GravshipHelper.h
 * 
 *
 */

#pragma once

#include "Common.h"
#include "WorldEntity.h"

namespace Entity{
class GravshipHelperFactory;
};
class GravshipHelper : public WorldEntity {
	friend class Entity::GravshipHelperFactory;
public:


	// Constructor and destructor
	GravshipHelper();
	~GravshipHelper();

	void load();


	static GravshipHelper* EntityToGravshipHelper(WorldEntity*);


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
	// Change Enemy state, all the other properties of the Enemy may be changed from here
	void changeState(const std::string name);

	// Change speed, no accleration right now.
	void changeVelocity(float x_speed, float y_speed);

	void setGravityFieldRadius(float newRadius);

	void setMaxOrbitSpeed(float newSpeed){
		m_MaxOrbiterSpeed = newSpeed;
		m_MaxOrbiterSpeedSQ = newSpeed*newSpeed;
	}

	void setMaxForce(float newForce){
		m_MaxForce = newForce;
		m_MaxForceSQ = newForce * newForce;
	}

	std::set<WorldEntity*> getOrbitingEntities(){
		return m_OrbitingEntities;
	}
private: 
 	
	WorldEntity* clone();

	void updateOrbitingEntities();

	void applyGravityToOrbitingEntities();

	//This is the Gravship's gravity helper object
	WorldEntity* m_Helper;

	irr::newton::IMaterial* m_EmptyMaterial;

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

	std::set<WorldEntity*> m_OrbitingEntities;

	std::set<WorldEntity*>::iterator m_OrbitingEntitiesItr;

	std::map<int, WorldEntity*>::iterator m_EntityItr;

	bool m_GravityOn;

	float m_GravityFieldRadius;

	float m_GravitationalPull;

	float m_GravitationCentripetalForce;

	float m_OrbitRingRadius;

	float m_MaxOrbiterSpeed;
	float m_MaxOrbiterSpeedSQ;
	float m_MaxForce;
	float m_MaxForceSQ;


};
