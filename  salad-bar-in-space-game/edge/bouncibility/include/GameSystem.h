#pragma once

#include "Common.h"
#include "console.h"
/*
Notes for the game system class
Reads in controls from inputmapping.xml
Obtains a the name of the object it will be controlling from xml (hard coded = "player_Enemy")
Displays a UserInterface overlay with current game state
State changes are in scripts, the scripts simply change properties of the object changing states.
Should just define this in C++ for now.  Buttons to use, A, B, Z, C, drumming?, both outwards, both inwards, both down, right up, right right, right left, 
Can have many different types of morphs, these will be cycled between and can only have one at a time. 
Can also pick up temporary power ups that don't have a control.
Controls:
shoot = A
become larger = hands out
become smaller = hands in
force right = right right
force left = right left
small jump = right up
big jump = both down
Use current morph = Z
Change current morph = C
Hover = Drumming
Some sort of melee attack(spikes) = B
Some sort of right hand movement like reload to change gun type? Maybe pull upwards to point up then back down
Also might be possible to change melee type (1 and 2 buttons)?
GUI Elements:
Lives
Score
Health?
Current selected morph, faded next available morph
Current weapon, limited ammo shown?
Current melee weapon?
Current powerup being used (either morphed or picked up) (maybe blinks when running out)



Need to update the GameSystem between levels.  LevelManager should do this.
*/
namespace Input{
	class Action;
	class Keyboard;
	class Wiimote;
};

class InputManager;
class EntityManager;
class LevelManager;
class ScriptManager;
class PhysicsManager;
class WorldEntityAIManager;
class SoundManager;
class IC_Console;

class Gravship;


class GameSystem : public CSingleton<GameSystem>
{
	friend CSingleton;
	private:

		GameSystem();
		GameSystem(const GameSystem& rhs);

		~GameSystem();
		GameSystem& operator=(const GameSystem& rhs);

		//!Find the position the camera should be to enclose the set width and height of the current level.
		void positionCamera();

		void handleInput();

		void setupInput();

		InputManager* m_Input_Mgr;
		// Score variable
		int m_Score;
		// Life variable
		int m_Lives;

		static Gravship* s_Gravship;
		// Current powers (for display in UI)
		//vector<Power> powers;
		/**** Time? ****/

		// Pointers to keyboard and wiimote devices
		Input::Keyboard* m_Keyboard;
		Input::Wiimote* m_Wiimote;

		// Available actions
		Input::Action* reverseGravity;
			//Set all to null
		Input::Action* gravityOn;

		Input::Action* down_momentum;
		Input::Action* right_momentum;
		Input::Action* left_momentum;
		Input::Action* up_momentum;
		Input::Action* melee;
		Input::Action* cycle_melee;
		Input::Action* shoot;
		Input::Action* cycle_weapon;
		Input::Action* cycle_morph;
		Input::Action* morph;
		Input::Action* hover;
		Input::Action* pause;
		Input::Action* resync;	
		Input::Action* quit;

		int m_CursorX;
		int m_CursorY;

		float m_MinX;
		float m_MinY;
		float m_MaxX;
		float m_MaxY;


		irr::s32 m_FPS;
		irr::u32 m_DeltaMillis;

		irr::gui::IGUIEnvironment* m_GUI;
		irr::scene::ICameraSceneNode* m_Camera;
		irr::scene::ISceneManager* m_SceneMgr;
		irr::IrrlichtDevice* m_Device;
		irr::video::IVideoDriver* m_Driver;

		LevelManager* m_LevelMgr;
		PhysicsManager* m_PhysicsMgr;
		IC_Console m_Console;



			
	// Public methods and variables
	public:

		IC_Console& getConsole();
		void init();
		void update();
		void startGame();
		void recoverAfterLevelChange();
		static Gravship* getGravship();
		void draw();
		void run();
		void shutdown();

		irr::u32 getDeltaMillis(){
			return m_DeltaMillis;
		}

		irr::s32 getFPS(){
			return m_FPS;
		}

		static EntityManager* entityManager;
		static LevelManager* levelManager;
		static ScriptManager* scriptManager;
		static SoundManager* soundManager;
		static PhysicsManager* physicsManager;
		static InputManager* inputManager;
		static WorldEntityAIManager* aiManager;
		static GameSystem* gameSystem;



};