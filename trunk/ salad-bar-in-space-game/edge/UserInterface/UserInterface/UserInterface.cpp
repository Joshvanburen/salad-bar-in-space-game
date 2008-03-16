/* CSCE 552
 * UserInterface.cpp
 * CPP file for the UserInterface class.
 */

// Includes
#include <irrlicht.h>
#include <iostream>
#include <vector>
//#include <InputManager>
//#include <ball>

// Irrlicht namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// Links with the irrlicht library file
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

// Class definition
class UserInterface
{
	// not sure how to use these
	//singleton = InputManager::getSingleton();
	//singletonPtr = InputManager::getSingletonptr();
	
	// Takes input and passes required action to Ball class
	void passInput() {
		// Detect press and call corresponding methods in ball class
		/*** Not sure if 1 is value for pressed state, or if this is
		 *** how I should be implementing this 
		 ***/
		/* Need to handle current state vs new state 
		 * i.e. become normal if small, become big if normal...
		 * Should this be done here or in changeState's implementation? 
		 */

		// WiiMote Controls
		// Assuming big = correct state name
		if( *wmGrow.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(big);
		// Assuming small = correct state name
		if( *wmShrink.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(small);
		// Assuming 1 = normal jump
		if( *wmNormalJump.BEHAVIOR_DETECT_PRESS == 1)
			ball::jump(1);
		// Assuming 2 = big jump
		if( *wmBigJump.BEHAVIOR_DETECT_PRESS == 1)
			ball::jump(2);
		if( *wmRightMomentum.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeVelocity(/*Not sure what to put here*/);
		if( *wmLeftMomentum.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeVelocity(/*Not sure what to put here*/);
		// Assuming hard = correct state name
		if( *wmHarden.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(hard);
		// Assuming helium  = correct state name
		if( *wmHeliumize.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(helium);
		// Assuming spike  = correct state name
		if( *wmGrowSpikes.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(spike);

		// Keyboard Controls
		// Assuming big = correct state name
		if( *kbGrow.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(big);
		// Assuming small = correct state name
		if( *kbShrink.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(small);
		// Assuming 1 = normal jump
		if( *kbNormalJump.BEHAVIOR_DETECT_PRESS == 1)
			ball::jump(1);
		// Assuming 2 = big jump
		if( *kbBigJump.BEHAVIOR_DETECT_PRESS == 1)
			ball::jump(2);
		if( *kbRightMomentum.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeVelocity(/*Not sure what to put here*/);
		if( *kbLeftMomentum.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeVelocity(/*Not sure what to put here*/);
		// Assuming hard = correct state name
		if( *kbHarden.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(hard);
		// Assuming helium  = correct state name
		if( *kbHeliumize.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(helium);
		// Assuming spike  = correct state name
		if( *kbGrowSpikes.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(spike);
	}

	// Draws the User Interface
	void drawUI() {
		// Not sure what to put here
	}
	// Updates the User Interface
	void updateUI() {
		// Not sure what to put here
	}
	// Gets the current score
	int getScore() {
		return playerScore;
	}
	// Sets the score
	void setScore(int pScore) {
		playerScore = pScore;
	}
	// Gets the current number of lives
	int getLives() {
		return playerLives;
	}
	// Sets the number of lives
	void setLives(int pLives) {
		playerLives = pLives;
	}
	// Gets the player name
	int getPlayerName() {
		return playerName;
	}
	// Sets the player name
	void setPlayerName(int pName) {
		playerName = pName;
	}
}