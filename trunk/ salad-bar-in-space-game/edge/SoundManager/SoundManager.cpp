
#include "irrXML.h"	
#include "irrKlang.h"
#include "SoundManager.h"

using namespace irrklang;
using namespace Sound;

Sound::Audio::Audio(const std::srring& sName, const std::string& sFilename, bool is3D) : name(sName), filename(sFilename), m_pSound(NULL){
	if(is3D){
		this->m_pSound = SoundManager::engine->play3D(filename, irrklang::vec3df(0, 0, 0), false, true, true);
		
	}
	else{
		this->m_pSound = SoundManager::engine->play2D(filename, false, true, true);
	}

}

Sound::Audio::~Audio(){
	destroy();

}

void Sound::Audio::destroy(){
	if (m_pSound){
		m_pSound->drop();
	}
	m_pSound = NULL;
}


void Sound::Audio::setVolume(float newVolume){
	m_pSound->setVolume(newVolume);
}

float Sound::Audio::getVolume(){

	return m_pSound->getVolume();
}

void Sound::Audio::setPosition(vec3df& newPosition){
	m_pSound->setPosition(newPosition);
}
vec3df& Sound::Audio::getPosition(){
	return m_pSound->getPosition();
}

void Sound::Audio::setPaused(bool paused){
	m_pSound->setIsPaused(paused);
}

void Sound::Audio::setLooped(bool looped){
	m_pSound->setIsLooped(looped);
}

void Sound::Audio::setMaxDistance(float distance){
	m_pSound->setMaxDistance(distance);
}

void Sound::Audio::setMinDistance(float distance){
	m_pSound->setMinDistance(distance);
}

const std::string& Sound::Audio::getName(){
	return name;
}
void Sound::Audio::play(bool looped){
	m_pSound->setIsLooped(looped);
	m_pSound->setPlayPosition(0);
	m_pSound->setIsPaused(false);
}

void Sound::Audio::stop(){
	m_pSound->stop();
}

SoundManager::SoundManager() : {
	engine = NULL;
}

SoundManager::~SoundManager(){
	this->shutdown();
}

void SoundManager::readInXML(const std::string& XMLFilename){
	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLFilename.c_str());

	if (!xml){
		std::cout << "There was an error loading the xml file " << XMLFilename << ".\n";
		throw Sound::SoundManagerInitException();
	}

	std::string name;
	std::string filename;
	int looped = 0;
	float volume = 0.0;
	int is3D = 0;
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	float min_distance = 0.0;
	float max_distance = 5.0;

	while(xml && xml->read()){

		switch(xml->getNodeType()){

		case irr::io::EXN_TEXT:
			//No text nodes
			break;

		case irr::io::EXN_ELEMENT:
			if (!strcmp("sound", xml->getNodeName())){
				name = xml->getAttributeValue("name");
				filename = xml->getAttributeValue("filename");
				looped = xml->getAttributeValueAsInt("looped");
				volume = xml->getAttributeValueAsFloat("volume");
				is3D = xml->getAttributeValueAsInt("3D");

				
				Sound::Audio* audio = new Sound::Audio(name, filename, (bool)is3D);

				audio->setLooped((bool)looped);
				audio->setVolume(volume);
				
				if (is3D){
					x = xml->getAttributeValueAsFloat("x");
					y = xml->getAttributeValueAsFloat("y");
					z = xml->getAttributeValueAsFloat("z");
					min_distance = xml->getAttributeValueAsFloat("distance_min");
					max_distance = xml->getAttributeValueAsFloat("distance_max");

					audio->setPosition(irrklang::vec3df(x, y, z));
					audio->setMinDistance(min_distance);
					audio->setMaxDistance(max_distance);

					
				}

				
				
				m_AudioItr = m_AudioMap.find(name);

				//If the script is already in the map, print out and continue;
				if (m_AudioItr != AudioMap.end()){
					std::cout << "Sound with name: " << name << "already exists. Continuing...\n";
					delete audio;
				}
				else{
					m_AudioMap.insert(std::make_pair(name, audio));
				}

			}
			break;
		}

	}

	delete xml;
	return true;
	
}
void SoundManager::init(){
	engine = createIrrKlangDevice();

	if (!engine){
		throw SoundManagerInitException();
	}

	//Load in global sounds

	readInXML("./res/sounds/global.xml");
}

void SoundManager::addNewSounds(const std::string& XMLSoundDefinitions){
	readInXML(XMLSoundDefinitions);
}

void SoundManager::shutdown(){
	removeAll();
	
	engine->drop();
}

void SoundManager::reset(){

		this->shutdown();
		this->init();
}

void SoundManager::update(){
	//need to get properties from the main camera.
	//Set the current listener position to the camera or the ball or whatever each time update is called.
	engine->setListenerPosition();

}

void SoundManager::removeAll(){
	Sound::StrAudioMap::iterator audioItrEnd = m_AudioMap.end();

	for (m_AudioItr = m_AudioMap.begin(); m_AudioItr != audioItrEnd; ++m_AudioItr){
		m_AudioItr->second->destroy();
		delete m_AudioItr->second;
	}
	engine->removeAllSoundSources();
	m_AudioMap.clear();
}

void SoundManager::removeSound(Sound::Audio* sound){
	sound->destroy();
	delete sound;
	m_AudioMap.erase(sound->getName());
}

void SoundManager::removeSound(const std::string& soundName){
	m_AudioItr = m_AudioMap.find(soundName);

	if (m_AudioItr == m_AudioMap.end()){
		//sound doesn't exist, so don't remove anything
		return;
	}
	m_AudioItr->second->destroy();
	delete m_AudioItr->second;

	m_AudioMap.erase(m_AudioItr);
	
}

Sound::Audio* SoundManager::addSound(const std::string& soundName, const std::string& filename, bool is3D){
	m_AudioItr = m_AudioMap.find(soundName);

	if (m_AudioItr != m_AudioMap.end()){
		throw Sound::AudioAlreadyExists();
	}

	Sound::Audio* audio = new Sound::Audio(soundName, filename, is3D);
	
	m_AudioMap.insert(std::make_pair(soundName, audio));
	
	return audio;
}

Sound* SoundManager::getSound(const std::string& soundName){
	m_AudioItr = m_AudioMap.find(soundName);

	//If the sound wasn't found, throw exception
	if (m_AudioItr == m_AudioMap.end()){
		throw Sound::AudioDoesntExist();
	}
	return *(m_AudioItr->second);

}


