#include "WorldEntity.h"

WorldEntity::WorldEntity(){
	x = 0;
	y = 0;
	z = 0;
}

void WorldEntity::setID( int iID ){
	if(iID > 0)
		id = iID;
}

int WorldEntity::getID(){
	return id;
}

// Sets location of entity
void WorldEntity::setLocation(x, y, z){
	fx = x;
	fy = y;
	fz = z;
}