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

class PhysicsManager;

//! namepsace containing methods to help with implementation of PhysicsManager
namespace Physics{
	class WorldEntityCollisionCallback;
	typedef std::map<std::string, irr::newton::IMaterial*> StrMaterialMap;

	typedef std::map<std::string, WorldEntityCollisionCallback*> StrCollisionCallbackMap;

	typedef std::set<Scripting::ScriptFunction*> ScriptList;

	typedef std::set<Sound::Audio*> SoundList;
	
	//! Thrown if there is an attempt to add a script handler twice.
	class DuplicateScript { };
	//! Class containing the callback method for the Physics system.  This derived class maintains the two material names and the list of observing entity scripts.  Only for WorldEntity collisions, not entity and levels
	class WorldEntityCollisionCallback : irr::newton::IMaterialCollisionCallback{
		
		friend class ::PhysicsManager;

	private:
		Physics::ScriptList::iterator m_ScriptItrEnd;
		Physics::ScriptList::iterator m_ScriptItr;
		const std::string& m_Material1;
		const std::string& m_Material2;

		WorldEntity* entity1;
		WorldEntity* entity2;

		Physics::ScriptList m_CollisionHandlerScripts;

		Physics::SoundList::iterator m_SoundsItr;
		Physics::SoundList::iterator m_SoundsItrEnd;

		Physics::SoundList m_Sounds;

		WorldEntityCollisionCallback(const std::string& m_Material1, const std::string& material2);
		WorldEntityCollisionCallback(const std::string& m_Material1, const std::string& material2, Scripting::ScriptFunction* scriptFunction);
		~WorldEntityCollisionCallback();
		WorldEntityCollisionCallback(const WorldEntityCollisionCallback& rhs);
		WorldEntityCollisionCallback& operator=(const WorldEntityCollisionCallback& rhs);			
	public:
		//! Should call the correct script for the registered observer. Will pass pointers to the two WorldEntities that collided.
		void ContactEnd(irr::newton::IMaterialPair* material_pair);
		
		//! Retreives the WorldEntities pointers saved in the user data field.  These will be sent to the correct script when ContactEnd is called.
		int  ContactBegin (irr::newton::IMaterialPair *material_pair, irr::newton::IBody *body0, irr::newton::IBody *body1);

		//! This is where any information about which points contacted needs to be saved.
		int  ContactProcess (irr::newton::IMaterialPairAndContact *material_pair_and_contact);

		void addHandler(Scripting::ScriptFunction* scriptFunction);

		void addSound(Sound::Audio* sound);
	};
	//! Thrown if an material was assumed to have existed but was not found. 
	class MaterialDoesntExist {  };
};

class PhysicsManager :
	public CSingleton<PhysicsManager>{
private:

	irr::IrrlichtDevice* m_Device;

	irr::newton::IWorld* m_World;

	float m_Gravity;

	std::string m_MaterialDefinition;

	Physics::StrMaterialMap m_MaterialMap; //!< The list of materials held by the PhysicsManager, referenced by string name.

	Physics::StrMaterialMap::iterator m_MaterialItr; //!< An iterator for the Level list.   Pointing to the current level during game execution.

	Physics::StrCollisionCallbackMap m_CallbackMap; //!< The map of materialpairs to collision callback objects for those pairs.  Two material strings are concatenated together in alphabetical order as key.

	Physics::StrCollisionCallbackMap::iterator m_CallbackItr; //!< An iterator for the material collision map.

	bool readInXML(const std::string& XMLMaterialDefinition);

	PhysicsManager();
	~PhysicsManager();
	PhysicsManager(const PhysicsManager& rhs);
	PhysicsManager& operator=(const PhysicsManager& rhs);

public:
	friend CSingleton<PhysicsManager>;

	//! Initialize the LevelManager system. Provide the filename of the XML file that has the definition of the scenario to play.  Returns false if failed.
	bool init(irr::IrrlichtDevice* device); 

	//! Add new entity definitions.  This function is the same as init in that it loads in the material definitions.  This allows a user of the class to keep different sets of materials in different definitions and only load in parts of them.
	bool addNewDefinitions(const std::string& XMLMaterialDefinition);

	//! The given script will be called whenever a collision occurs between the two materials.  The colliding WorldEntities will be sent to the script as arguments.
	bool addObserver(Scripting::ScriptFunction* scriptFunction, const std::string& material1, const std::string& material2);

	bool addCollisionSound(Sound::Audio* sound, const std::string& material1, const std::string& material2);

	//! Get to the current value for gravity
	float getGravity() const{
		return m_Gravity;
	}

	//! Set a new value for gravity
	void setGravity(float newGravity){
		m_Gravity = newGravity;
	}

	//! Return a pointer to the material referred to by the given name.
	irr::newton::IMaterial* getMaterial(const std::string& name);

	void update(); //!< Updates the physics world and handles anything else that needs to happen each iteration of the main loop.

	void shutdown();  //!< shutdown any resources used by the LevelManager. 
	
	//Don't forget to call this sometime later.
	void clear(); //!< Clears entity callbacks that have been registered, also unloads all materials except global.  Should be called between levels.  WARNING! All material handles become stale after this call.

	irr::newton::IWorld* getPhysicsWorld(){
		return m_World;
	}
	
};

