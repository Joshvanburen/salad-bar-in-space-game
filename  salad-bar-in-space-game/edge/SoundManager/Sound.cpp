#include "Sound.h"

#include <string>

// Primary
Sound::Sound(std::string sName, std::string sLoc){
	setName( sName );
	setLoc( sLoc );
}

void Sound::setLoc( std::string sLoc ){
	loc = sLoc;
}

void Sound::setName( std::string sName ){
	name = sName;
}
	