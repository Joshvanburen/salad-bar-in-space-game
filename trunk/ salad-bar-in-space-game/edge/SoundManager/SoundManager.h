#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "Sound.h"
#include<map>
using namespace std;
#include<string>

class SoundManager{
	friend class Sound;
public:
	//ISoundEngine* engine = createIrrKlangDevice();

	SoundManager();
	~SoundManager();

	//! Plays a sound by name
	static void playSound( const std::string& sound );
	//! Looks for the sound in queue and returns the name if found
	std::string getSound( const std::string& sound );
	//! Removes sound by name.
	void removeSound( const std::string& sound );
	//! Clears all sounds from queue
	void clearAllSounds();
	
private:
	int maxSounds;
	//! Stores sounds in a map. 
	map<std::string, Sound > soundMap;
	
	//! Reads XML for parameters 
	static bool loadSound( const std::string& XMLFilename );

	ISoundEngine* engine;

};

#endif