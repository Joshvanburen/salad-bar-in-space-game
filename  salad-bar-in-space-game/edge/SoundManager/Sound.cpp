#include "Sound.h"

#include <string>

// Shouldn't normally be called
Sound::Sound(){
	setName("default");
	setLoc("None");
}

// Shouldn't normally be called
Sound::Sound(std::string sName){
	setName(sName);
	setLoc("None");
}

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
	