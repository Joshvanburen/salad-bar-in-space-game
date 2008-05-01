#pragma once

#include "Common.h"
#include "irrlicht.h"
namespace irr{
	namespace newton{
		class IMaterial;
		class IBody;
	}
}
namespace Sound{
	class Audio;
}

typedef std::list<WorldEntity*> EntityVector;
//!The Level class holds everything within a specific level.  It loads in all the world entities and updates them.  Each level has its own xml file. 
class Level
{
	//Private methods and variables
	private:
			//!Vector to hold all world entities
			EntityVector m_WorldEntities;
			EntityVector m_EntitiesToAdd;
			EntityVector m_EntitiesToRemove;
			//!Iterator for the world entities
			EntityVector::iterator m_WorldEntityItr;
			//!Level name
			std::string m_LevelName;
			//!Filename of xml definition for this level.
			std::string m_XmlFile;
			//!Level file name
			std::string m_LevelFile;
			//!name of music file
			std::string m_MusicName;
			int m_MusicIndex;
			//!Handle to the sound object for music.
			std::vector<Sound::Audio*> m_MusicList;
			Sound::Audio* m_Music;
			//!Level timer;
			int m_Time;
			//!Current status of level
			int m_Status;

			irr::scene::ICameraSceneNode* m_Camera;

			irr::core::dimension2di m_LevelDimensions;

			int m_MinX, m_MinY, m_MaxX, m_MaxY;

			int m_StartingX;

			int m_StartingY;

			irr::scene::IMeshSceneNode* m_SceneNode;

			irr::scene::IMesh* m_Mesh;

			irr::newton::IBody* m_Physics_Body;

			irr::newton::IMaterial* m_Material;

			friend std::ostream& operator << (std::ostream& os, const Level& level);
		
	//Public methods and variables
	public:
			static const int WAITING_REPEAT;
			static const int RUNNING;
			static const int WAITING_START;
			static const int FINISHED;
			static const int STOPPED;

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
			irr::scene::ICameraSceneNode* getCamera(){
				return m_Camera;
			}
			void removeEntity(WorldEntity* entity);

			void addEntity(WorldEntity* entity);
			//!Releases all resources used.  Does not need to delete the entities it has, EntityManager does this.
			void shutdown();
			//!Updates the level and all children
			void update();
			//!Gets the level name
			const std::string& getName();
			//!Gets the music file name
			const std::string& getMusicName();
			//!Gets the current level time
			int getCurrentTime();
			//!Sets the current level time
			void setCurrentTime(int ctime);
			//!return the status of the current level.  Used to determine whether to go to now or display restart menu, etc.
			int status();
			//!Allows someone else to set the status of this level to FINISHED, WAITING_REPEAT, or any other possible statuses.  This would be accessed by the UserInterface or possibly a finished marker worldentity.
			void setStatus(int newStatus);
			//!Returns the dimensions specified in the XML as the bounds of the level.  Might be used by camera and input system.
			irr::core::dimension2di getDimensions(){
				return m_LevelDimensions;
			}

			float getMinX(){
				return (float)m_MinX;
			}
			float getMinY(){
				return (float)m_MinY;
			}
			float getMaxX(){
				return (float)m_MaxX;
			}
			float getMaxY(){
				return (float)m_MaxY;
			}
};
