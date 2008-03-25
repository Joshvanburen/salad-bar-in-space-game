#include "SoundManager.h"
#include <string>
#include <iostream>
#include "irrXML.h"	
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

SoundManager::SoundManager(){

   // start the sound engine with default parameters
   ISoundEngine* engine = createIrrKlangDevice();
}

SoundManager::~SoundManager(){
	engine->drop(); // delete engine
}

bool SoundManager::loadSound(const std::string &XMLFilename){

	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLFilename.c_str());

	if (!xml){
		std::cout << "There was an error loading the xml file " << XMLFilename << ".\n";
		return false;
	}

	while(xml && xml->read()){

		switch(xml->getNodeType()){

		case irr::io::EXN_TEXT:
			//No text nodes
			break;

		case irr::io::EXN_ELEMENT:
			if (!strcmp("script", xml->getNodeName())){
				name = xml->getAttributeValue("name");
				filename = xml->getAttributeValue("filename");

				script = new Scripting::Script(name, filename);
				
				m_ScriptItr = m_ScriptMap.find(name);

				//If the script is already in the map, print out and continue;
				if (m_ScriptItr != m_ScriptMap.end()){
					std::cout << "Sound with name: " << name << "already exists. Continuing...\n";
					delete script;
				}
				else{
					soundMap.insert(std::make_pair(name, script));
				}

			}
			break;
		}

	}

	delete xml;
	return true;


}

std::string SoundManager::getSound(const std::string &sound){

	return "?";
}

void SoundManager::playSound(const std::string &sound){

	
}
