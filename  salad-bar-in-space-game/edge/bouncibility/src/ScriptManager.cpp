#include "Common.h"
#include "angelscript.h"
#include "scriptstring.h"
#include "irrXML.h"	
#include "ScriptManager.h"

void MessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if( msg->type == asMSGTYPE_WARNING ) 
		type = "WARN";
	else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = "INFO";
	std::cout << msg->section << " (" << msg->row << ", " << msg->col << ") : " << type << " : " << msg->message << "\n";
}

Scripting::MaterialCollisionFunction::MaterialCollisionFunction(){
	this->m_Signature = "void collide()";
}

void Scripting::MaterialCollisionFunction::callFunction() {

	ScriptFunction::callFunction();

}

Scripting::ScriptFunction::ScriptFunction() : m_ID(-1), m_pContext(NULL){
	 m_Signature = "void main()";
}

Scripting::ScriptFunction::~ScriptFunction(){

}

void Scripting::ScriptFunction::callFunction(){
	int r = m_pContext->Prepare(m_ID);
	if( r < 0 ) 
	{
		std::cout << "Failed to prepare the context. Unable to execute script function: " << m_Signature << std::endl;
	}
	
	execute();
}
void Scripting::ScriptFunction::execute(){
	int r = m_pContext->Execute();
	
	if( r != asEXECUTION_FINISHED )
	{
		// The execution didn't finish as we had planned. Determine why.
		
		if( r == asEXECUTION_ABORTED )
			std::cout << "The script was aborted before it could finish. Probably it timed out." <<  std::endl;
		else if( r == asEXECUTION_EXCEPTION )
		{
			std::cout << "The script ended with an exception." <<  std::endl;

			// Write some information about the script exception
			int funcID = m_pContext->GetExceptionFunction();
			std::cout << "func: " << ScriptManager::getEngine()->GetFunctionDeclaration(m_ID) << std::endl;
			std::cout << "modl: " << ScriptManager::getEngine()->GetFunctionModule(m_ID) << std::endl;
			std::cout << "sect: " << ScriptManager::getEngine()->GetFunctionSection(m_ID) << std::endl;
			std::cout << "line: " << m_pContext->GetExceptionLineNumber() << std::endl;
			std::cout << "desc: " << m_pContext->GetExceptionString() << std::endl;
		}
		else
			std::cout << "The script ended for some unforeseen reason (" << r << ")." << std::endl;
	}
	else
	{
		// Retrieve the return value from the context
	}

}


Scripting::ScriptFunction::ScriptFunction(const ScriptFunction& rhs){
	this->m_ID = rhs.m_ID;
	this->m_pContext = NULL;
	this->m_Signature = rhs.m_Signature;
}



Scripting::Script::Script(const std::string& sName, const std::string& sFilename) : loaded(false), m_pContext(NULL){
	m_Name = sName;
	m_Filename = sFilename;
}

Scripting::Script::~Script(){
	this->unload();
}

int Scripting::Script::load(){
	if (!loaded){

		FILE					*stream = NULL;
		std::string				txt;
		int						size = 0;

		txt.clear();

		stream = fopen(m_Filename.c_str(), "rb");
		if (!stream)
		{
			throw Scripting::ScriptLoadingFailure();
		}

		fseek(stream, 0, SEEK_END);
		size = (int)ftell(stream);
		fseek(stream, 0, SEEK_SET);

		txt.resize(size);

		fread(&txt[0], size, 1, stream);
		fclose(stream);
		stream = NULL;

		if (ScriptManager::getEngine()->AddScriptSection(m_Filename.c_str(), m_Name.c_str(), &txt[0], size, 0) < 0)
		{
			throw Scripting::ScriptLoadingFailure();

		}

		if (ScriptManager::getEngine()->Build(m_Filename.c_str()) < 0)
		{
			throw Scripting::ScriptLoadingFailure();
			
		}

		m_pContext = ScriptManager::getEngine()->CreateContext();

		if (!m_pContext)
		{
			throw Scripting::ScriptLoadingFailure();
		
		}
	}
	loaded = true;
	return 0;
}

void Scripting::Script::unload(){
	
	if (loaded){
		Scripting::StrFunctionMap::iterator functionItrEnd = m_FunctionMap.end();

		for (m_FunctionItr = m_FunctionMap.begin(); m_FunctionItr != functionItrEnd; ++m_FunctionItr){
			delete m_FunctionItr->second;
		}

		m_FunctionMap.clear();

		ScriptManager::getEngine()->Discard(m_Filename.c_str());

		if (m_pContext){
			this->m_pContext->Release();
			m_pContext = NULL;
		}
	}
	loaded = false;
}

Scripting::ScriptFunction* Scripting::Script::addFunction(const std::string& name){

	m_FunctionItr = m_FunctionMap.find(name);

	if (m_FunctionItr == m_FunctionMap.end()){
		Scripting::ScriptFunction* function = ScriptManager::getSingleton().createScriptFunction(name);
		function->m_pContext = this->m_pContext;
		function->m_ID = ScriptManager::getEngine()->GetFunctionIDByDecl(m_Filename.c_str(), function->m_Signature.c_str());
		m_FunctionMap.insert(std::make_pair(name,function));
		return function;
	}
	else{
		throw FunctionAlreadyRegistered();
	}
}


void Scripting::Script::removeFunction(const std::string& name){
	m_FunctionItr = m_FunctionMap.find(name);

	//If the function wasn't found, throw exception
	if (m_FunctionItr == m_FunctionMap.end()){
		throw Scripting::FunctionDoesntExist();
	}

	delete m_FunctionItr->second;

	m_FunctionMap.erase(m_FunctionItr);
}

Scripting::ScriptFunction* Scripting::Script::getFunction(const std::string& name){
	m_FunctionItr = m_FunctionMap.find(name);

	//If the function wasn't found, throw exception
	if (m_FunctionItr == m_FunctionMap.end()){
		throw Scripting::FunctionDoesntExist();
	}

	return ((*m_FunctionItr).second);
}


asIScriptEngine* ScriptManager::s_Engine = NULL;

bool ScriptManager::init(){
	s_Engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	if( s_Engine == 0 )
	{
		std::cout << "Failed to create script engine." << std::endl;
		return false;
	}

	// The script compiler will write any compiler messages to the callback.
	s_Engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, ::asCALL_CDECL);

	RegisterScriptString(s_Engine);

	m_ScriptDefinition = "./res/scripting/global.xml";

	readInXML(m_ScriptDefinition);
	return true;

}


bool ScriptManager::readInXML(const std::string& XMLScriptDefinition){
	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(XMLScriptDefinition.c_str());

	if (!xml){
		std::cout << "There was an error loading the xml file " << XMLScriptDefinition << ".\n";
		return false;
	}

	// strings for storing the data we want to get out of the file
	std::string name;
	std::string filename;


	Scripting::Script* script;

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
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
					std::cout << "Script with name: " << name << "already exists. Continuing...\n";
					delete script;
				}
				else{
					m_ScriptMap.insert(std::make_pair(name, script));
				}

			}
			break;
		}

	}
	delete xml;

	return true;
}

ScriptManager::ScriptManager(){

}

ScriptManager::~ScriptManager(){
	this->shutdown();
}

bool ScriptManager::addNewDefinitions(const std::string& XMLScriptDefinition){
	return readInXML(XMLScriptDefinition);
}

void ScriptManager::shutdown(){


	Scripting::StrScriptMap::iterator scriptItrEnd = m_ScriptMap.end();

	for (m_ScriptItr = m_ScriptMap.begin(); m_ScriptItr != scriptItrEnd; ++m_ScriptItr){
		(*m_ScriptItr).second->unload();
		delete (*m_ScriptItr).second;
	}

	m_ScriptMap.clear();

	this->removeAllScriptFunctions();

	if (s_Engine){
		s_Engine->Release();
		s_Engine = NULL;
	}
}

Scripting::Script& ScriptManager::loadScript(const std::string& name){
	Scripting::Script& script = getScript(name);

	script.load();

	return script;

}

Scripting::Script& ScriptManager::getScript(const std::string& name){
	m_ScriptItr = m_ScriptMap.find(name);

	//If the script wasn't found, throw exception
	if (m_ScriptItr == m_ScriptMap.end()){
		throw Scripting::ScriptDoesntExist();
	}

	return *(m_ScriptItr->second);
}

void ScriptManager::unloadScript(const std::string& name){

	Scripting::Script& script = getScript(name);
	script.unload();
}

void ScriptManager::unloadAll(){
	Scripting::StrScriptMap::iterator scriptItrEnd = m_ScriptMap.end();

	for (m_ScriptItr = m_ScriptMap.begin(); m_ScriptItr != scriptItrEnd; ++m_ScriptItr){
		(*m_ScriptItr).second->unload();
	}
}

void ScriptManager::removeScript(const std::string& name){
	m_ScriptItr = m_ScriptMap.find(name);

	//If the script wasn't found, return
	if (m_ScriptItr == m_ScriptMap.end()){
		return;
	}

	m_ScriptItr->second->unload();

	delete m_ScriptItr->second;

	m_ScriptMap.erase(m_ScriptItr);

}
void ScriptManager::reset(){
	this->shutdown();

	this->init();
}

void ScriptManager::registerScriptFunction(const std::string& name, Scripting::ScriptFunction* function){
	m_FunctionItr = m_FunctionMap.find(name);

	//If the function is already registered, print and return;
	if (m_FunctionItr != m_FunctionMap.end()){
		std::cout << "Function: " << name << " is already registered. continuing... \n";
		return;
	}
	
	m_FunctionMap.insert(std::make_pair(name, function));

}

void ScriptManager::removeAllScriptFunctions(){
	Scripting::StrFunctionMap::iterator functionItrEnd = m_FunctionMap.end();

	for (m_FunctionItr = m_FunctionMap.begin(); m_FunctionItr != functionItrEnd; ++m_FunctionItr){
		delete m_FunctionItr->second;
	}

	m_FunctionMap.clear();
}

void ScriptManager::removeScriptFunction(const std::string& name){
	m_FunctionItr = m_FunctionMap.find(name);

	//If the function wasn't found, return
	if (m_FunctionItr == m_FunctionMap.end()){
		return;
	}

	delete m_FunctionItr->second;

	m_FunctionMap.erase(m_FunctionItr);


}

Scripting::ScriptFunction* ScriptManager::createScriptFunction(const std::string& name){
	m_FunctionItr = m_FunctionMap.find(name);

	//If the function wasn't found, throw exception
	if (m_FunctionItr == m_FunctionMap.end()){
		throw Scripting::FunctionDoesntExist();
	}
	Scripting::ScriptFunction* function = new Scripting::ScriptFunction(*(m_FunctionItr->second));

	return function;
}


void ScriptManager::update(){
	//Doesn't do anything right now.  In future it may handle suspending scripts and queueing them based on priority etc.
}

void ScriptManager::registerObject(const std::string& declaration, int size /* = 0 */, ::asDWORD flags /* = 0 */){
	int r = s_Engine->RegisterObjectType(declaration.c_str(),size,flags);

	r < 0 ? throw Scripting::ScriptRegistrationException() : ++r;

}

void ScriptManager::registerObjectMethod(const std::string obj, const std::string declaration, ::asUPtr fnPtr){
	int r = s_Engine->RegisterObjectMethod(obj.c_str(),declaration.c_str(),fnPtr,::asCALL_THISCALL);
	
	r < 0 ? throw Scripting::ScriptRegistrationException() : ++r;

}

void ScriptManager::registerAsGlobal( const std::string declaration, void* ptr )
{
	if(ptr)
	{
		int r = s_Engine->RegisterGlobalProperty(declaration.c_str(),ptr);
		r < 0 ? throw Scripting::ScriptRegistrationException() : ++r;
	}
	else
	{
		throw Scripting::ScriptRegistrationException();
	}
}
void ScriptManager::registerAsGlobal(const std::string declaration, ::asUPtr fnPtr)
{
	int r = s_Engine->RegisterGlobalFunction(declaration.c_str(), fnPtr ,::asCALL_CDECL);

	r < 0 ? throw Scripting::ScriptRegistrationException() : ++r;

}

void PrintString(std::string &str)
{
	std::cout << str;
}

