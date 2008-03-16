/* CSCE 552
 * UserInterface.cpp
 * CPP file for the UserInterface class.
 */

// Includes
#include <irrlicht.h>
#include <iostream>
#include <vector>
#include <InputManager>
#include <ball>

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
	public void passInput() {
		// Detect press and call corresponding methods in ball class
		/*** Not sure if 1 is value for pressed state, or if this is
		 *** how I should be implementing this 
		 ***/
		/* Need to handle current state vs new state 
		 * i.e. become normal if small, become big if normal...
		 * Should this be done here or in changeState's implementation? 
		 */	
		// Assuming big = correct state name
		if( *grow.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(big);
		// Assuming small = correct state name
		if( *shrink.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(small);
		// Assuming 1 = normal jump
		if( *normalJump.BEHAVIOR_DETECT_PRESS == 1)
			ball::jump(1);
		// Assuming 2 = big jump
		if( *bigJump.BEHAVIOR_DETECT_PRESS == 1)
			ball::jump(2);
		if( *rightMomentum.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeVelocity(/*Not sure what to put here*/);
		if( *leftMomentum.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeVelocity(/*Not sure what to put here*/);
		// Assuming hard = correct state name
		if( *harden.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(hard);
		// Assuming helium  = correct state name
		if( *heliumize.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(helium);
		// Assuming spike  = correct state name
		if( *growSpikes.BEHAVIOR_DETECT_PRESS == 1)
			ball::changeState(spike);
	}

	// Draws the User Interface
	public void drawUI() {
		// Not sure what to put here
	}
	// Updates the User Interface
	public void updateUI() {
		// Not sure what to put here
	}
	// Gets the current score
	public int getScore() {
		return playerScore;
	}
	// Sets the score
	public void setScore(int pScore) {
		playerScore = pScore;
	}
	// Gets the current number of lives
	public int getLives() {
		return playerLives;
	}
	// Sets the number of lives
	public void setLives(int pLives) {
		playerLives = pLives;
	}
	// Gets the player name
	public int getPlayerName() {
		return playerName;
	}
	// Sets the player name
	public void setPlayerName(int pName) {
		playerName = pName;
	}
}