#include "WorldEntity.h"
#include "irrnewt.hpp"
#include "irrlicht.h"
// default constructor
WorldEntity::WorldEntity() : m_SceneNode(NULL), m_Mesh(NULL), m_Physics_Body(NULL){
	fx = 0;
	fy = 0;
	fz = 0;
}

// lets user set the id
WorldEntity::WorldEntity(int iID): m_SceneNode(NULL), m_Mesh(NULL), m_Physics_Body(NULL){
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
void WorldEntity::setMesh(irr::scene::IAnimatedMesh* newMesh){
	//Need to do something here to update everything else with the current mesh.  Especially the scene node.
	m_Mesh = newMesh;
}
// Sets location of entity
void WorldEntity::setLocation( float x, float y, float z){
	fx = x;
	fy = y;
	fz = z;

	if (m_Physics_Body){
		//m_SceneNode->setPosition(irr::core::vector3df(fx, fy, fz));
		m_Physics_Body->setPosition(irr::core::vector3df(fx, fy, fz));
	}

}