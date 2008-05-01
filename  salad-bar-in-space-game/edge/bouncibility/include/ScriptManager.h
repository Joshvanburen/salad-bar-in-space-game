
#pragma once
#include "Common.h"
#include "singleton.h"
#include "angelscript.h"
#include "Enemy.h"


class ScriptManager;
class WorldEntity;
class Enemy;
//!namespace containing all Script related structures used by the ScriptManager.
namespace Scripting{

	class ScriptRegistrationException{};
	class ScriptLoadingFailure{};
	class ScriptDoesntExist{};
	class FunctionDoesntExist{};
	class FunctionAlreadyRegistered{};

	//! Encapsulates a function that can be called inside an AngelScript script.  
	class ScriptFunction{
		friend class Script;
		friend class ::ScriptManager;
	protected:

		std::string m_Signature; //!< Signature of the function inside the script. Assumed to be initialized by a child constructor.
		int m_ID; //!< ID assigned to this function by AngelScript.
		asIScriptContext* m_pContext; //!< Context to call this script in.  Used to load variables on the stack and suspend or abort scripts.
		
		ScriptFunction(const ScriptFunction& rhs); //!< ScriptManager instantiates a copy of a ScriptFunction every time create is called.  These must be implemented by any subclass that requires data to be copied.
		ScriptFunction& operator=(const ScriptFunction& rhs); //!< ScriptManager instantiates a copy of a ScriptFunction every time create is called.  These must be implemented by any subclass that requires data to be copied.
		
		virtual void execute(); //!< Executes this function.  Prepares the script to call this function if it hasn't already.
	
	public:
		virtual void callFunction();  //!< Should prepare arguments, etc.  Then call execute.
		

		ScriptFunction();
		virtual ~ScriptFunction();

	};

	class MaterialCollisionFunction : public ScriptFunction {
		
	public:

		void callFunction(WorldEntity* entity1, WorldEntity* entity2);

		MaterialCollisionFunction();
	};



	class WorldEntityAIFunction : public ScriptFunction {

	public:

		void callFunction(Enemy* enemy);

		WorldEntityAIFunction();
	};

	class MainFunction : public ScriptFunction {
	public:
		virtual void callFunction();

		MainFunction();
	};

	typedef std::map<std::string, ScriptFunction*> StrFunctionMap;

	//! Class encapsulating a script.  A Script can have any number of functions.  A Script object must be loaded before it can be used.
	class Script{
		friend class ::ScriptManager;
	private:
		bool loaded;
		std::string m_Name;	//!< The name of this Script object.  Used when building contexts.  Should be unique to this script file.
		std::string m_Filename; //!< The filename of this script.

		Scripting::StrFunctionMap m_FunctionMap;

		Scripting::StrFunctionMap::iterator m_FunctionItr;

		asIScriptContext* m_pContext; //!< The context for this Script.  Everything relating to this script is loaded in this context object.
		
		Script(const std::string& sName, const std::string& sFilename);  
		~Script();
		Script(const Script& rhs);
		Script& operator=(const Script& rhs);


		void unload(); //!< Unloads the stored script from memory.

	public:
		
		//! Tells the caller whether this script has been loaded yet or not.
		bool isLoaded(){
			return loaded;
		}
		int load(); //!< Loads the script from memory, compiles and builds it.  Stores it until told to unload it.

		ScriptFunction* addFunction(const std::string& name); //!< Add a function that is capable of being called for this Script.
		void removeFunction(const std::string& name); //!< Remove a function that can be called for this Script.
		ScriptFunction* getFunction(const std::string& name); //!< Retrieves a handle to a function that can be called for this Script given a string name.
	};

	typedef std::map<std::string, Script*> StrScriptMap; 
	typedef std::list<Script*> ScriptList; 
};
//! ScriptManager encapsulates all workings of the Script system.
/*! ScriptManager holds a mapping between string script names and filenames.  When it is told to load a certain script, it loads it into memory.  
* Users can then access the script by name.  Assuming it has been loaded, users can add ScriptFunction classes to the Script.
* These tell the Script object which functions it can call in the loaded script.  A user uses the ScriptFunction object to call the script function.
* During level loading, the script manager is told which scripts will be used.  These are loaded in.  
*/
class ScriptManager :
	public CSingleton<ScriptManager>{

	friend class ::asIScriptEngine;
	friend class Scripting::Script;
private:

	std::string m_ScriptDefinition;
	
	static asIScriptEngine *s_Engine; //!< A pointer to the scripting engine.

	Scripting::StrScriptMap m_ScriptMap; //!< Maps Strings to Scripts.

	Scripting::StrFunctionMap m_FunctionMap;

	Scripting::StrFunctionMap::iterator m_FunctionItr;

	//Script::ScriptList m_ExecutionQueue; //!< List of scripts to be executed.

	//Script::ScriptList::iterator m_ExecutionQueueItr; //!< Iterator for the execution queue.

	Scripting::StrScriptMap::iterator m_ScriptItr; //!< An iterator for the String Script map

	bool readInXML(const std::string& XMLScriptDefinition); //!< Helper method for reading in Script definitions.

	Scripting::ScriptFunction* createScriptFunction(const std::string& name); //!< Retreives a new copy of the script type specified by the string name

	ScriptManager();
	~ScriptManager();
	ScriptManager(const ScriptManager& rhs);
	ScriptManager& operator=(const ScriptManager& rhs);

public:
	friend CSingleton<ScriptManager>;

	bool init(); //!< Initialize the scripting system. Maps script names to filenames.

	bool addNewDefinitions(const std::string& XMLScriptDefinition);  //Adds additional script names to filenames.

	void shutdown();  //!< shutdown any resources used by the ScriptManager. Unloads all loaded scripts. Clears string to script filename map. 

	Scripting::Script& loadScript(const std::string& name); //!< Tells the script manager that the script with the given name should be loaded in.

	Scripting::Script& getScript(const std::string& name); //!< Retrieve a handle to the script with the given name
	void unloadScript(const std::string& name); //!< Tells the ScriptManager that this Script is no longer needed, so it can be unloaded.

	void addScript(const std::string& name, const std::string& filename);

	void unloadAll(); //!< Unloads all scripts in the system.  Useful between levels.

	void removeScript(const std::string& name);

	void reset(); //!< Unloads all script definitions except global, as if init had just been called.

	void update(); //!< Not currently used. Executes all scripts in the execution queue.  Gives scripts in the front of the list more execution time.  Suspends scripts when allotted time expires.
	
	void registerScriptFunction(const std::string& name, Scripting::ScriptFunction* function); //!< Registers a function with the ScriptManager.  It will manage the deletion of the ScriptFunction when it is done.

	void removeScriptFunction(const std::string& name); //!< Removes a registered script function.

	void removeAllScriptFunctions(); //!< Remomve all script functions.

	void registerObject(const std::string& declaration, int size = 0, asDWORD flags = 0);
	void registerReferenceObject(const std::string&); //!< Registers a class name for the scripting system to access.  

	void registerObjectMethod(std::string obj, std::string declaration, ::asUPtr fnPtr);  //!< Registers a method with an already registered class name.

	void registerAsGlobal( const std::string declaration, void* ptr );

	void registerAsGlobal(const std::string declaration, ::asUPtr fnPtr);

	static ::asIScriptEngine* getEngine(){
		return s_Engine;
	}
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