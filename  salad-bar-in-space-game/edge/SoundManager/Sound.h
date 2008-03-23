#ifndef SOUND_H
#define SOUND_H

#include <string>

class Sound{
private:

	//! Sound Constructor
	Sound( std::string sName );
	//! Default Constructor
	Sound();

	~Sound();
	//! plays sound
	void play( std::string sound);
	//! Returns the name of the sound
	std::string getName(){return name;};
	//! Sets name of sound
	void setName( std::string sName );

	std::string name;


};

#endif