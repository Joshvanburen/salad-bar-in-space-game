
#include "irrXML.h"	
#include "irrKlang.h"
#include "SoundManager.h"

using namespace irrklang;
using namespace Sound;

Sound::Audio::Audio(const std::string& sName, const std::string& sFilename, bool is3D) : m_pSound(NULL), m_Name(sName), m_Filename(sFilename){
	this->is3D = is3D;

	this->position = irrklang::vec3df(0.0,0.0,0.0);
	this->m_Sources.push_back(SoundManager::engine->addSoundSourceFromFile(m_Filename.c_str(), irrklang::ESM_AUTO_DETECT, true));


}

Sound::Audio::~Audio(){
	destroy();

}

void Sound::Audio::destroy(){
	if (m_pSound){
		m_pSound->drop();
	}
	m_pSound = NULL;
	m_Sources.clear();
}


void Sound::Audio::appendAudio(irrklang::ISoundSource* soundSource){
	this->m_Sources.push_back(soundSource);
}
void Sound::Audio::setVolume(float newVolume){
	SoundSourceList::iterator sourceEnd = m_Sources.end();

	for (m_SourcesItr = m_Sources.begin(); m_SourcesItr != sourceEnd; ++m_SourcesItr){
		(*m_SourcesItr)->setDefaultVolume(newVolume);
	}

}

float Sound::Audio::getVolume(){
	if (m_Sources.size() > 0){

		return m_Sources[0]->getDefaultVolume();
	}
	else{
		return 0.0;
	}
}

void Sound::Audio::setPosition(vec3df& newPosition){
	this->position = newPosition;
	if (m_pSound){

		m_pSound->setPosition(newPosition);
	}
}

void Sound::Audio::setPosition(float x, float y, float z){
	this->position = irrklang::vec3df(x, y, z);

	if (m_pSound){
		m_pSound->setPosition(position);
	}
}
vec3df Sound::Audio::getPosition(){
	return position;
}

void Sound::Audio::setPaused(bool paused){
	if (m_pSound){
		m_pSound->setIsPaused(paused);
	}
}

void Sound::Audio::setMaxDistance(float distance){
	SoundSourceList::iterator sourceEnd = m_Sources.end();

	for (m_SourcesItr = m_Sources.begin(); m_SourcesItr != sourceEnd; ++m_SourcesItr){
		(*m_SourcesItr)->setDefaultMaxDistance(distance);
	}


}

void Sound::Audio::setMinDistance(float distance){
	SoundSourceList::iterator sourceEnd = m_Sources.end();

	for (m_SourcesItr = m_Sources.begin(); m_SourcesItr != sourceEnd; ++m_SourcesItr){
		(*m_SourcesItr)->setDefaultMinDistance(distance);
	}
}

const std::string& Sound::Audio::getName(){
	return m_Name;
}
void Sound::Audio::play(bool looped){
	int value = rand() % m_Sources.size();
	if (is3D){
		m_pSound = SoundManager::engine->play3D(m_Sources[value], position, looped, false, true);
	}
	else{
		m_pSound = SoundManager::engine->play2D(m_Sources[value], looped, false, true);
	}
}

void Sound::Audio::stop(){
	if (m_pSound){

		m_pSound->stop();

	}
}

irrklang::ISoundEngine* SoundManager::engine = NULL;
SoundManager::SoundManager() {
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
				volume = xml->getAttributeValueAsFloat("volume");
				is3D = xml->getAttributeValueAsInt("3D");

				
				Sound::Audio* audio = new Sound::Audio(name, filename, (bool)is3D);

				audio->setVolume(volume);
				
				min_distance = xml->getAttributeValueAsFloat("distance_min");
				max_distance = xml->getAttributeValueAsFloat("distance_max");

				audio->setPosition(irrklang::vec3df(x, y, z));
				audio->setMinDistance(min_distance);
				audio->setMaxDistance(max_distance);

					
				

				
				
				m_AudioItr = m_AudioMap.find(name);

				//If the script is already in the map, print out and continue;
				if (m_AudioItr != m_AudioMap.end()){
					std::cout << "Sound with name: " << name << "already exists. Appending sound to current Audio object.\n";
					m_AudioItr->second->appendAudio(audio->m_Sources[0]);
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
	return;
	
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
//	engine->setListenerPosition();

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
	
	Sound::Audio* audio = new Sound::Audio(soundName, filename, is3D);

	if (m_AudioItr != m_AudioMap.end()){
		m_AudioItr->second->appendAudio(audio->m_Sources[0]);
		delete audio;
		return m_AudioItr->second;
	}
	else{
		m_AudioMap.insert(std::make_pair(soundName, audio));
		return audio;
	}
}

Sound::Audio* SoundManager::getSound(const std::string& soundName){
	m_AudioItr = m_AudioMap.find(soundName);

	//If the sound wasn't found, throw exception
	if (m_AudioItr == m_AudioMap.end()){
		throw Sound::AudioDoestExist();
	}
	return (m_AudioItr->second);

}


