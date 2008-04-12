#pragma once

#include "Common.h"


namespace irr{
	namespace newton{
		class IWorld;
		class IMaterial;
		class IMaterialCollisionCallback;
		class IBody;
		class IMaterialPair;
		class IMaterialPairAndContact;
	};
};
namespace Scripting{
	class ScriptFunction;
};
namespace Sound{
	class Audio;
};


class WorldEntity;

class WorldEntityAIManager;

//! namepsace containing methods to help with implementation of WorldEntityAIManager
namespace WorldEntityAI{

	typedef std::map<std::string, WorldEntityAIFunction*> StrAIFunctionMap;

	typedef std::set<Scripting::ScriptFunction*> ScriptList;

	
};

//! WorldEntityAIManager loads in the given XML file for the defines of AIs.  It tells the WorldEntity update() how to move or shoot.
class WorldEntityAIManager :
	public CSingleton<WorldEntityAIManager>{
private:

	WorldEntityAI::StrAIFunctionMap m_AIFunctionMap; //!< The map of AI types to AI script functions.

	WorldEntityAI::StrAIFunctionMap::iterator m_AIFunctionItr; //!< An iterator for the AI type to script map.

	std::string m_AIDefinition;

	bool readInXML(const std::string& XMLMaterialDefinition);

	WorldEntityAIManager();
	~WorldEntityAIManager();
	//PhysicsManager(const PhysicsManager& rhs);
	//PhysicsManager& operator=(const PhysicsManager& rhs);

public:
	friend CSingleton<WorldEntityAIManager>;

	//! Initialize the WorldEntityAIManager system. Provide the filename of the XML file that has the definition of the scenario to play.  Returns false if failed.
	bool init(irr::IrrlichtDevice* device); 

	////! Add new entity definitions.  This function is the same as init in that it loads in the material definitions.  This allows a user of the class to keep different sets of materials in different definitions and only load in parts of them.
	//bool addNewDefinitions(const std::string& XMLMaterialDefinition);

	////! The given script will be called whenever a collision occurs between the two materials.  The colliding WorldEntities will be sent to the script as arguments.
	//bool addObserver(Scripting::ScriptFunction* scriptFunction, const std::string& material1, const std::string& material2);

	////bool addCollisionSound(Sound::Audio* sound, const std::string& material1, const std::string& material2);

	////! Get to the current value for gravity
	//float getGravity() const{
	//	return m_Gravity;
	//}

	////! Set a new value for gravity
	//void setGravity(float newGravity){
	//	m_Gravity = newGravity;
	//}

	////! Return a pointer to the material referred to by the given name.
	//irr::newton::IMaterial* getMaterial(const std::string& name);

	void update(WorldEntity* entity); //!< Updates the physics world and handles anything else that needs to happen each iteration of the main loop.

	void shutdown();  //!< shutdown any resources used by the LevelManager. 
	
	//Don't forget to call this sometime later.
	void clear(); //!< Clears entity callbacks that have been registered, also unloads all materials except global.  Should be called between levels.  WARNING! All material handles become stale after this call.

	//irr::newton::IWorld* getPhysicsWorld(){
	//	return m_World;
	//}
	
};

