/* CSCE 552
 * UserInterface.h
 * Header file for the UserInterface class.
 */

// Includes the irrlicht header
#include "irrlicht.h"

// Allows the use of vectors
#include "vector"
#include "cstdio"


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
			std::string playerName;
			// Current powers (for display in UI)
			//vector<Power> powers;
			/**** Time? ****/
			/**** High Scores? ****/
			// Pointers to keyboard and wiimote devices
			Input::InputDevice *keyboard = InputManager::getKeyboard();
			Input::InputDevice *wiimote = InputManager::getWiimote();
			// Create pointers to Wiimote actions
			//*****ACTIONS FOR ANALOG**********//
			Input::Action *wmGrow = InputManager::createAction(
				wmGrow, *wiimote, Input::Wiimote::WII_MOVE_HANDS_OUTWARD, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *wmShrink = InputManager::createAction(
							wmShrink, *wiimote, Input::Wiimote::WII_MOVE_HANDS_INWARD, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *wmNormalJump = InputManager::createAction(
							wmNormalJump, *wiimote, Input::Wiimote::WII_RIGHT_HAND_UP, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *wmBigJump = InputManager::createAction(
							wmBigJump, *wiimote, Input::Wiimote::WII_BOTH_HANDS_DOWN, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *wmRightMomentum = InputManager::createAction(
							wmRightMomentum, *wiimote, Input::Wiimote::WII_RIGHT_HAND_RIGHT, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *wmLeftMomentum = InputManager::createAction(
							wmLeftMomentum, *wiimote, Input::Wiimote::WII_RIGHT_HAND_LEFT, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *wmHarden = InputManager::createAction(
							wmHarden, *wiimote, Input::Wiimote::WII_Z_BUTTON, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *wmHeliumize = InputManager::createAction(
							wmHeliumize, *wiimote, Input::Wiimote::WII_C_BUTTON, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *wmGrowSpikes = InputManager::createAction(
							wmGrowSpikes, *wiimote, Input::Wiimote::WII_B_BUTTON, Input::Action::BEHAVIOR_DETECT_PRESS);
			// Create pointers to Keyboard actions
			//************NORMAL DIRECTIONS/MOVEMENTS*************//
			// Full Qualifiers?
			Input::Action *kbGrow = InputManager::createAction(
							kbGrow, *keyboard, Input::Keyboard::KEY_1, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *kbShrink = InputManager::createAction(
							kbShrink, *keyboard, Input::Keyboard::KEY_2, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *kbNormalJump = InputManager::createAction(
							kbNormalJump, *keyboard, Input::Keyboard::KEY_W, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *kbBigJump = InputManager::createAction(
							kbBigJump, *keyboard, Input::Keyboard::KEY_S, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *kbRightMomentum = InputManager::createAction(
							kbRightMomentum, *keyboard, Input::Keyboard::KEY_E, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *kbLeftMomentum = InputManager::createAction(
							kbLeftMomentum, *keyboard, Input::Keyboard::KEY_Q, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *kbHarden = InputManager::createAction(
							kbHarden, *keyboard, Input::Keyboard::KEY_3, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *kbHeliumize = InputManager::createAction(
							kbHeliumize, *keyboard, Input::Keyboard::KEY_4, Input::Action::BEHAVIOR_DETECT_PRESS),
						  *kbGrowSpikes = InputManager::createAction(
							kbGrowSpikes, *keyboard, Input::Keyboard::KEY_SPACE, Input::Action::BEHAVIOR_DETECT_PRESS);
			
	// Public methods and variables
	public:
			// Updates UserInterface
			void update();
			// Draws UserInterface
			void draw();
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