

#pragma once
#include "singleton.h"
#include "irrXML.h"

class EntityManager;
//!namespace containing all Entity related structures used by the EntityManager.
namespace Entity{

	typedef std::map<std::string, EntityFactory*> TypeFactoryMap; 

	//! Base class for all factories in the system.  Provides a loadEntity method given an XML filename of the entity.
	class EntityFactory
	{
		friend class ::Loader;
	private:
		//! Each child class of EntityFactory should implement the way it wishes to load its WorldEntity object based on the type it is.
		virtual WorldEntity& loadEntity(const std::string& XMLFilename);
		explicit EntityFactory();
		virtual ~EntityFactory();
	};


	//! Loader is used by EntityManager.  It keeps track of the factories for each type.  Types are currently hard coded.  
	class Loader
	{
		friend class ::EntityManager;
	private:
		TypeFactoryMap mFactoryMap; //!< Maps type strings to the factory pointer to use for that type.
		//! Creates the worldEntity using the provided factory and returns a reference to it.
		WorldEntity& loadEntity(EntityFactory* factory);

		//! Returns the factory pointer given a type name string.  This should be used by EntityManager while loading Entities in.
		EntityFactory* getFactory(const std::string& type);
		
		//! Registers a factory and type into the Loader.  These are hard coded in the EntityManager and might always be.  Factories must be loaded before that type can be created.  There should not be multiple factories for a single type.  The first type given will be the only one registered.  Once a factory is loaded it can be forgotten about and will be deleted by the Loader.
		void registerFactory(const std:string& type, EntityFactory* factory);

		Loader();
		
		~Loader();

		friend std::ostream& operator << (std::ostream& os, const Loader& device);

	};



	//! Entity is a data structure for storing the information about an entity loaded in by EntityManager from an XML file. 
	/*! Entity objects are created internally by the EntityManager, I don't think there is a need to give anyone else access to them.
	* Entity contain the string name to refer to this entity by.  The XML filename this entity's definition is contained in.
	* It also contains a pointer to the factory that can create this type of entity.  This is determined by the type.
	*/
	class Entity{
		friend class EntityManager;
	protected:
		const std::string& m_Name; //!< The name of the Entity.
		const EntityFactory* mp_Factory; //!< The pointer to the factory for this entity, can be determined by type string
		const std:string& m_XMLFile; //!< The xml filename the factory will use to instantiate a worldentity.

		void destroy(); //!< Releases any memory this entity is taking up so it can be dropped from the system.

		friend std::ostream& operator << (std::ostream& os, const Entity& entity);

		explicit Entity(const std::string& sName, const std::string& sType, const std::string& sXMLFile);

		Entity(const Entity& rhs);

		Entity& operator=(const Action& rhs);

		~Entity();
	public:
		
		const std::string& getName() const; //!< Returns the name this entity is referred to by.
	};

	typedef std::map<std::string, Entity*> StrEntityMap; 

	typedef std::map<int, WorldEntity*> IdEntityMap;

	//end namespace Entity
};
//! EntityManager provides an easy and organized way of retrieving entities that were defined via an XML file.  It loads in the given XML file for the global defintion of entities.  Given an entity string name, it will then provide an instantiation of an WorldEntity and keep track of global IDs and will keep track of the deletion of the WorldEntity.
class EntityManager :
	public CSingleton<EntityManager>{
private:

	static int next_ID; //!< Next available ID to assign to the next WorldEntity that is instantiatated.

	Entity::StrEntityMap m_EntityMap; //!< Maps strings to Entity objects.

	Input::IdEntityMap m_IdEntityMap; //!< Maps integers to instantiated world entities.

	Input::StrIdEntityMap::iterator m_EntityItr; //!< An iterator for the id entity map.


	EntityManager();
	~EntityManager();
	EntityManager(const EntityManager& rhs);
	EntityManager& operator=(const EntityManager& rhs);
public:
	friend CSingleton<EntityManager>;

	bool init(); //!< Initialize the Entity system.

	void shutdown();  //!< shutdown any resources used by the EntityManager.

	//!Retrieves the entity information for an entity named by the given string.  Loads this entity with the correct factory and returns a reference to it.
	WorldEntity& createEntity(const std::string name); 

	//! Deletes an entity identified by the given ID number.  This can be called to delete an entity since delete should not be called on WorldEntity.  All Entities will be cleaned up when EntityManager is shutdown.
	void remove(const int entityID);

	//! Deletes all currently instantiated entities.  This would be ideal for moving from level to level.  Even the ball should get deleted because it will also be in the next level's definition.
	void removeAll();

	//!Returns a reference to an already instantiated WorldEntity object given it's ID, NULL if WorldEntity with ID does not exist.
	WorldEntity& getEntity(const int entityID) const;

};