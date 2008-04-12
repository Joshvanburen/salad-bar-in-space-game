/* CSCE 552
 * UserInterface.cpp
 * CPP file for the UserInterface class.
 */

// Include headers
#include "Common.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "LevelManager.h"
#include "irrnewt.hpp"
#include "Gravship.h"
#include "GameSystem.h"


// Class definition

GameSystem::GameSystem(){
	//Set all to null
	reverseGravity, gravityOn, up_momentum, right_momentum, left_momentum, up_momentum, melee, cycle_melee, shoot, cycle_weapon, cycle_morph, morph, hover, pause = NULL;

	m_Gravship = NULL;

	m_Cursor = NULL;

	m_Input_Mgr = InputManager::getSingletonPtr();
	m_Keyboard = &(m_Input_Mgr->getKeyboard());
	m_Wiimote = &(m_Input_Mgr->getWiimote());

	m_Score = 0;
	m_Lives = 0;

	m_CursorX = 0;
	m_CursorY = 0;
}


GameSystem::~GameSystem(){
	this->shutdown();
}


void GameSystem::shutdown(){
	m_Input_Mgr->deleteAction("up_momentum");
	m_Input_Mgr->deleteAction("right_momentum");
	m_Input_Mgr->deleteAction("left_momentum");
	m_Input_Mgr->deleteAction("up_momentum");



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
	
	reverseGravity, gravityOn, up_momentum, right_momentum, left_momentum, up_momentum, melee, cycle_melee, shoot, cycle_weapon, cycle_morph, morph, hover, pause = NULL;


}
void GameSystem::init(){
	//will add ability to change controls with XML file.
	up_momentum = m_Input_Mgr->createAction("up_momentum", *m_Keyboard, Input::Keyboard::KEY_W, Input::Action::BEHAVIOR_DETECT_PRESS);
	right_momentum = m_Input_Mgr->createAction("right_momentum", *m_Keyboard, Input::Keyboard::KEY_D, Input::Action::BEHAVIOR_DETECT_PRESS);
	left_momentum = m_Input_Mgr->createAction("left_momentum", *m_Keyboard, Input::Keyboard::KEY_A, Input::Action::BEHAVIOR_DETECT_PRESS);
	down_momentum = m_Input_Mgr->createAction("down_momentum", *m_Keyboard, Input::Keyboard::KEY_S, Input::Action::BEHAVIOR_DETECT_PRESS);
	gravityOn = m_Input_Mgr->createAction("gravity_on", *m_Keyboard, Input::Keyboard::KEY_SPACE, Input::Action::BEHAVIOR_DETECT_PRESS);

	reverseGravity = m_Input_Mgr->createAction("reverse_gravity", *m_Keyboard, Input::Keyboard::KEY_R, Input::Action::BEHAVIOR_DETECT_PRESS);

	gravityOn->addCode(Input::Wiimote::WII_B_BUTTON, *m_Wiimote);
	

	up_momentum->addCode(Input::Wiimote::WII_UP_BUTTON, *m_Wiimote);
	down_momentum->addCode(Input::Wiimote::WII_DOWN_BUTTON, *m_Wiimote);
	right_momentum->addCode(Input::Wiimote::WII_RIGHT_BUTTON, *m_Wiimote);
	left_momentum->addCode(Input::Wiimote::WII_LEFT_BUTTON, *m_Wiimote);
/*
	melee = m_Input_Mgr->createAction("melee", *m_Keyboard, Input::Keyboard::KEY_E, Input::Action::BEHAVIOR_DETECT_TAP);
	cycle_melee = m_Input_Mgr->createAction("cycle_melee", *m_Keyboard, Input::Keyboard::KEY_I, Input::Action::BEHAVIOR_DETECT_TAP);
	shoot = m_Input_Mgr->createAction("shoot", *m_Keyboard, Input::Keyboard::KEY_S, Input::Action::BEHAVIOR_DETECT_TAP);
	cycle_weapon = m_Input_Mgr->createAction("cycle_weapon", *m_Keyboard, Input::Keyboard::KEY_O, Input::Action::BEHAVIOR_DETECT_TAP);
	cycle_morph = m_Input_Mgr->createAction("cycle_morph", *m_Keyboard, Input::Keyboard::KEY_P, Input::Action::BEHAVIOR_DETECT_TAP);
	morph = m_Input_Mgr->createAction("morph", *m_Keyboard, Input::Keyboard::KEY_M, Input::Action::BEHAVIOR_DETECT_PRESS);
	hover = m_Input_Mgr->createAction("hover", *m_Keyboard, Input::Keyboard::KEY_C, Input::Action::BEHAVIOR_DETECT_PRESS);
	pause = m_Input_Mgr->createAction("pause", *m_Keyboard, Input::Keyboard::KEY_LSHIFT, Input::Action::BEHAVIOR_DETECT_TAP);
*/
	/*	grow->addCode(Input::Wiimote::WII_MOVE_HANDS_OUTWARD, *m_Wiimote);
	shrink->addCode(Input::Wiimote::WII_MOVE_HANDS_INWARD, *m_Wiimote);
	jump->addCode(Input::Wiimote::WII_RIGHT_HAND_UP, *m_Wiimote);
	right_momentum->addCode(Input::Wiimote::WII_RIGHT_HAND_RIGHT, *m_Wiimote);
	left_momentum->addCode(Input::Wiimote::WII_RIGHT_HAND_LEFT, *m_Wiimote);
	large_jump->addCode(Input::Wiimote::WII_BOTH_HANDS_DOWN, *m_Wiimote);
	melee->addCode(Input::Wiimote::WII_B_BUTTON, *m_Wiimote);
	cycle_melee->addCode(Input::Wiimote::WII_2_BUTTON, *m_Wiimote);
	shoot->addCode(Input::Wiimote::WII_A_BUTTON, *m_Wiimote);
	cycle_weapon->addCode(Input::Wiimote::WII_RIGHT_HAND_POINT_UP, *m_Wiimote);
	cycle_morph->addCode(Input::Wiimote::WII_C_BUTTON, *m_Wiimote);
	morph->addCode(Input::Wiimote::WII_Z_BUTTON, *m_Wiimote);
	hover->addCode(Input::Wiimote::WII_DRUM, *m_Wiimote);
	pause->addCode(Input::Wiimote::WII_HOME_BUTTON, *m_Wiimote);
*/	
	//Initialize GUI


	//Create mouse cursor

}



void GameSystem::startGame(){

	//Assumes player_Enemy is available for now.
	int entity_ID = EntityManager::getSingleton().getEntityID("player");

	if (entity_ID < 0){
		m_Gravship = NULL;
		return;
	}
	else{
		this->m_Gravship = dynamic_cast<Gravship*>(&(EntityManager::getSingleton().getEntity(entity_ID)));
		m_Cursor = LevelManager::getSingleton().getSceneManager()->addSphereSceneNode(5.0);
		//irr::scene::ISceneNode* node = LevelManager::getSingleton().getSceneManager()->addCubeSceneNode(5.0, m_Gravship->getSceneNode());
		//node->setMaterialTexture(0, LevelManager::getSingleton().getDriver()->getTexture("./res/textures/neon_green.png"));
		//node->setPosition(irr::core::vector3df(0.0f, 0.0f, -15.0f));

	}
}

void GameSystem::recoverAfterLevelChange(){
	//Need to get the new Enemy in the new level.  Assume it is available for now.
	//Assumes player_Enemy is available for now.
	int entity_ID = EntityManager::getSingleton().getEntityID("player");

	if (entity_ID < 0){
		m_Gravship = NULL;
		return;
	}
	else{
		this->m_Gravship = dynamic_cast<Gravship*>(&(EntityManager::getSingleton().getEntity(entity_ID)));
	}
}
void GameSystem::update() {
	irr::core::position2di mouse_change = InputManager::getSingleton().getMouse().getRelativePosition();
	irr::core::vector3df cursor_position = m_Cursor->getPosition();
	m_Cursor->setPosition(irr::core::vector3df(cursor_position.X-mouse_change.X, cursor_position.Y + mouse_change.Y, 0.0f));
	if (up_momentum->isPressed()){
		m_Gravship->getPhysicsBody()->setVelocity(irr::core::vector3df(0.0f, 3.0f, 0.0f));
	}
	if(down_momentum->isPressed()){
		m_Gravship->getPhysicsBody()->setVelocity(irr::core::vector3df(0.0f, -3.0f, 0.0f));
	}
	if(right_momentum->isPressed()){
		m_Gravship->getPhysicsBody()->setVelocity(irr::core::vector3df(3.0f, 0.0f, 0.0f));
	}
	if(left_momentum->isPressed()){
		m_Gravship->getPhysicsBody()->setVelocity(irr::core::vector3df(-3.0f, 0.0f, 0.0f));
	}

	if(gravityOn->isPressed()){
		m_Gravship->enableGravityField(true);
	}
	else{
		m_Gravship->enableGravityField(false);
	}

	if(reverseGravity->isPressed()){
		m_Gravship->reverseGravityField(true);
	}
	else{
		m_Gravship->reverseGravityField(false);
	}
}

// Draws the User Interface
void GameSystem::draw() {
	//Draw GUI
}
