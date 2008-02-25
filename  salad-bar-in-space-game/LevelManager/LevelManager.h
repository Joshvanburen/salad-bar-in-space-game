#pragma once
#include <list>
#include "singleton.h"



class Level;


typedef std::list<std::string> LevelList;
//! EntityManager provides an easy and organized way of retrieving entities that were defined via an XML file.  It loads in the given XML file for the global defintion of entities.  Given an entity string name, it will then provide an instantiation of an WorldEntity and keep track of global IDs and will keep track of the deletion of the WorldEntity.
class LevelManager :
	public CSingleton<LevelManager>{
private:
	LevelList m_Levels; //!< The list of levels in order for this scenario, XML files to each different level definition.
	
	Level& m_CurrentLevel; //!< A handle to the currently instantiated level.

	LevelList::iterator m_LevelItr; //!< An iterator for the Level list.   Pointing to the current level during game execution.

	LevelManager();
	~LevelManager();
	LevelManager(const LevelManager& rhs);
	LevelManager& operator=(const LevelManager& rhs);
public:
	friend CSingleton<LevelManager>;

	//! Initialize the LevelManager system. Provide the filename of the XML file that has the definition of the scenario to play.  Returns false if failed.
	bool init(const std::string& XMLScenarioDefinition); 

	void shutdown();  //!< shutdown any resources used by the LevelManager. 


	void reset(); //!< Reset the system, as if init had just been called.
	

	void reset(const std::string& XMLScenarioDefinition); //!< Reset the system, initializing with this provided new scenario


	//!Returns a reference to the current level object. 
	Level& getCurrentLevel() const;

	//! Go to the next level.  Handles the updating of UserInterface, etc.  Returns false if there is no next.
	bool goToNext();

	//! Repeat the current level, re-initializing all the variables to what they were at start.
	bool repeat();

	//! Update the level.  Calls update on the current level.  It also checks if the level has been beaten and the level needs to be changed.  Or if the characater has died and there needs to be a repeat of the current level.
	void update();

	//! Draws the current level.  Basically calls draw on the current level object as of now.
	void draw();
};



