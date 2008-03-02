/* 
 * 
 * ball.h
 * 
 *
 */


#ifndef BALL_H
#define BALL_H

#include <utility>	//For using pair
#include "Common.h"
#include "WorldEntity.h"


class Ball : public WorldEntity {

public:

	// Constructor and destructor
	Ball();
	~Ball();

	void load();

	//Where story happens, check state, collision detect, physics staffs.
	void update();

	void draw();

	// Change ball state, all the other properties of the ball may be changed from here
	void changeState(const std::string name);

	// Change ball speed, no accleration right now.
	void changeVelocity(float x_speed, float y_speed);

	//Jump!! Yahooooo!
	//Right now we have two different kinds of jumps
	//jump will give velocity a y+ speed, which is depends on jump_type
	void jump(int jump_type);



private: 
 	
	//We are trying to use as few as possible parameters
	//These two parameter can describe many states
	//They always change together, unless in Helium state
	float size;
	float weight; 

	//How powerful the ball is
	int   attack;

	//2d speed and direction - Irrlicht probably has a better vector class for representing 2d vectors!
	std::pair<float, float> velocity;

	//I'll delete it in ball after it implemented in WorldEntity
	int currentState;

	//How much percents of speed left after bouncing
	int bouncibility;
};

#endif
