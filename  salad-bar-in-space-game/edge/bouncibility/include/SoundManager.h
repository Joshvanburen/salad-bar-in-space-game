#pragma once
#include "Common.h"
#include "irrklang.h"
#include "singleton.h"

class SoundManager;

namespace Sound{
	typedef std::vector<irrklang::ISoundSource*> SoundSourceList;
	class Audio{
		friend class ::SoundManager;
	private:
		bool is3D;
		std::string m_Name;
		std::string m_Filename;
		
		SoundSourceList m_Sources;

		SoundSourceList::iterator m_SourcesItr;
		irrklang::ISound* m_pSound;
		irrklang::vec3df position;
		//! Sound Constructor - takes both name and location
		Audio(const std::string& sName, const std::string& sFilename, bool is3D);
		//! Default Constructor
		Audio();

		~Audio();

		void appendAudio(irrklang::ISoundSource* soundSource);		

		Audio(const Audio& rhs);
		Audio& operator=(const Audio& rhs);

		void destroy();

		//! Sets name of sound
		void setName( std::string sName );
	public:
		void setVolume(float newVolume);

		float getVolume();
		
		bool isPlaying();


		void setPosition(irrklang::vec3df& newPosition);

		void setPosition(float x, float y, float z);
		irrklang::vec3df getPosition();

		void setPaused(bool paused);

		void setMaxDistance(float distance);

		void setMinDistance(float distance);

		//! Returns the name of the sound
		const std::string& getName();

		void play(bool looped = false);

		void stop();
	};

	class SoundManagerInitException {};
	class AudioAlreadyExists {};
	class AudioDoestExist {};
	typedef std::map<std::string, Audio*> StrAudioMap;
};
  
class SoundManager : public CSingleton<SoundManager>{
	friend CSingleton<SoundManager>;
	friend Sound::Audio;
public:
	void init();

	void addNewSounds(const std::string& XMLSoundDefinitions);

	void shutdown();

	void reset();

	void update();

	void removeAll();

	void removeSound(Sound::Audio* sound);

	Sound::Audio* addSound(const std::string& soundName, const std::string& filename, bool is3D);

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


};