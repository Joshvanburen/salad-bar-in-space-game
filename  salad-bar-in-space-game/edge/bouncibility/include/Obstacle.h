/*
Obstacle header file
obstacle.h

- JV
*/

#pragma once
#include <utility>

//Includes header files
#include "Common.h"
#include "WorldEntity.h"

//Includes the Obstacle factory class in the entity namespace
namespace Entity{
class ObstacleFactory;
};

//Defines the obstacle class
class Obstacle : public WorldEntity{

	//Defines ObstacleFactory as a friend class, which allows ObstacleFactory access to all Obstacle class methods
	friend class Entity::ObstacleFactory;

//Public Methods and variables
public:
	//Constructor and destructor
	Obstacle();
	~Obstacle();
	//Loads the obstacle
	void load();
	//Returns a pointe to an obstacle when given a world entity pointer
	static Obstacle* EntityToObstacle(WorldEntity*);
	//Update by checking physics, collission, etc..
	void update();
	//Draws the obstacle
	void draw();
	// Changes properties of the obstacle
	void changeState(const std::string name);
	//Returns whether or not the obstacle is destructable
	bool isDestructable();
	//Gets the height of the obstacle
	float getHeight();
	//Gets the width of the obstacle
	float getWidth();
	//Gets the depth of the obstacle
	float getDepth();
	//Gets the weight of the obstacle
	float getWeight();
	//Sets the height
	void setHeight(float h);
	//Sets the width
	void setWidth(float w);
	//Sets the depth
	void setDepth(float d);
	//Sets the weight
	void setWeight(float w);
	//Sets destructable or not
	void setDestructable(bool d);
	
//Private methods and variables
private:
	//Basically clones the object and returns a world entity pointer
	WorldEntity* clone();
	//Height
	float height;
	//Width
	float width;
	//Depth
	float depth;
	//Weight
	float weight;
	//Whether or not the object is destructable
	bool destructable;
	//Current state
	std::string currentState;
};
