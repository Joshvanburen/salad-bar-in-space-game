#include "WorldEntity.h"
#include "irrlicht.h"
// default constructor
WorldEntity::WorldEntity() : sceneNode(NULL){
	fx = 0;
	fy = 0;
	fz = 0;
}

// lets user set the id
WorldEntity::WorldEntity(int iID): sceneNode(NULL){
	setID(iID);
	fx = 0;
	fy = 0;
	fz = 0;
}

WorldEntity::WorldEntity(int iID, float x, float y, float z){
	setID(iID);
	setLocation(x,y,z);
}

void WorldEntity::setID( int iID ){
	if(iID > 0)
		id = iID;
}

int WorldEntity::getID(){
	return id;
}

// Sets location of entity
void WorldEntity::setLocation( float x, float y, float z){
	fx = x;
	fy = y;
	fz = z;
}

void WorldEntity::setSceneNode(irr::scene::ISceneNode* sceneNode){
	this->sceneNode = sceneNode;
}