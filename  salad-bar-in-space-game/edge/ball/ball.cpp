#include ".\ball.h"


//These two will depends on worldEntity
ball::ball(){
}

ball::~ball(){
}


bool ball::init(){

	//Read script ,set all these parameters
	//ball_node;
	//ball_body;
	//ball_material;
	//z_fix;
	//size;
	//weight; 
	//attack;
	//hp;
	//max_hp;
	//maxVelocity_x;
	//currentState;
	//bouncibility;
	//friction;
	//softness;
	//move_force
	//jump_force1
	//jump_force2

	left = 0;
	right = 0;
	jump = 0;

}


void ball::update(float fElapsedTime){

	if(left_) //give force left
		ball_body->addForce(core::vector3df(-move_force,0,0));
	if(right_)//give force right
		ball_body->addForce(core::vector3df(move_force,0,0));

	switch (jump)
		case 1: {
			ball_body->jump(jump_force1);
			break;
		}
		case 2: {
			ball_body->jump(jump_force2);
			break;
		}
		default:{}


	left = 0;
	right = 0;
	jump = 0;

}	



//set ball state
bool ball::setState(state next_state){

}


void ball::setHp(int dHp){
	
	if (dHp > 0)
		if (dHp > max_hp)
			ball.hp = max_hp;
		else
			ball.hp = dHp;

}


//Right now we have two different kinds of jumps
//jump will give velocity a y+ speed, which is depends on jump_type
//void ball::jump(int jump_type){


//}
