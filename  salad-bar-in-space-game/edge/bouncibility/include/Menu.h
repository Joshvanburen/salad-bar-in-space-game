/*
Header file for the menu system
*/

#pragma once

#include "Common.h"
#include "singleton.h"
#include "irrlicht.h"

//Access to the irr namespace
namespace irr
{
	//Access to class IrrlichtDevice
	class IrrlichtDevice;

	//Access to the scene namespace
	namespace scene
	{
		//Access to IBillboardSceneNode
		class IBillboardSceneNode;
		//Access to class ISceneManager
		class ISceneManager;
	}

	//Access to video namespace
	namespace video
	{
		//Access to class
		class IVideoDriver;
	}
}

//Defines the menu class
class Menu :
	//Menu is a Singleton object so only one can be created
	public CSingleton <Menu>
	{
		//CSingleton as friend
		friend CSingleton;
		friend class GameSystem;

		//Private functions and members
		private:
			//Initialization
			void init(irr::IrrlichtDevice* device);
			//Quit
			void quit();
			//New game
			void newGame();
			//New game
			irr::scene::IBillboardSceneNode * b_newGame;
			//Quit
			irr::scene::IBillboardSceneNode * b_quit;
			//Video driver
			irr::video::IVideoDriver* s_Driver;
			//Scene manager
			irr::scene::ISceneManager* s_Smgr;
			//Device
			irr::IrrlichtDevice* s_Device;
		//Public functions and members
		public:
			//Constructor
			Menu();
			//Destructor
			~Menu();
			//Pause
			void pause();
			//Unpause
			void unPause();
	};
	