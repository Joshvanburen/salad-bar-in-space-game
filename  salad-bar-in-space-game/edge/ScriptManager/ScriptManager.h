
#pragma once
#include "Common.h"
#include "singleton.h"
#include "angelscript.h"

class ScriptManager;
//!namespace containing all Script related structures used by the ScriptManager.
namespace Script{

	class Script;

	typedef std::map<int, void*> IdScriptMap; 
	typedef std::list<void*> ScriptList; 
};
//! InputManager encapsulates all workings of the InputSystem.
/*! Currently the InputManager only uses DirectInput.  In the future the Input API used might be decoupled from the InputManager.  But at the present time,
* there are very few other options besides DirectInput.
*/
class ScriptManager :
	public CSingleton<ScriptManager>{

	friend class ::asIScriptEngine;
private:
	
	asIScriptEngine *engine;

	Script::IdScriptMap m_ScriptMap; //!< Maps Ids to Scripts.

	Script::ScriptList m_ExecutionQueue; //!< List of scripts to be executed.

	Script::ScriptList::iterator m_ExecutionQueueItr; //!< Iterator for the execution queue.

	Script::IdScriptMap::iterator m_ScriptItr; //!< An iterator for the Id Script map

	ScriptManager();
	~ScriptManager();
	ScriptManager(const ScriptManager& rhs);
	ScriptManager& operator=(const ScriptManager& rhs);

public:
	friend CSingleton<ScriptManager>;

	bool init(); //!< Initialize the scripting system.

	void shutdown();  //!< shutdown any resources used by the ScriptManager.

	Script::Script& loadScript(); //!< Tells the ScriptManager that this script will be needed soon. Returns a handle to the encompassing Script object.  Should check if this script is loaded already, if it is, send handle back, increasing reference count?. 

	void unloadScript(); //!< Tells the ScriptManager that this Script is no longer needed, so it can be unloaded.

	void unloadAll(); //!< Unloads all scripts in the system.

	void update(); //!< Executes all scripts in the execution queue.  Gives scripts in the front of the list more execution time.  Suspends scripts when allotted time expires.
	
	//Need some way to register class information cleanly
};



/*What should ScriptManager be able to do?

Load in scripts in an efficient way.  This may be all needed scripts at the beginning of a level or it may be only when needed, etc.
Make sure that scripts do not take up a large portion of execution time.  They should be suspended if they take up too much time.  
Scripts in the queue should get an equal amount of time to finish.  After each time limit for each time update is called, all scripts are suspended.  
Execution of the scripts resumes when upate is called next.
The script manager should be told how much time it has available to run the scripts, this should be deteremined by how much time is left to run the current frame.
All of the classes in the game should be accessible to the script.  Possibly some of irrlicht's classes should also be available.  It may be better to only offer 
our own classes and call irrlicht's classes from our methods.  
Right now the StateManager and the PhysicsManager need to call the scripts.  Are there any other times when a script needs to be called?  Triggers will need to call scripts when they are made.  In a better design, initialization of entities would call some sort of init function in a script as well.
Each of these things that need scripts to be called should tell the script manager what function signature they are calliing in the script.  
Each Entity declaration can have its own script.  So two enemies of the same type can have a single script.  This means 20 enemies instantiated from the same declaration would have the same script called for thei state changes, initialization, etc.
*/