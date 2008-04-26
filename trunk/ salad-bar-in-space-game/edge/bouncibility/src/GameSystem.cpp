/* CSCE 552
 * UserInterface.cpp
 * CPP file for the UserInterface class.
 */

// Include headers
#include "Common.h"
#include "GameIncludes.h"
#include "GameSystem.h"
#include "MastEventReceiver.cpp"
#include "console.h"
#include "ExecScriptCommand.h"
#include "LoadScriptCommand.h"

using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::gui;

EntityManager* GameSystem::entityManager = EntityManager::getSingletonPtr();
LevelManager* GameSystem::levelManager = LevelManager::getSingletonPtr();
ScriptManager* GameSystem::scriptManager = ScriptManager::getSingletonPtr();
SoundManager* GameSystem::soundManager = SoundManager::getSingletonPtr();
PhysicsManager* GameSystem::physicsManager = PhysicsManager::getSingletonPtr();
InputManager* GameSystem::inputManager = InputManager::getSingletonPtr();
WorldEntityAIManager* GameSystem::aiManager = WorldEntityAIManager::getSingletonPtr();
GameSystem* GameSystem::gameSystem = GameSystem::getSingletonPtr();
void grab(int v)
{
	GameSystem::getSingleton().getConsole().appendMessage(WideString(v));
}

void grab(asUINT v)
{
	GameSystem::getSingleton().getConsole().appendMessage(WideString(v));
}

void grab(WorldEntity* v){
	GameSystem::getSingleton().getConsole().appendMessage(WideString(IC_StrConv::toWideString("WorldEntity*"))); 
}
void grab(bool v)
{
	if (v){
		GameSystem::getSingleton().getConsole().appendMessage("true");
	}
	else{
		GameSystem::getSingleton().getConsole().appendMessage("false");

	}
}

void grab(float v)
{
	
	char* str = new char[30];
	sprintf(str, "%.4g", v);
	GameSystem::getSingleton().getConsole().appendMessage(WideString(IC_StrConv::toWideString(String(str))));
	delete[] str;
}

void grab(double v)
{
	char* str = new char[30];
	sprintf(str, "%.4g", v);
	GameSystem::getSingleton().getConsole().appendMessage(WideString(IC_StrConv::toWideString(String(str))));
	delete[] str;
}

void grab(const std::string &v)
{
	
	GameSystem::getSingleton().getConsole().appendMessage(WideString(IC_StrConv::toWideString(v.c_str())));
}

void grab()
{
	// There is no value
}


IC_Console& GameSystem::getConsole(){
	return m_Console;
}

void PrintString(std::string& str){

	std::cout << str << std::endl;

}

// Class definition

Gravship* GameSystem::s_Gravship = NULL;
Gravship* GameSystem::getGravship(){
		return s_Gravship;
}
GameSystem::GameSystem(){
	//Set all to null
	reverseGravity, gravityOn, up_momentum, right_momentum, left_momentum, up_momentum, melee, cycle_melee, shoot, cycle_weapon, cycle_morph, morph, hover, pause = NULL;

	resync = NULL;
	s_Gravship = NULL;
	m_Camera = NULL;

	m_Input_Mgr = InputManager::getSingletonPtr();
	m_Keyboard = &(m_Input_Mgr->getKeyboard());
	m_Wiimote = &(m_Input_Mgr->getWiimote());

	m_Score = 0;
	m_Lives = 0;

	m_CursorX = 0;
	m_CursorY = 0;

	 m_DeltaMillis = 0;

	 m_GUI = NULL;

	m_MaxX = 0;
	m_MaxY = 0;
	m_MinX = 0;
	m_MinY = 0;

	m_LevelMgr = NULL;
	m_PhysicsMgr = NULL;

	m_SceneMgr = NULL;
	m_Driver = NULL;
	m_Device = NULL;

	quit = NULL;
}


GameSystem::~GameSystem(){
	this->shutdown();
}


void GameSystem::shutdown(){

	m_Input_Mgr->deleteAction("up_momentum");
	m_Input_Mgr->deleteAction("right_momentum");
	m_Input_Mgr->deleteAction("left_momentum");
	m_Input_Mgr->deleteAction("up_momentum");
	m_Input_Mgr->deleteAction("resync");


	m_Input_Mgr->deleteAction("reverse_gravity");
	m_Input_Mgr->deleteAction("gravity_on");

	m_Input_Mgr->deleteAction("melee");
	m_Input_Mgr->deleteAction("cycle_melee");
	m_Input_Mgr->deleteAction("shoot");
	m_Input_Mgr->deleteAction("cycle_weapon");
	m_Input_Mgr->deleteAction("cycle_morph");
	m_Input_Mgr->deleteAction("morph");
	m_Input_Mgr->deleteAction("hover");
	m_Input_Mgr->deleteAction("pause");
	
	m_Input_Mgr->deleteAction("quit");

	reverseGravity, gravityOn, up_momentum, right_momentum, left_momentum, up_momentum, melee, cycle_melee, shoot, cycle_weapon, cycle_morph, morph, hover, pause = NULL;

	quit = NULL;
	resync = NULL;

	m_Input_Mgr->shutdown();
	m_LevelMgr->shutdown();
	SoundManager::getSingleton().shutdown();
	m_PhysicsMgr->shutdown();
	ScriptManager::getSingleton().shutdown();
	WorldEntityAIManager::getSingleton().shutdown();
	m_Device->drop();


}
void GameSystem::init(){


	ScriptManager::getSingleton().init();

	SoundManager::getSingleton().init();

	ScriptManager::getSingleton().registerAsGlobal("void PrintString(string &in)", ::asFUNCTION(PrintString));
	InputManager::getSingleton().init();

	m_Input_Mgr = InputManager::getSingletonPtr();
	m_LevelMgr = LevelManager::getSingletonPtr();
	m_PhysicsMgr = PhysicsManager::getSingletonPtr();

	dimension2d<s32> screenDim(1280,1024);

	m_Device = irr::createDevice( irr::video::EDT_DIRECT3D9, screenDim, 24,
		false, true, true, InputManager::getSingleton().getEventReceiver());

	m_SceneMgr = m_Device->getSceneManager();

	WorldEntityAIManager::getSingleton().init();
	//PhysicsManager must be initialized before LevelManager because Entity initialization requires physics.
	PhysicsManager::getSingleton().init(m_Device);

	this->setupInput();

	LevelManager::getSingleton().init(m_Device, "./res/scenarios/tutorial.xml");

	
	LevelManager::getSingleton().startGame();


	m_Driver = m_Device->getVideoDriver();

	m_Device->getCursorControl()->setVisible(false);

	m_Console.getConfig().dimensionRatios.Y = 0.8f;

	m_Console.initializeConsole(m_Device->getGUIEnvironment(), screenDim);
	
	//register common commands
	m_Console.loadDefaultCommands(m_Device);


		// Register special function with overloads to catch any type.
	// This is used by the exec command to output the resulting value from the statement.
	ScriptManager::getSingleton().registerAsGlobal("void _grab(bool)", asFUNCTIONPR(grab, (bool), void));
	ScriptManager::getSingleton().registerAsGlobal("void _grab(int)", asFUNCTIONPR(grab, (int), void));
	ScriptManager::getSingleton().registerAsGlobal("void _grab(uint)", asFUNCTIONPR(grab, (asUINT), void));
	ScriptManager::getSingleton().registerAsGlobal("void _grab(float)", asFUNCTIONPR(grab, (float), void));
	ScriptManager::getSingleton().registerAsGlobal("void _grab(double)", asFUNCTIONPR(grab, (double), void));
	ScriptManager::getSingleton().registerAsGlobal("void _grab()", asFUNCTIONPR(grab, (), void));
	ScriptManager::getSingleton().registerAsGlobal("void _grab(const string& in)", asFUNCTIONPR(grab, (const std::string&), void));
	ScriptManager::getSingleton().registerAsGlobal("void _grab(WorldEntity& in)", asFUNCTIONPR(grab, (WorldEntity*), void));
	

	m_GUI = m_Device->getGUIEnvironment();
	//Initialize GUI

	ScriptManager::getSingleton().registerScriptFunction("main", new Scripting::MainFunction());


	IC_Command* cmd = new EXECUTESCRIPT();
	m_Console.registerCommand(cmd);

	cmd = new LOADSCRIPT();
	m_Console.registerCommand(cmd);


	//Create mouse cursor

}



void GameSystem::startGame(){

	//Assumes player_Enemy is available for now.
	int entity_ID = EntityManager::getSingleton().getEntityID("player");

	if (entity_ID < 0){
		s_Gravship = NULL;
		return;
	}
	else{
		this->s_Gravship = dynamic_cast<Gravship*>((EntityManager::getSingleton().getEntity(entity_ID)));
		//irr::scene::ISceneNode* node = LevelManager::getSingleton().getSceneManager()->addCubeSceneNode(5.0, s_Gravship->getSceneNode());
		//node->setMaterialTexture(0, LevelManager::getSingleton().getDriver()->getTexture("./res/textures/neon_green.png"));
		//node->setPosition(irr::core::vector3df(0.0f, 0.0f, -15.0f));
	}

	irr::core::dimension2di dimensions= LevelManager::getSingleton().getCurrentLevel().getDimensions();
	m_MinY = 0;
	m_MaxY = dimensions.Height;
	m_MinX = 0;
	m_MaxX = dimensions.Width;

//	positionCamera();

}

void GameSystem::recoverAfterLevelChange(){
	//Need to get the new Enemy in the new level.  Assume it is available for now.
	//Assumes player_Enemy is available for now.
	int entity_ID = EntityManager::getSingleton().getEntityID("player");

	if (entity_ID < 0){
		s_Gravship = NULL;
		return;
	}
	else{
		this->s_Gravship = dynamic_cast<Gravship*>((EntityManager::getSingleton().getEntity(entity_ID)));
	}
}

void GameSystem::positionCamera(){
	irr::core::vector3df camera_location(m_Camera->getPosition());
	irr::core::vector3df camera_look(m_Camera->getTarget());
	camera_location.rotateYZBy(-20, camera_look);
	
	m_Camera->setPosition(camera_location);

}

void GameSystem::run(){
	while(m_Device->run())
	{
		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene clears the screen with a color and also the
		depth buffer if wanted. Then we let the Scene Manager and the
		GUI Environment draw their content. With the endScene() call
		everything is presented on the screen.
		*/
		m_FPS = m_Driver->getFPS();
		if(m_FPS > 0)
		{
			m_DeltaMillis  = (irr::u32) (1000.0f / (irr::f32)m_FPS);
		}
		m_Input_Mgr->stopPolling();
		m_PhysicsMgr->update();
		m_LevelMgr->update();

		update();

		m_Input_Mgr->getInput();
		m_Driver->beginScene(true, true, SColor(255,100,101,140));
			m_SceneMgr->drawAll();
			m_GUI->drawAll();
			m_Console.renderConsole(m_GUI,m_Driver,m_DeltaMillis);
		m_Driver->endScene();

		if(quit->isPressed()){
			break;
		}
		m_Input_Mgr->resumePolling();
	}

}

void GameSystem::setupInput(){
	//will add ability to change controls with XML file.
	up_momentum = m_Input_Mgr->createAction("up_momentum", *m_Keyboard, Input::Keyboard::KEY_W, Input::Action::BEHAVIOR_DETECT_PRESS);
	right_momentum = m_Input_Mgr->createAction("right_momentum", *m_Keyboard, Input::Keyboard::KEY_D, Input::Action::BEHAVIOR_DETECT_PRESS);
	left_momentum = m_Input_Mgr->createAction("left_momentum", *m_Keyboard, Input::Keyboard::KEY_A, Input::Action::BEHAVIOR_DETECT_PRESS);
	down_momentum = m_Input_Mgr->createAction("down_momentum", *m_Keyboard, Input::Keyboard::KEY_S, Input::Action::BEHAVIOR_DETECT_PRESS);
	gravityOn = m_Input_Mgr->createAction("gravity_on", *m_Keyboard, Input::Keyboard::KEY_SPACE, Input::Action::BEHAVIOR_DETECT_PRESS);

	reverseGravity = m_Input_Mgr->createAction("reverse_gravity", *m_Keyboard, Input::Keyboard::KEY_R, Input::Action::BEHAVIOR_DETECT_PRESS);
	
	reverseGravity->addCode(Input::Wiimote::WII_B_BUTTON, *m_Wiimote);

	gravityOn->addCode(Input::Wiimote::WII_A_BUTTON, *m_Wiimote);
	
	resync = m_Input_Mgr->createAction("resync", *m_Wiimote, Input::Wiimote::WII_2_BUTTON, Input::Action::BEHAVIOR_DETECT_PRESS);
	up_momentum->addCode(Input::Wiimote::WII_UP_BUTTON, *m_Wiimote);
	down_momentum->addCode(Input::Wiimote::WII_DOWN_BUTTON, *m_Wiimote);
	right_momentum->addCode(Input::Wiimote::WII_RIGHT_BUTTON, *m_Wiimote);
	left_momentum->addCode(Input::Wiimote::WII_LEFT_BUTTON, *m_Wiimote);

	
	
	quit = InputManager::getSingleton().createAction("quit", InputManager::getSingleton().getKeyboard(), Input::Keyboard::KEY_ESC, Input::Action::BEHAVIOR_DETECT_TAP);
	quit->addCode(Input::Wiimote::WII_HOME_BUTTON, InputManager::getSingleton().getWiimote());

	



}
void GameSystem::handleInput(){
	if (up_momentum->isPressed()){
		s_Gravship->getPhysicsBody()->setVelocity(irr::core::vector3df(s_Gravship->getPhysicsBody()->getVelocity().X, 0.0f, 3.0f));
	}
	if(down_momentum->isPressed()){
		s_Gravship->getPhysicsBody()->setVelocity(irr::core::vector3df(s_Gravship->getPhysicsBody()->getVelocity().X, 0.0f, -3.0f));
	}
	if(right_momentum->isPressed()){
		s_Gravship->getPhysicsBody()->setVelocity(irr::core::vector3df(3.0f, s_Gravship->getPhysicsBody()->getVelocity().Y, 0.0f));
	}
	if(left_momentum->isPressed()){
		s_Gravship->getPhysicsBody()->setVelocity(irr::core::vector3df(-3.0f, s_Gravship->getPhysicsBody()->getVelocity().Y, 0.0f));
	}

	if (resync->isPressed()){
		m_Wiimote->resync();

	}	
	if(gravityOn->isPressed()){
		s_Gravship->enableGravityField(true);
	}
	else{
		s_Gravship->enableGravityField(false);
	}

	if(reverseGravity->isPressed()){
		s_Gravship->getHelper()->reverseGravityField(true);
	}
	else{
		s_Gravship->getHelper()->reverseGravityField(false);
	}

}
void GameSystem::update() {

	handleInput();

	irr::core::position2di mouse_change = m_Input_Mgr->getMouse().getRelativePosition();

	irr::core::position2df wiimote_change = m_Wiimote->getRelativePosition();

	irr::core::vector3df cursor_position = s_Gravship->getHelper()->getLocation();
	float newX = cursor_position.X+wiimote_change.X-mouse_change.X;
	float newY = cursor_position.Z-wiimote_change.Y+mouse_change.Y;;

	if (newX > m_MaxX){
		newX = m_MaxX;
	}
	else if (newY > m_MaxY){
		newY = m_MaxY;
	}
	else if (newX < m_MinX){
		newX = m_MinX;
	}
	else if (newY < m_MinY){
		newY = m_MinY;
	}
	s_Gravship->getHelper()->setLocation(irr::core::vector3df(newX, s_Gravship->getHelper()->getLocation().Y, newY));
	//s_Gravship->getHelper()->setLocation(irr::core::vector3df(cursor_position.X-wiimote_change.X, cursor_position.Y + wiimote_change.Y, 0.0f));
	irr::core::vector3df direction(0.0f, 1.0f, 0.0f);

	
	direction.rotateXYBy(-m_Wiimote->joystickAngle(),irr::core::vector3df(0.0f, 0.0f, 0.0f ));
	direction = direction * 5 * m_Wiimote->joystickMagnitude();
	irr::core::vector3df rotation(direction.getHorizontalAngle());
	s_Gravship->setRotation(irr::core::vector3df(s_Gravship->getSceneNode()->getRotation().X, s_Gravship->getSceneNode()->getRotation().Y, rotation.Y - rotation.X));

	s_Gravship->getPhysicsBody()->setVelocity(direction);


}

// Draws the User Interface
void GameSystem::draw() {
	//Draw GUI
}

