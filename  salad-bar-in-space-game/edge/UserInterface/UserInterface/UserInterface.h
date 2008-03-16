/* CSCE 552
 * UserInterface.h
 * A header file for the UserInterface class.
 */

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

// Includes the irrlicht header and iostream
#include <irrlicht.h>
#include <iostream>

// Allows the use of vectors
#include <vector>

// Include InputManager and ball classes
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
	// Private methods and variables
	private:
			// Score variable
			int playerScore;
			// Life variable
			int playerLives;
			// Player name
			string playerName;
			// Vector to hold all inputs
			vector<Input> inputs;
			// Current powers (for display in UI)
			vector<Power> powers;
			/**** Time? ****/
			/**** High Scores? ****/
			// Pointers to keyboard and wiimote devices
			Input::InputDevice *keyboard = InputManager::getKeyboard();
			Input::InputDevice *wiimote = InputManager::getWiimote();
			// Create pointers to Wiimote actions
			//*****ACTIONS FOR ANALOG**********//
			Input::Action *wmGrow = InputManager::createAction(
							wmGrow, *wiimote, WII_MOVE_HANDS_OUTWARD, BEHAVIOR_DETECT_PRESS),
						  *wmShrink = InputManager::createAction(
							wmShrink, *wiimote, WII_MOVE_HANDS_INWARD, BEHAVIOR_DETECT_PRESS),
						  *wmNormalJump = InputManager::createAction(
							wmNormalJump, *wiimote, WII_RIGHT_HAND_UP, BEHAVIOR_DETECT_PRESS),
						  *wmBigJump = InputManager::createAction(
							wmBigJump, *wiimote, WII_BOTH_HANDS_DOWN, BEHAVIOR_DETECT_PRESS),
						  *wmRightMomentum = InputManager::createAction(
							wmRightMomentum, *wiimote, WII_RIGHT_HAND_RIGHT, BEHAVIOR_DETECT_PRESS),
						  *wmLeftMomentum = InputManager::createAction(
							wmLeftMomentum, *wiimote, WII_RIGHT_HAND_LEFT, BEHAVIOR_DETECT_PRESS),
						  *wmHarden = InputManager::createAction(
							wmHarden, *wiimote, WII_Z_BUTTON, BEHAVIOR_DETECT_PRESS),
						  *wmHeliumize = InputManager::createAction(
							wmHeliumize, *wiimote, WII_C_BUTTON, BEHAVIOR_DETECT_PRESS),
						  *wmGrowSpikes = InputManager::createAction(
							wmGrowSpikes, *wiimote, WII_B_BUTTON, BEHAVIOR_DETECT_PRESS);
			// Create pointers to Keyboard actions
			//************NORMAL DIRECTIONS/MOVEMENTS*************//
			Input::Action *kbGrow = InputManager::createAction(
							kbGrow, *keyboard, KEY_1, BEHAVIOR_DETECT_PRESS),
						  *kbShrink = InputManager::createAction(
							kbShrink, *keyboard, KEY_2, BEHAVIOR_DETECT_PRESS),
						  *kbNormalJump = InputManager::createAction(
							kbNormalJump, *keyboard, KEY_W, BEHAVIOR_DETECT_PRESS),
						  *kbBigJump = InputManager::createAction(
							kbBigJump, *keyboard, KEY_S, BEHAVIOR_DETECT_PRESS),
						  *kbRightMomentum = InputManager::createAction(
							kbRightMomentum, *keyboard, KEY_E, BEHAVIOR_DETECT_PRESS),
						  *kbLeftMomentum = InputManager::createAction(
							kbLeftMomentum, *keyboard, KEY_Q, BEHAVIOR_DETECT_PRESS),
						  *kbHarden = InputManager::createAction(
							kbHarden, *keyboard, KEY_3, BEHAVIOR_DETECT_PRESS),
						  *kbHeliumize = InputManager::createAction(
							kbHeliumize, *keyboard, KEY_4, BEHAVIOR_DETECT_PRESS),
						  *kbGrowSpikes = InputManager::createAction(
							kbGrowSpikes, *keyboard, KEY_SPACE, BEHAVIOR_DETECT_PRESS);
			
	// Public methods and variables
	public:
			// Takes input and passes required action to Ball class
			void passInput();
			// Draws the User Interface
			void drawUI();
			// Updates the User Interface
			void updateUI();
			// Gets the current score
			int getScore();
			// Sets the score
			void setScore(int pScore);
			// Gets the current number of lives
			int getLives();
			// Sets the number of lives
			void setLives(int pLives);
			// Gets the player name
			int getPlayerName();
			// Sets the player name
			void setPlayerName(int pName);
}


#endif