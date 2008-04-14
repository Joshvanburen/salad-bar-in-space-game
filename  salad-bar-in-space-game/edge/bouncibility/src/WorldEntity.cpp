#include "WorldEntity.h"
#include "irrnewt.hpp"
#include "EntityManager.h"
#include "irrlicht.h"
// default constructor
WorldEntity::WorldEntity() : m_SceneNode(NULL), m_Mesh(NULL), m_Physics_Body(NULL){
	location.X = 0;
	location.Y = 0;
	location.Z = 0;
	m_Radius = 0.0f;
}

// lets user set the id
WorldEntity::WorldEntity(int iID): m_SceneNode(NULL), m_Mesh(NULL), m_Physics_Body(NULL){
	setID(iID);
	location.X = 0;
	location.Y = 0;
	location.Z = 0;
	m_Radius = 0.0f;
	m_Mass = 1;
}

WorldEntity::~WorldEntity(){
	if (m_SceneNode){
		this->m_SceneNode->drop();
		m_SceneNode = NULL;
	}
	if (m_Mesh){
		this->m_Mesh->drop();
		m_Mesh = NULL;
	}
	if (m_Physics_Body){
		m_Physics_Body->remove();
		m_Physics_Body = NULL;
	}
}
WorldEntity::WorldEntity(int iID, float x, float y, float z){
	setID(iID);
	setLocation(irr::core::vector3df(x,y,z));
	m_Radius = 0.0f;
}


float WorldEntity::getX(){

	return location.X;
}

float WorldEntity::getY(){
	return location.Y;
}
void WorldEntity::calculateBoundingSphere(){
	if (m_SceneNode == 0)
	{
		m_Radius = 0.0f;
		return;
	}

    using irr::core::vector3df;

    //use the Bounding box from the scene node to calculate the bounding sphere
    const irr::core::aabbox3df& bb = m_SceneNode->getTransformedBoundingBox();
    m_Radius = 0.5f*(bb.getExtent().X - bb.getCenter().X);
    m_Radius = irr::core::max_(m_Radius, 0.5f*(bb.getExtent().Y - bb.getCenter().Y) );
    //No need to check Z-extents of bounding box
}
void WorldEntity::move(){



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
	location.X = x;
	location.Y = y;
	location.Z = z;

	if (m_Physics_Body){
		//m_SceneNode->setPosition(irr::core::vector3df(fx, fy, fz));
		m_Physics_Body->setPosition(location);
	}

}

float WorldEntity::getBoundingSphereRadius(){
	return m_Radius;
}
void WorldEntity::setLocation(irr::core::vector3df newLocation){
	this->location = newLocation;
	if (m_Physics_Body){
		m_Physics_Body->setPosition(newLocation);
	}
}