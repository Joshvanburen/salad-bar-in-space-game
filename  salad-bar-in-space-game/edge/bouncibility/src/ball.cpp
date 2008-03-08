#include "irrlicht.h"
#include "ball.h"


void Ball::load(){
}

void Ball::update(){
	const irr::core::vector3df position  = this->sceneNode->getPosition();
	this->sceneNode->setPosition(irr::core::vector3df(position.X,position.Y, 0.0f));
}

void Ball::changeState(const std::string name){
}

void Ball::changeVelocity(float x_speed, float y_speed){
}

void Ball::jump(int jump_type){
}
Ball::Ball(){
	this->move_force = 10.0f;
	this->jump_force = 20.0f;
	this->gravity_force=-20.0f;
}

Ball::~Ball(){
}