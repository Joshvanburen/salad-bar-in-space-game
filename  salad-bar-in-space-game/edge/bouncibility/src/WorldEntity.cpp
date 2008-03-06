#include "WorldEntity.h"
#include "irrnewt.hpp"
#include "irrlicht.h"
// default constructor
WorldEntity::WorldEntity() : sceneNode(NULL), mesh(NULL), physics_material(NULL), physics_body(NULL){
	fx = 0;
	fy = 0;
	fz = 0;
}

// lets user set the id
WorldEntity::WorldEntity(int iID): sceneNode(NULL), mesh(NULL), physics_material(NULL), physics_body(NULL){
	setID(iID);
	fx = 0;
	fy = 0;
	fz = 0;
}

WorldEntity::~WorldEntity(){

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
void WorldEntity::setBodyMaterial(irr::newton::IMaterial* newMaterial){
	physics_material = newMaterial;
}
void WorldEntity::setMesh(irr::scene::IAnimatedMesh* newMesh){
	mesh = newMesh;
}
void WorldEntity::setPhysicsBody(irr::newton::IBody* newBody){
	physics_body = newBody;
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