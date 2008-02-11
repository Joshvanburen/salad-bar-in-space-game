#ifndef WORLD_ENTITY_H
#define WORLD_ENTITY_H

#include "irrlicht.h"
using namespace scene;

class WorldEntity{

public:
	// Constructor
	WorldEntity();
	// destructor
	~WorldEntity();

	void setLocation( x, y, z);
	
	SceneNode entity();

private:
	float x;// Used for location on the x-axis
	float y;// Used for location on the y-axis
	float z;// Used for location on the z-axis

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
};

#endif