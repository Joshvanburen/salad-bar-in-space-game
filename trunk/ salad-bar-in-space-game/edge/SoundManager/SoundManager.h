#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include<map>
#include<string>
#include "irrKlang.h"
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

class SoundManager{

public:
	//ISoundEngine* engine = createIrrKlangDevice();

	SoundManager();
	~SoundManager();

	static void playSound( const std::string& sound );
	std::string getSound( const std::string& sound );
	
private:

	//! Stores sounds in a map. Priority is given to sounds to better manage them.
	map<std::string, ISoundSource, int> sound;
	
	//! Reads XML for parameters 
	static void loadSound( const std::string& XMLFilename );

};

#endif