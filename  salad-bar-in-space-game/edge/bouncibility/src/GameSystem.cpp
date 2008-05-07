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
#include "Pause.h"

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

Bullet* GameSystem::bulletSrc = NULL;
//Menu* GameSystem::menu = Menu::getSingletonPtr();
Pause p;

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
	pause = NULL;
	start = NULL;

	m_SplashTTL = 2000;
	m_FireRate = 1000;
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
	pause = NULL;
	start = NULL;
	resync = NULL;

	image = NULL;
	hudImage = NULL;
	titleScreen = NULL;
	title = NULL;
	splash = NULL;
	splashScreen = NULL;

	m_Input_Mgr->shutdown();
	m_LevelMgr->shutdown();
	SoundManager::getSingleton().shutdown();
	m_PhysicsMgr->shutdown();
	ScriptManager::getSingleton().shutdown();
	WorldEntityAIManager::getSingleton().shutdown();
	if (m_Device->getReferenceCount() > 0)
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


	m_ScreenDim.set(1280, 1024);
	m_Device = irr::createDevice( irr::video::EDT_DIRECT3D9, m_ScreenDim, 24,
		true, false, false, InputManager::getSingleton().getEventReceiver());

	m_SceneMgr = m_Device->getSceneManager();

	WorldEntityAIManager::getSingleton().init();
	//PhysicsManager must be initialized before LevelManager because Entity initialization requires physics.
	PhysicsManager::getSingleton().init(m_Device);

	this->setupInput();

	//Menu::getSingleton().init(m_Device);

	LevelManager::getSingleton().init(m_Device, "./res/scenarios/tutorial.xml");

	
	LevelManager::getSingleton().startGame();


	m_Driver = m_Device->getVideoDriver();

	m_Device->getCursorControl()->setVisible(false);

	m_Console.getConfig().dimensionRatios.Y = 0.8f;

	m_Console.initializeConsole(m_Device->getGUIEnvironment(), m_ScreenDim);
	
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

	hudImage = m_Driver->getTexture("./res/textures/HeadsUp.png");
	titleScreen = m_Driver->getTexture("./textures/cats/splash2.png");
	splashScreen = m_Driver->getTexture("./textures/cats/TittleScreen.png");
	gameOver = m_Driver->getTexture("./res/textures/gameOver.png");


	image = m_GUI->addImage(hudImage, irr::core::position2d<irr::s32>(0, 0), true);
	image->setVisible(false);
	title = m_GUI->addImage(titleScreen, irr::core::position2d<irr::s32>(0,0), true);
	title->setVisible(true);
	splash = m_GUI->addImage(splashScreen, irr::core::position2d<irr::s32>(0,0), true);
	splash->setVisible(false);
	over = m_GUI->addImage(gameOver, irr::core::position2d<irr::s32>(0,0), true);
	over->setVisible(false);


	image->setMinSize(irr::core::dimension2di(1280, 1024));
	image->setMaxSize(irr::core::dimension2di(1280, 1024));
	title->setMinSize(irr::core::dimension2di(1280, 1024));
	title->setMaxSize(irr::core::dimension2di(1280, 1024));
	splash->setMinSize(irr::core::dimension2di(1280, 1024));
	splash->setMaxSize(irr::core::dimension2di(1280, 1024));
	over->setMinSize(irr::core::dimension2di(1280, 1024));
	over->setMaxSize(irr::core::dimension2di(1280, 1024));
	//this->m_PointsDisplay = m_GUI->addStaticText(L"0", irr::core::rect<s32>(1100,10,1250,30), true, false);


	//m_PointsDisplay->setOverrideColor(irr::video::SColor(0, 255, 255, 255));
	ScriptManager::getSingleton().registerScriptFunction("main", new Scripting::MainFunction());

	IC_Command* cmd = new EXECUTESCRIPT();
	m_Console.registerCommand(cmd);

	cmd = new LOADSCRIPT();
	m_Console.registerCommand(cmd);


	//Create mouse cursor

}

void GameSystem::appendSplashScreen(irr::gui::IGUIImage* image){
	m_SplashTTL = 2000;
	m_SplashScreens.push_back(image);
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
		this->s_Gravship->setLocation(s_Gravship->getLocation().X, s_Gravship->getLocation().Y, 780);
		s_Gravship->getHelper()->setLocation(s_Gravship->getLocation().X, s_Gravship->getLocation().Y, s_Gravship->getLocation().Z - 400.0f);
		LevelManager::getSingleton().getCurrentLevel().getCamera()->setParent(s_Gravship->getSceneNode());
		s_Gravship->getHelper()->getPhysicsBody()->getMaterial()->setCollidable(PhysicsManager::getSingleton().getMaterial("enemy"), true);
		//irr::scene::ISceneNode* node = LevelManager::getSingleton().getSceneManager()->addCubeSceneNode(5.0, s_Gravship->getSceneNode());
		//node->setMaterialTexture(0, LevelManager::getSingleton().getDriver()->getTexture("./res/textures/neon_green.png"));
		//node->setPosition(irr::core::vector3df(0.0f, 0.0f, -15.0f));
	}

	irr::core::dimension2di dimensions= LevelManager::getSingleton().getCurrentLevel().getDimensions();
	m_MinY = 0;
	m_MaxY = dimensions.Height;
	m_MinX = 0;
	m_MaxX = dimensions.Width;

	bulletSrc = new Bullet;
	//this->getbulletSrc = &bulletSrc;
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
	bool firstTime = true;
	bool quitOnce = true;
	int i = 0;

	irr::u32 r = 0;
	irr::u32 b = 0;
	irr::u32 g = 255;

	while(m_Device->run())
	{

		//menu->init(m_Device);

		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene clears the screen with a color and also the
		depth buffer if wanted. Then we let the Scene Manager and the
		GUI Environment draw their content. With the endScene() call
		everything is presented on the screen.
		*/
		m_FPS = m_Driver->getFPS();

		irr::core::stringw tmp(L"FPS: ");
		tmp += m_FPS;
		m_Device->setWindowCaption(tmp.c_str());
		m_FireRate -= this->getDeltaMillis();
		m_SplashTTL -= this->getDeltaMillis();
		if(m_SplashTTL < 0){
			m_SplashTTL = 2000;
			if (m_SplashScreens.size() > 0){
				irr::gui::IGUIImage* image = m_SplashScreens.front();
				image->remove();
				m_SplashScreens.pop_front();
			}	
		}
		if(firstTime){
			p.pause();
			if(start->isPressed()){
				if( title->isVisible() ){
					title->setVisible(false);
					splash->setVisible(true);
					image->setVisible(false);
				}else if( splash->isVisible() ){
					title->setVisible(false);
					splash->setVisible(false);
					image->setVisible(true);
					firstTime = false;
					p.unPause();
				}
			}	
		}

		if( pause->isPressed() ){
			if(p.isPaused() ){
				splash->setVisible(false);
				image->setVisible(true);
				p.unPause();
			}
			else{
				splash->setVisible(true);
				image->setVisible(false);
				p.pause();
			}
		}

		if(m_FPS > 0){
			m_DeltaMillis  = (irr::u32) (1000.0f / (irr::f32)m_FPS);
		}



		m_Driver->beginScene(true, true, SColor(255,100,101,140));
		if (m_FPS > 0){
			if(!p.isPaused()){
				m_Input_Mgr->stopPolling();
				m_PhysicsMgr->update();
				m_LevelMgr->update();
				//m_SceneMgr->drawAll();
				//update();
			}
			m_SceneMgr->drawAll();
			m_Driver->draw2DRectangle(irr::video::SColor(255, r, g, b), irr::core::rect<irr::s32>(irr::core::position2di(640-s_Gravship->getHealth(), 10), irr::core::position2di(640+s_Gravship->getHealth(), 45)));
			
			if( s_Gravship->getHealth() <= 75 ){
				r = 255;
				g = 0;
			}


			if(s_Gravship->getHealth() <= 0 ){
				p.pause();
				image->setVisible(false);
				over->setVisible(true);
				quitOnce = false;
			}

			update();
			m_Input_Mgr->getInput();
		}
		
		m_GUI->drawAll();
		m_Console.renderConsole(m_GUI,m_Driver,m_DeltaMillis);
		m_Driver->endScene();

		if( quit->isPressed() ){
			if(quitOnce){
				//m_LevelMgr->repeat();
				p.pause();
				image->setVisible(false);
				over->setVisible(true);
				quitOnce = false;
			}else{
				exit(0);
			}
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

	shoot = m_Input_Mgr->createAction("shoot", *m_Keyboard, Input::Keyboard::KEY_Z, Input::Action::BEHAVIOR_DETECT_RELEASE);


	shoot->addCode(Input::Wiimote::WII_B_BUTTON, *m_Wiimote);

	reverseGravity = m_Input_Mgr->createAction("reverse_gravity", *m_Keyboard, Input::Keyboard::KEY_R, Input::Action::BEHAVIOR_DETECT_PRESS);
	

	
	resync = m_Input_Mgr->createAction("resync", *m_Wiimote, Input::Wiimote::WII_2_BUTTON, Input::Action::BEHAVIOR_DETECT_PRESS);
	up_momentum->addCode(Input::Wiimote::WII_UP_BUTTON, *m_Wiimote);
	down_momentum->addCode(Input::Wiimote::WII_DOWN_BUTTON, *m_Wiimote);
	right_momentum->addCode(Input::Wiimote::WII_RIGHT_BUTTON, *m_Wiimote);
	left_momentum->addCode(Input::Wiimote::WII_LEFT_BUTTON, *m_Wiimote);

	
	
	quit = InputManager::getSingleton().createAction("quit", InputManager::getSingleton().getKeyboard(), Input::Keyboard::KEY_ESC, Input::Action::BEHAVIOR_DETECT_RELEASE);
	quit->addCode(Input::Wiimote::WII_HOME_BUTTON, InputManager::getSingleton().getWiimote());

	pause = InputManager::getSingleton().createAction("pause", InputManager::getSingleton().getKeyboard(), Input::Keyboard::KEY_P, Input::Action::BEHAVIOR_DETECT_RELEASE );
	pause->addCode(Input::Wiimote::WII_PLUS_BUTTON, InputManager::getSingleton().getWiimote());

	start = InputManager::getSingleton().createAction("start", InputManager::getSingleton().getKeyboard(), Input::Keyboard::KEY_SPACE, Input::Action::BEHAVIOR_DETECT_RELEASE );
	start->addCode(Input::Wiimote::WII_MINUS_BUTTON, InputManager::getSingleton().getWiimote());

}
void GameSystem::handleInput(){
	if (up_momentum->isPressed()){
		s_Gravship->getPhysicsBody()->setVelocity(irr::core::vector3df(s_Gravship->getPhysicsBody()->getVelocity().X, 15.0f, 0.0f));
	}
	if(down_momentum->isPressed()){
		s_Gravship->getPhysicsBody()->setVelocity(irr::core::vector3df(s_Gravship->getPhysicsBody()->getVelocity().X, -15.0f, 0.0f));
	}
	if(right_momentum->isPressed()){
		s_Gravship->getPhysicsBody()->setVelocity(irr::core::vector3df(-15.0f, s_Gravship->getPhysicsBody()->getVelocity().Y, 0.0f));
	}
	if(left_momentum->isPressed()){
		s_Gravship->getPhysicsBody()->setVelocity(irr::core::vector3df(15.0f, s_Gravship->getPhysicsBody()->getVelocity().Y, 0.0f));
	}
	if (!up_momentum->isPressed() && !down_momentum->isPressed() && !right_momentum->isPressed() && !left_momentum->isPressed()){
		s_Gravship->getPhysicsBody()->setVelocity(irr::core::vector3df(0.0f, 0.0f, 0.0f));
	}
	if(shoot->isPressed()){
		if (m_FireRate < 0){
			m_FireRate = 1000;
			s_Gravship->shoot();
		}
		
	}

	LevelManager::getSingleton().getCurrentLevel().getCamera()->setTarget(irr::core::vector3df(s_Gravship->getPhysicsBody()->getPosition().X, s_Gravship->getPhysicsBody()->getPosition().Y, -10000));
	s_Gravship->getHelper()->setLocation(s_Gravship->getLocation().X, s_Gravship->getLocation().Y, s_Gravship->getLocation().Z - 500);
	if (resync->isPressed()){
		m_Wiimote->resync();

	}	
	//std::cout << m_Wiimote->getPitch() << "\n";
	//s_Gravship->getHelper()->getPhysicsBody()->setRotation(irr::core::vector3df(0.0f, m_Wiimote->getPitch(), 0.0f));
	if(gravityOn->isPressed()){
		s_Gravship->enableGravityField(true);
	}
	else{
		s_Gravship->enableGravityField(false);
	}

	//if(reverseGravity->isPressed()){
	//	s_Gravship->getHelper()->reverseGravityField(true);
	//	std::cout << EntityManager::getSingleton().getEntityID("spawner") << "\n";
	//	((Spawner*)EntityManager::getSingleton().getEntity(11))->pause(true);
	//}
	//else{
	//	s_Gravship->getHelper()->reverseGravityField(false);
	//}

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
	//irr::core::vector3df direction(0.0f, 1.0f, 0.0f);

	
	//direction.rotateXYBy(-m_Wiimote->joystickAngle(),irr::core::vector3df(0.0f, 0.0f, 0.0f ));
	//direction = direction * 20 * m_Wiimote->joystickMagnitude();
	
	//irr::core::vector3df rotation(direction.getHorizontalAngle());
	//s_Gravship->setRotation(irr::core::vector3df(s_Gravship->getSceneNode()->getRotation().X, s_Gravship->getSceneNode()->getRotation().Y, rotation.Y - rotation.X));

	//s_Gravship->getPhysicsBody()->setVelocity(direction);


}

// Draws the User Interface
void GameSystem::draw() {
	//Draw GUI
}

