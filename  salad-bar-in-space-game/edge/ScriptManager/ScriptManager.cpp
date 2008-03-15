#include "angelscript.h"
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


bool ScriptManager::init(const std::string& XMLScriptDefinition){
	engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	if( engine == 0 )
	{
		std::cout << "Failed to create script engine." << std::endl;
		return false;
	}

	// The script compiler will write any compiler messages to the callback.
	engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, ::asCALL_CDECL);

	return true;

}