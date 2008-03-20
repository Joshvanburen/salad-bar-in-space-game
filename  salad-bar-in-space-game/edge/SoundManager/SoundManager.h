#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

#include<map>
#include<string>

class SoundManager{

public:
	//ISoundEngine* engine = createIrrKlangDevice();

	SoundManager();
	~SoundManager();

	static void playSound( const string& sound );
	string getSound( const string& sound );
	
private:

	//! Stores sounds in a map. Priority is given to sounds to better manage them.
	map<string, sound, int priority> sound;
	
	//! Reads XML for parameters 
	static void loadSound( const string& XMLFilename );

}

#endif