#include "WorldEntity.h"

// default constructor
WorldEntity::WorldEntity() : sceneNode(NULL){
	x = 0;
	y = 0;
	z = 0;
}

// lets user set the id
WorldEntity::WorldEntity(int iID) sceneNode(NULL){
	setID(iID);
	x = 0;
	y = 0;
	z = 0;
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