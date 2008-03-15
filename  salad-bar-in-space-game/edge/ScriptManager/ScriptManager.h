
#pragma once
#include "Common.h"
#include "singleton.h"
#include "angelscript.h"


class ScriptManager;
//!namespace containing all Script related structures used by the ScriptManager.
namespace Script{

	class ScriptRegistrationException{};



	class ScriptFunction{
		friend class Script;
	protected:
		std::string m_Signature;
		int m_ID;
		asIScriptContext* m_pContext;
		
		virtual void prepare() = 0;

		ScriptFunction(const ScriptFunction& rhs);
		ScriptFunction& operator=(const ScriptFunction& rhs);
	public:

		virtual void execute();

		ScriptFunction();
		virtual ~ScriptFunction();

	};
	typedef std::map<std::string, ScriptFunction*> StrFunctionMap;

	class Script{
		friend class ::ScriptManager;
	private:
		std::string m_Name;
		std::string m_Filename;

		StrFunctionMap m_FunctionMap;

		StrFunctionMap::iterator m_FunctionMapItr;

		asIScriptContext* m_pContext;
		
		Script(const std::string& sName);
		~Script();
		Script(const Script& rhs);
		Script& operator=(const Script& rhs);

		int load();

		void unload();

	public:
		void addFunction(const std::string& name, ScriptFunction*);
		void removeFunction(const std::string& name);
		ScriptFunction& getFunction(const std::string& name);
	};

	typedef std::map<std::string, Script*> StrScriptMap; 
	typedef std::list<Script*> ScriptList; 
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

	Script::StrScriptMap m_ScriptMap; //!< Maps Strings to Scripts.

	//Script::ScriptList m_ExecutionQueue; //!< List of scripts to be executed.

	//Script::ScriptList::iterator m_ExecutionQueueItr; //!< Iterator for the execution queue.

	Script::StrScriptMap::iterator m_ScriptItr; //!< An iterator for the String Script map

	ScriptManager();
	~ScriptManager();
	ScriptManager(const ScriptManager& rhs);
	ScriptManager& operator=(const ScriptManager& rhs);

public:
	friend CSingleton<ScriptManager>;

	bool init(const std::string& XMLScriptDefinition); //!< Initialize the scripting system. Maps script names to filenames.

	bool addNewDefinitions(const std::string& XMLScriptDefinition);  //Adds additional script names to filenames.

	void shutdown();  //!< shutdown any resources used by the ScriptManager.

	Script::Script& loadScript(); //!< Tells the ScriptManager that this script will be needed soon. Returns a handle to the encompassing Script object.  Should check if this script is loaded already, if it is, send handle back, increasing reference count?. 

	void unloadScript(const std::string& name); //!< Tells the ScriptManager that this Script is no longer needed, so it can be unloaded.

	void unloadAll(); //!< Unloads all scripts in the system.  Useful between levels.

	void reset(); //!< Unloads all script definitions except global, as if init had just been called.

	void update(); //!< Executes all scripts in the execution queue.  Gives scripts in the front of the list more execution time.  Suspends scripts when allotted time expires.
	
	void registerObject(const std::string&, int size, ::asDWORD flags = 0);

	void registerObjectMethod(std::string obj, std::string declaration, ::asUPtr fnPtr);
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


/* During initialization, script string names are mapped to actual script filenames.
During level loading, the script manager is told which scripts will be used.  These are loaded in.  
Each entity declaration can declare a script.  
In the physics callback, the script for an entity is referred to by name.  It is then assumed that that entity has defined a correct collide method.
This collide method is defined in the system as a subclass of ScriptFunction.  A CollideFunction is created during the callback registration and
added to the correct script object for that entity.  The CollideFunction object is saved in the CollisionCallbackObject and would have execute called
on it whenever the collision occurred.  The CollisionFunction should create its own overloaded execute function that calls the super classes execute().
This would accept and push the correct arguments onto the AngelScript stack, the parent function would then call the function and return when it is complete.
When the script is called, blocking occurs to wait for it to finish, this could become a big problem which might need to be fixed somehow (multithreading?).
The overloaded execute function can also return a value to the caller if it wishes to.  It would pop the angelscript return value off the stack and cast and return it.
*/