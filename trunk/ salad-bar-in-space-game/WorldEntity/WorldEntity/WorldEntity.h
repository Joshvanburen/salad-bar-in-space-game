#ifndef WORLD_ENTITY_H
#define WORLD_ENTITY_H

#include "irrlicht.h"
using namespace scene;

class WorldEntity{

public:
	// Constructor
	WorldEntity();
	//Overloaded Constructor
	WorldEntity(int iID);
	// virtual destructor
	virtual ~WorldEntity(){};

	// Every world entity will have a location
	void setLocation(x, y, z);
	
	SceneNode entity();

	// get/set ID
	int getID();
	void setID( int iID );

	// The update function prototypes take a float fElapsedTime 
	// which will be how much time has passed since the last time 
	// that function was called. This information is useful for 
	// animation of objects as well as determining collision
	// of moving objects.	
	virtual void update(float fElapsedTime) = 0;

	// The draw function prototypes take a float fElapsedTime 
	// which will be how much time has passed since the last time 
	// that function was called. This information is useful for 
	// animation of objects as well as determining collision
	// of moving objects.	
	virtual void draw(float fElapsedTime) = 0;

	virtual void load() = 0;

	// There are many WorldEntities that we may wish to pass a change of state to,
	// so the world Entity should be where the current state object is held. It
	// provides a virtual function for manipulating the state.
	virtual void changeState(const std::string name) = 0;

private:
	float fx;// Used for location on the x-axis
	float fy;// Used for location on the y-axis
	float fz;// Used for location on the z-axis

	int id;// Unique ID
};

#endif