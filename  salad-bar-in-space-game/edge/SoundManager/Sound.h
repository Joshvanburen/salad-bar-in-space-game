#ifndef SOUND_H
#define SOUND_H

#include <string>

class Sound{
private:

	//! Sound Constructor - takes both name and location
	Sound( std::string sName, std::string sLoc );
	//! Default Constructor
	Sound();

	~Sound();

	//! sets location of file to be played in sound class
	void setLoc( std::string );
	//! Location of sound file
	std::string getLoc(){return loc;};
	//! Returns the name of the sound
	std::string getName(){return name;};
	//! Sets name of sound
	void setName( std::string sName );
	//! name of string
	std::string name;
	//! location of file
	std::string loc;
};

#endif