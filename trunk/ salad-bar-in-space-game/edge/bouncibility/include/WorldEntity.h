#ifndef WORLD_ENTITY_H
#define WORLD_ENTITY_H

#include "Common.h"
#include "singleton.h"
#include "irrlicht.h"
namespace irr{
	namespace newton{
		class IMaterial;
		class IBody;
	}

	
}
namespace Entity{
	class EntityFactory;
}

class WorldEntity{
	friend class Entity::EntityFactory;
	friend class EntityManager;
public:

	// Constructor
	WorldEntity();
	//Overloaded Constructors
	WorldEntity(int iID);
	WorldEntity(int iID, float x, float y, float z);

	// virtual destructor
	virtual ~WorldEntity();

	// Every world entity will have a location
	void setLocation(float x, float y, float z);
	void setLocation(irr::core::vector3df newLocation);

	irr::core::vector3df getLocation(){
		return location;
	}

	float getX();

	float getY();

	irr::scene::ISceneNode* entity();

	void AddRef(){

	}
	void move();
	void Release(){

	}
	// get/set ID
	int getID();
	void setID( int iID );

	//Returns a pointer to the scene node for this WorldEntity
	irr::scene::ISceneNode* getSceneNode(){
		return m_SceneNode;
	}

	void setSceneNode(irr::scene::ISceneNode* node){
		m_SceneNode = node;
		calculateBoundingSphere();
	}

	void setPhysicsBody(irr::newton::IBody* body){
		m_Physics_Body = body;
	}


	irr::newton::IBody* getPhysicsBody(){
		return m_Physics_Body;
	}


	irr::scene::IAnimatedMesh* getMesh(){
		return m_Mesh;
	}


	void setMesh(irr::scene::IAnimatedMesh* newMesh);

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

	float getBoundingSphereRadius();
protected:
	
	//!Create a deep copy of this WorldEntity, used by EntityManager to clone and entity for efficiency.
	virtual WorldEntity* clone() = 0;

	//!Calculates the bounding sphere of this entity used for simple collision detections.
	void calculateBoundingSphere();

	irr::core::vector3df location;
	int id;// Unique ID

	//!Radius of bounding sphere
	float m_Radius;

	//!Mass of the object used in the physics system
	int m_Mass;
	irr::scene::ISceneNode* m_SceneNode; //Scene node of worldEntity.  Assumed to be initialized and added to the scene by factory.

	irr::scene::IAnimatedMesh* m_Mesh;

	irr::newton::IBody* m_Physics_Body;

};

#endif