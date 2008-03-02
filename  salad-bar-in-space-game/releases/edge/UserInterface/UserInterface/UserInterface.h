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

// Irrlicht namespace
using namespace irr;

// Links with the irrlicht library file
#pragma comment(lib, "Irrlicht.lib")

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
			/* I was told to include Ball as well, I'm not sure what the
			 * purpose of this is, or whether it should a variable or a method
			 */
	// Public methods and variables
	public:
			// Takes input and passes required action to Ball class
			/**** Do we have an input class? ****/
			void passInput(Input input);
			// Constructor
			/**** Do I need one of these? ****/
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