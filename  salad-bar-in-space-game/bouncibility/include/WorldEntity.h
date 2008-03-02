#ifndef WORLD_ENTITY_H
#define WORLD_ENTITY_H

#include "Common.h"
namespace irr{
namespace scene{
	class ISceneNode;
}
}
class WorldEntity{

public:
	// Constructor
	WorldEntity();
	//Overloaded Constructors
	WorldEntity(int iID);
	WorldEntity(int iID, float x, float y, float z);

	// virtual destructor
	virtual ~WorldEntity(){};

	// Every world entity will have a location
	void setLocation(float x, float y, float z);
	
	irr::scene::ISceneNode* entity();

	// get/set ID
	int getID();
	void setID( int iID );

	//Returns a pointer to the scene node for this WorldEntity
	irr::scene::ISceneNode* getSceneNode(){
		return sceneNode;
	}

	void setSceneNode(irr::scene::ISceneNode* sceneNode);

	// The update function prototypes take a float fElapsedTime 
	// which will be how much time has passed since the last time 
	// that function was called. This information is useful for 
	// animation of objects as well as determining collision
	// of moving objects.	--Chase I think we should have UserInterface or something have a static getCurrentTime method instead of passing it in as a parameter.
	virtual void update() = 0;


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

	irr::scene::ISceneNode* sceneNode; //Scene node of worldEntity.  Assumed to be initialized and added to the scene by factory.
};

#endif