#include "Common.h"
#include "GameIncludes.h"

//Constructor
Obstacle::Obstacle() : WorldEntity()
{
	//By default obstacles are not destructable
	destructable = false;
}

//Destructor
Obstacle::~Obstacle()
{
}

//Load method
void Obstacle::load()
{
}

//Update method to be used if the obstacle should be moved or something for some odd reason
void Obstacle::update()
{
	WorldEntity::update();
	//Gets the current physics position of the object as a 3d float vector
	const irr::core::vector3df position  = this->m_Physics_Body->getPosition();
	
	//Sets the position of the object
	this->m_Physics_Body->setPosition(irr::core::vector3df(position.X,position.Y, 0));
}

//Returns a obstacle pointer
Obstacle* Obstacle::EntityToObstacle(WorldEntity* entity)
{
	//Returns a obstacle pointer
	return dynamic_cast<Obstacle*>(entity);
}

//Clone method
WorldEntity* Obstacle::clone()
{
	//Creates a new instance of Obstacle
	Obstacle* entity = new Obstacle();
	
	//Copies the location of the object
	entity->location = this->location;
	
	//Sets the ID
	entity->id = -1;
	
	//Copies the current state
	entity->currentState = this->currentState;
	
	//If the mesh exists,  get it
	if(m_Mesh)
	{
		//Grabs the mesh
		this->m_Mesh->grab();
	}
	
	//Copies the mesh
	entity->m_Mesh = this->m_Mesh;
	
	//Copies the radius
	entity->m_Radius = this->m_Radius;
	
	//Clones the scene node
	entity->m_SceneNode = this->m_SceneNode->clone();

	//Physics node
	irr::newton::SBodyFromNode physics_node;
	
	//Sets the physics node
	physics_node.Node = entity->m_SceneNode;
	
	//Sets the node type
	//I think a box may be more appropriate, but I can't find the types of primitives in the documentation
	physics_node.Type = irr::newton::EBT_PRIMITIVE_ELLIPSOID;

	//Sets the character controller
	irr::newton::ICharacterController* body = m_Physics_Body->getWorld()->createCharacterController(m_Physics_Body->getWorld()->createBody(physics_node));
	
	//Sets the rotation update 
	body->setRotationUpdate(true);
	
	//Continuos collissions
	body->setContinuousCollisionMode(true);

	//Sets the material
	body->setMaterial(m_Physics_Body->getMaterial());

	//Sets user data
	body->setUserData(entity);

	//Adds the continuos force of gravity
	body->addForceContinuous(irr::core::vector3df(0,PhysicsManager::getSingleton().getGravity(),0));
	
	//Sets the physics body
	entity->m_Physics_Body = body;
	
	//Sets the weight
	entity->weight = this->weight;

	//Returns the pointer to the obstacle object
	return entity;
}

//Changes the state
void Obstacle::changeState(const std::string name)
{
}

//Sets the height
void Obstacle::setHeight(float h)
{
	//Sets the height
	height = h;
}

//Sets the width
void Obstacle::setWidth(float w)
{
	//Sets the width
	width = w;
}

//Sets the depth
void Obstacle::setDepth(float d)
{
	//Sets the depth
	depth = d;
}

//Sets destructable
void Obstacle::setDestructable(bool d)
{
	//Sets destructable
	destructable = d;
}

//Gets the height
float Obstacle::getHeight()
{
	//Returns the height
	return height;
}

//Gets the width
float Obstacle::getWidth()
{
	//Returns the width
	return width;
}

//Gets the depth
float Obstacle::getDepth()
{
	//Returns the depth
	return depth;
}

//Is the object destructable
bool Obstacle::isDestructable()
{
	//Returns destructable
	return destructable;
}