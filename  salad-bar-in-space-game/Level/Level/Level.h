#pragma once

#include "Common.h"

namespace scene{
class ISceneNode;
class ISceneManager;
}

typedef std::vector<WorldEntity&> EntityVector;
//!The Level class holds everything within a specific level.  It loads in all the world entities and updates them.  Each level has its own xml file. 
class Level
{
	//Private methods and variables
	private:
			//!Vector to hold all world entities
			EntityVector m_WorldEntities;
			//!Iterator for the world entities
			EntityVector::iterator m_WorldEntityItr;
			//!Level name
			std::string m_LevelName;
			//!Filename of xml definition for this level.
			std::string m_XmlFile;
			//!Level file name
			std::string m_LevelFile;
			//!Filename of music file
			std::string m_MusicFile;
			//!Level timer;
			int m_Time;
			//!Current status of level
			int m_Status;

			friend std::ostream& operator << (std::ostream& os, const Level& level);
		
	//Public methods and variables
	public:
			static const int WAITING_REPEAT;
			static const int RUNNING;
			static const int WAITING_START;
			static const int FINISHED;
			static const int STOPPED:

			explicit Level();
			~Level();
			//! Load the level from the given xml definition.  Loads in all entities from the EntityManager and stores them in the entityvector.
			/*! Level xml definition format
			<Level attributes, name, music, time, startingx, starting y, mapfilename>
				<WorldEntities>
					<Entity name, xlocation, ylocation, startState/>
					...
				</WorldEntities>
			</Level>
			*/
			bool load(const std::string& LevelDefinition);
			//!Releases all resources used.  Does not need to delete the entities it has, EntityManager does this.
			void shutdown();
			//!Updates the level and all children
			void update();
			//!Gets the level name
			const std::string& getName();
			//!Gets the music file name
			const std::string& getMusicFileName();
			//!Gets the current level time
			int getCurrentTime();
			//!Sets the current level time
			void setCurrentTime(int ctime);
			//!return the status of the current level.  Used to determine whether to go to now or display restart menu, etc.
			int status();
			//!Allows someone else to set the status of this level to FINISHED, WAITING_REPEAT, or any other possible statuses.  This would be accessed by the UserInterface or possibly a finished marker worldentity.
			void setStatus(int newStatus);
};
