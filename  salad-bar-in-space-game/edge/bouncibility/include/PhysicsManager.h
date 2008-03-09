#pragma once

#include "Common.h"


namespace irr{
	namespace newton{
		class IWorld;
		class IMaterial;
		class IMaterialCollisionCallback;
		class IMaterialPair;
	}
}
class SomethingForScripts;;

//! namepsace containing methods to help with implementation of PhysicsManager
namespace Physics{

	typedef std::map<std::string, irr::newton::IMaterial*> StrMaterialMap;

	typedef std::map<std::string, irr::newton::IMaterialCollisionCallback*> StrCollisionCallbackMap;

	typedef std::vector<SomethingForScripts*> ScriptVector;

	//! Class containing the callback method for the Physics system.  This derived class maintains the two material names and the list of observing entity scripts.
	class WorldEntityCollisionCallback : IMaterialCollisionCallback{
		
		friend class ::PhysicsManager;

	private:
		const std::string& m_Material1;
		const std::string& m_Material2;

		WorldEntity* entity1;
		WorldEntity* entity2;

		ScriptVector m_CollisionHandlerScripts;

	public:
		//! Should call the correct script for the registered observer. Will pass pointers to the two WorldEntities that collided.
		int ContactEnd(irr::newton::IMaterialPair* material_pair);
		
		//! Retreives the WorldEntities pointers saved in the user data field.  These will be sent to the correct script when ContactEnd is called.
		int  ContactBegin (irr::newton::IMaterialPair *material_pair, irr::newton::IBody *body0, irr::newton::IBody *body1);

		void addHandler();
	}
}

//! LevelManager organizes the transition of levels.  It The current level should be retrieved from LevelManager. It loads in the given XML file for the global defintion of levels.  It tells the entitymanager how to initialize.
class PhysicsManager :
	public CSingleton<PhysicsManager>{
private:

	irr::newton::IWorld* m_World;

	float m_Gravity;

	std::string m_MaterialDefinition;

	StrMaterialMap m_MaterialMap; //!< The list of levels in order for this scenario, XML files to each different level definition.

	StrMaterialMap::iterator m_MaterialItr; //!< An iterator for the Level list.   Pointing to the current level during game execution.

	PhysicsManager();
	~PhysicsManager();
	PhysicsManager(const PhysicsManager& rhs);
	PhysicsManager& operator=(const PhysicsManager& rhs);
public:
	friend CSingleton<PhysicsManager>;

	//! Initialize the LevelManager system. Provide the filename of the XML file that has the definition of the scenario to play.  Returns false if failed.
	bool init(irr::IrrlichtDevice* device, const std::string& XMLMaterialDefinition); 

	//! Add new entity definitions.  This function is the same as init in that it loads in the material definitions.  This allows a user of the class to keep different sets of materials in different definitions and only load in parts of them.
	bool addNewDefinitions(const std::string& XMLMaterialDefinition);

	//! The given script will be called whenever a collision occurs between the two materials.  The colliding WorldEntities will be sent to the script as arguments.
	bool addObserver(SomethingForScripts script, std::string& material1, std::string& material2)

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
	
	void clear(); //!< Clears entity callbacks that have been registered, also unloads all materials except global.  Should be called between levels.

	irr::newton::IWorld* getPhysicsWorld(){
		return m_World;
	}
	
};


load in physics materials, give each type of material a name.  Each material can define any of the types of physical properties and a float value for each one.
Each entity definition refers to its material name by string name if it has a material.  This is associated with it when it is loaded in. Each entity definition also says whether it should have physics applied to it.


PhysicsManager contains a map.  When a material is used by name to attach to another material for collision.  It is checked by name in the collisionMap, in both permutations.  If one exists, the pointer has another worldEntity added to its list.  If it doesn't exist it is added both ways.

need to remember to save the WorldEntity pointers inside the IBody userdata field.