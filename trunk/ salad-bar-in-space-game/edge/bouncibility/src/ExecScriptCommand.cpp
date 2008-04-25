#include "GameIncludes.h"
#include "ExecScriptCommand.h"
void grab(int v)
{
	cout << v << endl;
}

void grab(asUINT v)
{
	cout << v << endl;
}

void grab(bool v)
{
	cout << std::boolalpha << v << endl;
}

void grab(float v)
{
	cout << v << endl;
}

void grab(double v)
{
	cout << v << endl;
}

void grab(const std::string &v)
{
	cout << v << endl;
}

void grab()
{
	// There is no value
}

EXECUTESCRIPT::EXECUTESCRIPT() : IC_Command(L"exec"){

	setUsage(L"exec [AngelScriptCode]");
	addDescLine(L"this command executes arbitrary angel script code.");
	addDescLine(L"Can call any exported C++ function.");
	// Register special function with overloads to catch any type.
	// This is used by the exec command to output the resulting value from the statement.
	ScriptManager::getSingleton().registerAsGlobal("void _grab(bool)", asFUNCTIONPR(grab, (bool), void));
	ScriptManager::getSingleton().registerAsGlobal("void _grab(int)", asFUNCTIONPR(grab, (int), void));
	ScriptManager::getSingleton().registerAsGlobal("void _grab(uint)", asFUNCTIONPR(grab, (asUINT), void));
	ScriptManager::getSingleton().registerAsGlobal("void _grab(float)", asFUNCTIONPR(grab, (float), void));
	ScriptManager::getSingleton().registerAsGlobal("void _grab(double)", asFUNCTIONPR(grab, (double), void));
	ScriptManager::getSingleton().registerAsGlobal("void _grab()", asFUNCTIONPR(grab, (), void));
	ScriptManager::getSingleton().registerAsGlobal("void _grab(const string& in)", asFUNCTIONPR(grab, (const std::string&), void));


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
		String script = "_grab(";
		String code =  IC_StrConv::toString(args[0]);
		script.append(code);
		script.append(")");
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