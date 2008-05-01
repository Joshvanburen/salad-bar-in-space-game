#include "Common.h"
#include "GameIncludes.h"

using namespace irr;
using namespace io;

void WorldEntityAIManager::shutdown(){

	m_AIFunctionMap.clear();

}
	
//void WorldEntityAIManager::update(){
//	
//	//m_World->update();
//	
//}

void WorldEntityAIManager::clear(){

	this->shutdown();



}

WorldEntityAIManager::WorldEntityAIManager(){
	
}

WorldEntityAIManager::~WorldEntityAIManager(){
	this->shutdown();
}

bool WorldEntityAIManager::init(){
	//m_Device = device;
	//m_World = irr::newton::createPhysicsWorld(device);

	




	m_AIDefinition = "./res/ai/global.xml";

	return readInXML(m_AIDefinition);


}

bool WorldEntityAIManager::readInXML(const std::string& XMLMaterialDefinition){
	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLMaterialDefinition.c_str());

	if (!xml){
		std::cout << "There was an error loading the xml file " << XMLMaterialDefinition << ".\n";
	}

	// strings for storing the data we want to get out of the file
	std::string type;
	std::string scriptName;
	int i = 0;
	char buffer [64];
	//std::string material2;
	//irr::newton::IMaterial* material;
	//irr::newton::IMaterial* second_material;
	//float softness = 0;
	//float kinetic_friction = 0;
	//float static_friction = 0;
	//float elasticity = 0;
	//std::string scriptName;
	//std::string soundName;
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case irr::io::EXN_TEXT:
			//No text nodes
			break;

		case irr::io::EXN_ELEMENT:
			if (!strcmp("ai_function", xml->getNodeName())){
				type = xml->getAttributeValue("type");
				scriptName = xml->getAttributeValue("script");

				Scripting::Script& script = ScriptManager::getSingleton().getScript(scriptName);
				if (!script.isLoaded()){
					script.load();
				}
				itoa(i, (char*)buffer, 10);
				//buffer = "entity_ai" + buffer;
				Scripting::WorldEntityAIFunction* script_function = new Scripting::WorldEntityAIFunction();
				ScriptManager::getSingleton().registerScriptFunction(buffer, script_function);

				Scripting::ScriptFunction* scriptFunction = script.addFunction(buffer);

				
				m_AIFunctionItr = m_AIFunctionMap.find(type);
				//If the ai type is already in the map, print out and continue;
				if (m_AIFunctionItr != m_AIFunctionMap.end()){
					std::cout << "AI with type: " << type << "already exists. Continuing...\n";
				}
				else{
					m_AIFunctionMap.insert(std::make_pair(type, (Scripting::WorldEntityAIFunction*)scriptFunction));
				}

				i++;
			}

			break;
		}

	}
	delete xml;

	return true;
}

Scripting::ScriptFunction* WorldEntityAIManager::getAI(const std::string type) {

	m_AIFunctionItr = m_AIFunctionMap.find(type);

	if (m_AIFunctionItr == m_AIFunctionMap.end()){
		std::cout << "AI with type: " << type << "not exists. Continuing...\n";
	}

	//std::cout << type << m_AIFunctionItr->first << "ai check ... \n";
	
	return m_AIFunctionItr->second;

}