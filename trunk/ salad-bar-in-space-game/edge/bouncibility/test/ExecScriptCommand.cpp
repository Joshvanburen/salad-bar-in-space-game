
#include "GameIncludes.h"
#include "ExecScriptCommand.h"
#include "LoadScriptCommand.h"



LOADSCRIPT::LOADSCRIPT() : IC_Command(L"load"){

	setUsage(L"load [AngelCodeFile]");
	addDescLine(L"This command loads an angelscript script file and executes it's main method.");

}

bool LOADSCRIPT::invoke(const array<WideString>& args, IC_Dispatcher *pDispatcher, IC_MessageSink* pOutput){
	if(args.size() == 0)
	{
		//this is one way of handling error
		//we print the usage to the console
		pOutput->logError_ANSI("A filename must be provided");
		printUsage(pOutput);
		return false;
	}
	else
	{
		std::string filename(IC_StrConv::toString(args[0]).c_str());
	
		//execute code
		ScriptManager::getSingleton().addScript(filename, filename);

		Scripting::Script& script = ScriptManager::getSingleton().loadScript(filename);
		Scripting::ScriptFunction* main_function = script.addFunction("main");
		main_function->callFunction();

		ScriptManager::getSingleton().removeScript(filename);
		
		return true;
	}
}

LOADSCRIPT::~LOADSCRIPT(){

}
EXECUTESCRIPT::EXECUTESCRIPT() : IC_Command(L"exec"){

	setUsage(L"exec [AngelScriptCode]");
	addDescLine(L"this command executes arbitrary angel script code.");
	addDescLine(L"Can call any exported C++ function.");

}
bool EXECUTESCRIPT::invoke(const array<WideString>& args, IC_Dispatcher *pDispatcher, IC_MessageSink* pOutput){
	if(args.size() == 0)
	{
		//this is one way of handling error
		//we print the usage to the console
		pOutput->logError_ANSI("no angel script code provided");
		printUsage(pOutput);
		return false;
	}
	else
	{
		String script = "";
		String code =  IC_StrConv::toString(args[0]);
		script.append(code);
		script.append("");
		//execute code
		int r = ScriptManager::getEngine()->ExecuteString(0, script.c_str());
		
		if( r < 0 )
			cout << "Invalid script statement. " << endl;
		else if( r == asEXECUTION_EXCEPTION ){
			cout << "A script exception was raised." << endl;
			if (r < 0)
			{
				//this is another way of error intimation
				//we throw an exception and the console takes
				//care of showing the message
				std::cout << "Code in console was invalid.\n";
			}
			else if (r 	== asEXECUTION_EXCEPTION){
				std::cout << "A script exception was raised.\n";
			}
		}
		return true;
	}
}

EXECUTESCRIPT::~EXECUTESCRIPT(){

}