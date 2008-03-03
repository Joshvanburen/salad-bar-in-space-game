/* 
 * 
 * ball.h
 * 
 *
 */


#ifndef BALL_H
#define BALL_H

#include <utility>	//For using pair
#include "irrlicht.h"
#include "irrnewt.hpp"

using namespace std;

#pragma comment(lib,"irrlicht.lib")
#pragma comment(lib,"irrnewt.lib")

using namespace irr;


class ball : public WorldEntity {

public:

	// tell the ball to go left or right
	// UserInterface just set one of them true, if player press left or right button
	bool left;
	bool right;

	// tell the ball whether and how to jump
	// set it 1 - player just did a normal jump
	// set it 2 - player just did a big jump
	int jump;

	// Constructor and destructor
	ball();
	~ball();

	//init the ball, all propoties, from script
	bool init();

	//void load();
	//void draw(float fElapsedTime);

	//Where story happens, check state, collision detect, physical staffs.
	void update(float fElapsedTime);


	//get ball state
	state getState() { 
		return currentState;
	}

	//set ball state
	bool setState(state next_state);

	// Get the position of the ball
	float getPosition_x() {
		return ball_node->getPosition().X;
	}

	float getPosition_y() {
		return ball_node->getPosition().Y;
	}

	// set the position of the ball
	void setPosition(float x, float y) {
		ball_node->setPosition(core::vector3df(x,y,z_fix));
	}
	
	// Get the Velocity of the ball
	float getVelocity_x() {
		return ball_node->getVelocity().X;
	}

	float getVelocity_y(){
		return ball_node->getVelocity().Y;
	}

	// set ball speed, no accleration right now.
	//PLAYER CAN'T DO IT DIRECTLY
	//void setVelocity(float x_speed, float y_speed);

	// get and set HP of the ball
	int getHp() {
		return hp;
	}

	void setHp(int dHp);


	//Jump!! Yahooooo!
	//Right now we have two different kinds of jumps
	//jump will give velocity a y+ speed, which is depends on jump_type
	//void jump(int jump_type);



private: 
 	
	//scene node for the irrlicht
	scene::ISceneNode* ball_node;
	
	//body for irrnewt
	irr::newton::ICharacterController* ball_body;
	
	//material for irrnewt
	irr::newton::IMaterial* ball_material

	//z value of tha ball, can not be changed during the game
	float z_fix;

	//We are trying to use as few as possible parameters
	//These two parameter can describe many states
	//They always change together, unless in Helium state
	float size;
	float weight; 

	//How powerful the ball is
	int   attack;

	//The rest HP of the ball
	int   hp;
	int	  max_hp;

	//maximum speed at x direction
	float maxVelocity_x;

	//I'll delete it in ball after it implemented in WorldEntity
	state currentState;

	//How much percents of speed left after bouncing
	float bouncibility;
	float friction;
	float softness;

	float move_force;
	float jump_force1;
	float jump_force2;
}

#endif
