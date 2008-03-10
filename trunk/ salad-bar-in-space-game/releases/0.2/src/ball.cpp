#include "irrlicht.h"
#include "irrnewt.hpp"
#include "ball.h"


void Ball::load(){
}

void Ball::update(){
	const irr::core::vector3df position  = this->m_Physics_Body->getPosition();
	this->m_Physics_Body->setPosition(irr::core::vector3df(position.X,position.Y, 0.0f));
}

void Ball::changeState(const std::string name){
}

void Ball::changeVelocity(float x_speed, float y_speed){
}

void Ball::jump(int jump_type){
}
Ball::Ball(){

}

Ball::~Ball(){
}