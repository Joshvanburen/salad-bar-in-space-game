#include "Common.h"
#include "GameIncludes.h"
// default constructor
WorldEntity::WorldEntity() : m_SceneNode(NULL), m_Mesh(NULL), m_Physics_Body(NULL){
	location.X = 0;
	location.Y = 0;
	location.Z = 0;
	m_Radius = 0.0f;
	m_EnableRotation = false;
	m_EnableMovement = true;
	m_MaxSpeed = 20;
	m_MaxSpeedSQ = 25;
	m_LevelMgr = LevelManager::getSingletonPtr();
}

// lets user set the id
WorldEntity::WorldEntity(int iID): m_SceneNode(NULL), m_Mesh(NULL), m_Physics_Body(NULL){
	setID(iID);
	location.X = 0;
	location.Y = 0;
	location.Z = 0;
	m_Radius = 0.0f;
	m_Mass = 1;
	m_LevelMgr = LevelManager::getSingletonPtr();
}

void WorldEntity::update(){
	if (m_EnableRotation){
		this->m_Physics_Body->setRotation(m_Rotation);
	}
	irr::core::vector3df position  = this->getSceneNode()->getPosition();



	if (position.X > m_LevelMgr->getCurrentLevel().getMaxX()){
		position.X =  m_LevelMgr->getCurrentLevel().getMaxX();
	}
	else if (position.Z > m_LevelMgr->getCurrentLevel().getMaxY()){
		position.Z = m_LevelMgr->getCurrentLevel().getMaxY();
	}
	else if (position.X < m_LevelMgr->getCurrentLevel().getMinX()){
		position.X = m_LevelMgr->getCurrentLevel().getMinX();
	}
	else if (position.Z < m_LevelMgr->getCurrentLevel().getMinY()){
		position.Z = m_LevelMgr->getCurrentLevel().getMinY();
	}

	this->getSceneNode()->setPosition(irr::core::vector3df(position.X,position.Y, position.Z));
}
WorldEntity::~WorldEntity(){

	if (m_Mesh){
		this->m_Mesh->drop();
		m_Mesh = NULL;
	}
	if (m_Physics_Body){
		m_Physics_Body->remove();
		m_Physics_Body = NULL;
		m_SceneNode = NULL;
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
	m_Radius /=4;
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