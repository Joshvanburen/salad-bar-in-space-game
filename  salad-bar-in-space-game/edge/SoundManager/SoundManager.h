#pragma once
#include "Common.h"
#include "singleton.h"

class SoundManager;

namespace Sound{
	class Audio{
		friend class ::SoundManager;
	private:

		std::string m_Name;
		irrklang::ISound* m_pSound;
		//! Sound Constructor - takes both name and location
		Audio(const std::string& sName, const std::string& sFilename, bool is3D);
		//! Default Constructor
		Audio();

		~Audio();

		

		Audio(const Sound& rhs);
		Audio& operator=(const Audio& rhs);

		void destroy();

		//! Sets name of sound
		void setName( std::string sName );
	public:
		void setVolume(float newVolume);

		float getVolume();

		void setPosition(irrklang::ved3df& newPosition);

		irrklang::vec3df& getPosition();

		void setPaused(bool paused);

		void setLooped(bool looped);

		void setMaxDistance(float distance);

		void setMinDistance(float distance);

		//! Returns the name of the sound
		const std::string& getName(){return name;};

		void play(bool looped = false);

		void stop();
	};

	class SoundManagerInitException {};
	class AudioAlreadyExists {};
	class AudioDoestExist {};
	typedef std::map<std::string, Audio*> StrAudioMap
};
  
class SoundManager : CSingleton<SoundManager>{
	friend CSingleton<ScriptManager>;
	friend Sound::Audio;
public:
	void init();

	void addNewSounds(const std::string& XMLSoundDefinitions);

	void shutdown();

	void reset();

	void update();

	void removeAll();

	void removeSound(const Audio* sound);

	Sound::Audio* addSound(const std::string& soundName, const std::string& filename);

	//! Looks for the sound in queue and returns the name if found
	Sound::Audio* getSound( const std::string& soundName );
	//! Removes sound by name.
	void removeSound( const std::string& soundName );
	//! Clears all sounds from queue
	void clearAllSounds();
	
private: 
	SoundManager();
	~SoundManager();

	SoundManager(const SoundManager& rhs);
	SoundManager& operator=(const SoundManager& rhs);

	static irrklang::ISoundEngine* engine;
	void readInXML(const std::string& XMLFilename);

	std::string XMLSoundDefinition;
	
	//! Stores sounds in a map. 
	Sound::StrAudioMap m_AudioMap;

	Sound::StrAudioMap::iterator m_AudioItr;


}