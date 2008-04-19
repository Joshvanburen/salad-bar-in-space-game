#include "Common.h"
#include "MastEventReceiver.cpp"
#include "GameIncludes.h"
#include "wiiuse.h"
#define WIIUSE_PATH		"wiiuse.dll"
//Interface methods don't need an implementation, yet.
bool Input::InputDevice::init(Input::InputDeviceInit &deviceInit){
	this->m_name = deviceInit.m_name;
	receiver = NULL;
	return true;
}
void Input::InputDevice::release(){
}

bool Input::InputDevice::read(){
	return true;
}

bool Input::InputDevice::buttonStatus(int code) const{
	return true;
}

Input::InputDevice::InputDevice() : receiver(NULL){
}

Input::InputDevice::~InputDevice(){
}
//End interface methods

Input::Mouse::Mouse() : m_AbsX(0), m_AbsY(0), m_dx(0), m_dy(0){
	m_ScreenCenter.X = 0;
	m_ScreenCenter.Y = 0;
}

Input::Mouse::~Mouse(){
	release();
}

const int Input::Mouse::MOUSE_LEFT_BUTTON = 0;
const int Input::Mouse::MOUSE_MIDDLE_BUTTON = 1;
const int Input::Mouse::MOUSE_RIGHT_BUTTON = 2;

bool Input::Mouse::init(Input::InputDeviceInit &deviceInit){
	InputDevice::init(deviceInit);

	return true;
}

bool Input::Mouse::read(){
	if (m_ScreenCenter.X == 0 && m_ScreenCenter.Y == 0){
		m_ScreenCenter = irr::core::position2di(LevelManager::getDriver()->getScreenSize().Width*0.5, LevelManager::getSingleton().getDriver()->getScreenSize().Height*0.5);
	}
	InputDevice::read();
	irr::core::position2di mouse_pos = LevelManager::getDevice()->getCursorControl()->getPosition();
	this->m_dx += m_ScreenCenter.X - mouse_pos.X;
	this->m_dy += m_ScreenCenter.Y - mouse_pos.Y;
	LevelManager::getDevice()->getCursorControl()->setPosition(m_ScreenCenter); 
	
	return true;
}

void Input::Mouse::release(){
	InputDevice::release();
}

bool Input::Mouse::buttonStatus(int code) const{
	InputDevice::buttonStatus(code);
	
	switch (code){
		case MOUSE_LEFT_BUTTON:
			return receiver->leftMouseDown();
			break;
		case MOUSE_MIDDLE_BUTTON:
			return receiver->middleMouseDown();
			break;
		case MOUSE_RIGHT_BUTTON:
			return receiver->rightMouseDown();
			break;
	}

	return false;

}

bool Input::Mouse::operator()(int code) const{
	return buttonStatus(code);
}


bool Input::Wiimote::init(Input::InputDeviceInit &deviceInit){
	int connected = 0;
	//call the base class version first.
	InputDevice::init(deviceInit);

	Input::WiimoteInit *wiimoteInit = ((WiimoteInit*)&deviceInit);

	//wiiuse_startup(WIIUSE_PATH);

	moteID = 1;
	
	wiimotes =  wiiuse_init(1);

	std::cout << "wiimotes = " << wiimotes[0] << std::endl;
	found = wiiuse_find(wiimotes, 1, 5);
std::cout << "wiimotes = " << wiimotes[0] << std::endl;
	if (!found)
		return false;

	
	wiiuse_set_timeout(wiimotes, 1, 10, 20);
	wiiuse_set_nunchuk_accel_threshold(wiimotes[0], 5);
	wiiuse_set_nunchuk_orient_threshold(wiimotes[0], 10);

	connected = wiiuse_connect(wiimotes, 1);
	if (connected)
		printf("Connected to %i wiimotes (of %i found).\n", connected, found);
	else {
		printf("Failed to connect to any wiimote.\n");
		return false;
	}

	wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1 | WIIMOTE_LED_4);
	wiiuse_rumble(wiimotes[0], 1);

	Sleep(200);

	wiiuse_rumble(wiimotes[0], 0);
	wiiuse_motion_sensing(wiimotes[0], 0);
	wiiuse_set_ir(wiimotes[0], 1);
	wiiuse_set_ir_vres(wiimotes[0], 1000, 1000);
	wiiuse_set_ir_position(wiimotes[0], WIIUSE_IR_BELOW);



	return true;
}
void Input::Wiimote::poll(){
	if (!found){
		return;
	}
	if (wiiuse_poll(wiimotes, 1)) {
	/*
	 *	This happens if something happened on any wiimote.
	 *	So go through each one and check if anything happened.
	 */
		int i = 0;
		for (; i < 1; ++i) {
			switch (wiimotes[i]->event) {
				case WIIUSE_EVENT:
					/* a generic event occured */
					handle_event(wiimotes[i]);
					break;

				case WIIUSE_STATUS:
					/* a status event occured */
					handle_ctrl_status(wiimotes[i]);
					break;

				case WIIUSE_DISCONNECT:
					/* the wiimote disconnected */
					//handle_disconnect(wiimotes[i]);
					break;

				default:
					break;
			}
		}
	}
}
bool Input::Wiimote::read(){
	InputDevice::read();

	if (!found){
		return false;
	}
//	if (wiiuse_poll(wiimotes, 1)) {
	/*
	 *	This happens if something happened on any wiimote.
	 *	So go through each one and check if anything happened.
	 */
	//	int i = 0;
	//	for (; i < 1; ++i) {
	//		switch (wiimotes[i]->event) {
	//			case WIIUSE_EVENT:
	//				/* a generic event occured */
	//				handle_event(wiimotes[i]);
	//				break;

	//			case WIIUSE_STATUS:
	//				/* a status event occured */
	//				handle_ctrl_status(wiimotes[i]);
	//				break;

	//			case WIIUSE_DISCONNECT:
	//				/* the wiimote disconnected */
	//				//handle_disconnect(wiimotes[i]);
	//				break;

	//			default:
	//				break;
	//		}
	//	}
	//}
	return true;
}

void Input::Wiimote::release(){
	InputDevice::release();
}

const int Input::Wiimote::WII_MOVE_HANDS_OUTWARD = 1 << 12;
const int Input::Wiimote::WII_MOVE_HANDS_INWARD = 2 << 12;
const int Input::Wiimote::WII_RIGHT_HAND_UP = 3 << 12;
const int Input::Wiimote::WII_BOTH_HANDS_DOWN = 4 << 12;
const int Input::Wiimote::WII_RIGHT_HAND_RIGHT = 5 << 12;
const int Input::Wiimote::WII_RIGHT_HAND_LEFT = 6 << 12;
const int Input::Wiimote::WII_RIGHT_HAND_POINT_UP = 7 << 12;
const int Input::Wiimote::WII_DRUM = 8 << 12;
const int Input::Wiimote::WII_A_BUTTON = WIIMOTE_BUTTON_A;
const int Input::Wiimote::WII_B_BUTTON = WIIMOTE_BUTTON_B;
const int Input::Wiimote::WII_PLUS_BUTTON = WIIMOTE_BUTTON_PLUS;
const int Input::Wiimote::WII_MINUS_BUTTON = WIIMOTE_BUTTON_MINUS;
const int Input::Wiimote::WII_Z_BUTTON = NUNCHUK_BUTTON_Z;
const int Input::Wiimote::WII_C_BUTTON = NUNCHUK_BUTTON_C;
const int Input::Wiimote::WII_1_BUTTON = WIIMOTE_BUTTON_ONE;
const int Input::Wiimote::WII_2_BUTTON = WIIMOTE_BUTTON_TWO;
const int Input::Wiimote::WII_HOME_BUTTON = WIIMOTE_BUTTON_HOME;
const int Input::Wiimote::WII_RIGHT_BUTTON = WIIMOTE_BUTTON_RIGHT;
const int Input::Wiimote::WII_LEFT_BUTTON = WIIMOTE_BUTTON_LEFT;
const int Input::Wiimote::WII_UP_BUTTON = WIIMOTE_BUTTON_UP;
const int Input::Wiimote::WII_DOWN_BUTTON = WIIMOTE_BUTTON_DOWN;


bool Input::Wiimote::buttonStatus(int code) const{
	if (!found){
		return false;
	}
	InputDevice::buttonStatus(code);
	if (code == Input::Wiimote::WII_MOVE_HANDS_OUTWARD){
		return hands_out;
	}
	else if (code == Input::Wiimote::WII_MOVE_HANDS_INWARD){
		return hands_in;
	}
	else if (code == Input::Wiimote::WII_RIGHT_HAND_UP){
		return right_hand_up;
	}
	else if (code == Input::Wiimote::WII_RIGHT_HAND_LEFT){
		return right_hand_left;
	}
	else if (code == Input::Wiimote::WII_RIGHT_HAND_RIGHT){
		return right_hand_right;
	}
	else if (code == Input::Wiimote::WII_BOTH_HANDS_DOWN){
		return both_hands_down;
	}

	return (wiimotes[0]->btns & code) == code;
	

}
bool Input::Wiimote::operator()(int code) const{
	InputDevice::buttonStatus(code);
	return buttonStatus(code);

}
void Input::Wiimote::toggleRumble(){
	if (!found){
		return;
	}
	wiiuse_toggle_rumble(wiimotes[0]);
}

void Input::Wiimote::handle_ctrl_status(struct wiimote_t* wm){


}

float Input::Wiimote::joystickAngle(){
	if (!found){
		return 0.0;
	}
	return angle;
}


float Input::Wiimote::joystickMagnitude(){
	if (!found){
		return 0.0;
	}
	return magnitude;
}
void Input::Wiimote::handle_event(struct wiimote_t* wm){
	


	if (wm->exp.type == EXP_NUNCHUK) {
		/* nunchuk */
		struct nunchuk_t* nc = (nunchuk_t*)&wm->exp.nunchuk;


		magnitude = nc->js.mag;
		angle = nc->js.ang;
	}

	/*
	 *	If IR tracking is enabled then print the coordinates
	 *	on the virtual screen that the wiimote is pointing to.
	 *
	 *	Also make sure that we see at least 1 dot.
	 */
	if (WIIUSE_USING_IR(wm)) {
		int i = 0;

		if (wm->ir.x != 0 && wm->ir.y != 0){
			this->ir_dx += wm->ir.x - ir_x; 
			this->ir_dy += wm->ir.y - ir_y;
			this->ir_x = wm->ir.x;
			this->ir_y = wm->ir.y;
		}
	}
}


float Input::Wiimote::getBatteryLevel(){
	if (!found){
		return 0.0;
	}
	wiiuse_status(wiimotes[0]);
	return battery_level;
}

int* Input::Wiimote::whichLEDs(){
	if (!found){
		return 0;
	}
	wiiuse_status(wiimotes[0]);
	return LEDs;
}

void Input::Wiimote::resync(){
	if (!found){
		return;
	}
	wiiuse_resync(wiimotes[0]);

}
bool Input::Wiimote::hasAttachement(){
	if (!found){
		return false;
	}
	wiiuse_status(wiimotes[0]);
	return attachment;
}
Input::Wiimote::Wiimote() : InputDevice(), battery_level(0), attachment(0), moteID(0), wiimotes(NULL), found(false){
	LEDs[0] = LEDs[1] = LEDs[2] = LEDs[3] = false;

	magnitude = 0;
	angle = 0;
	ir_x = 0;
	ir_y = 0;
	ir_dx = 0;
	ir_dy = 0;

}
Input::Wiimote::~Wiimote(){
	wiiuse_cleanup(wiimotes, 1);

}
bool Input::Keyboard::init(Input::InputDeviceInit &deviceInit){
	//Call the base class version first.
	InputDevice::init(deviceInit);

	Input::KeyboardInit *keyboardInit = (KeyboardInit*)&deviceInit;

	return true;

}

void Input::Keyboard::release(){
	InputDevice::release();
}

bool Input::Keyboard::read(){
	InputDevice::read();

	if (receiver == NULL){
		return true;
	}

	return true;
}

bool Input::Keyboard::buttonStatus(int code) const{
	InputDevice::buttonStatus(code);
	
	if (receiver->keyDown(code))
		return true;
	else
		return false;
}
bool Input::Keyboard::operator()(int code) const{
	InputDevice::buttonStatus(code);
	return buttonStatus(code);
}
const int Input::Keyboard::KEY_A = irr::KEY_KEY_A;
const int Input::Keyboard::KEY_B = irr::KEY_KEY_B;
const int Input::Keyboard::KEY_C = irr::KEY_KEY_C;
const int Input::Keyboard::KEY_D = irr::KEY_KEY_D;
const int Input::Keyboard::KEY_E = irr::KEY_KEY_E;
const int Input::Keyboard::KEY_F = irr::KEY_KEY_F;
const int Input::Keyboard::KEY_G = irr::KEY_KEY_G;
const int Input::Keyboard::KEY_H = irr::KEY_KEY_H;
const int Input::Keyboard::KEY_I = irr::KEY_KEY_I;
const int Input::Keyboard::KEY_J = irr::KEY_KEY_J;
const int Input::Keyboard::KEY_K = irr::KEY_KEY_K;
const int Input::Keyboard::KEY_L = irr::KEY_KEY_L;
const int Input::Keyboard::KEY_M = irr::KEY_KEY_M;
const int Input::Keyboard::KEY_N = irr::KEY_KEY_N;
const int Input::Keyboard::KEY_ESC = irr::KEY_ESCAPE;
const int Input::Keyboard::KEY_O = irr::KEY_KEY_O;
const int Input::Keyboard::KEY_P = irr::KEY_KEY_P;
const int Input::Keyboard::KEY_Q = irr::KEY_KEY_Q;
const int Input::Keyboard::KEY_R = irr::KEY_KEY_R;
const int Input::Keyboard::KEY_S = irr::KEY_KEY_S;
const int Input::Keyboard::KEY_T = irr::KEY_KEY_T;
const int Input::Keyboard::KEY_U = irr::KEY_KEY_U;
const int Input::Keyboard::KEY_V = irr::KEY_KEY_V;
const int Input::Keyboard::KEY_W = irr::KEY_KEY_W;
const int Input::Keyboard::KEY_X = irr::KEY_KEY_X;
const int Input::Keyboard::KEY_Y = irr::KEY_KEY_Y;
const int Input::Keyboard::KEY_Z = irr::KEY_KEY_Z;
const int Input::Keyboard::KEY_1 = irr::KEY_KEY_1;
const int Input::Keyboard::KEY_2 = irr::KEY_KEY_2;
const int Input::Keyboard::KEY_3 = irr::KEY_KEY_3;
const int Input::Keyboard::KEY_4 = irr::KEY_KEY_4;
const int Input::Keyboard::KEY_5 = irr::KEY_KEY_5;
const int Input::Keyboard::KEY_6 = irr::KEY_KEY_6;
const int Input::Keyboard::KEY_7 = irr::KEY_KEY_7;
const int Input::Keyboard::KEY_8 = irr::KEY_KEY_8;
const int Input::Keyboard::KEY_9 = irr::KEY_KEY_9;
const int Input::Keyboard::KEY_0 = irr::KEY_KEY_0;
const int Input::Keyboard::KEY_LSHIFT = irr::KEY_LSHIFT;
const int Input::Keyboard::KEY_RSHIFT = irr::KEY_RSHIFT;
const int Input::Keyboard::KEY_SPACE = irr::KEY_SPACE;

char Input::Keyboard::keyToChar(int key){
	char output = ' ';

	switch(key){
		case Input::Keyboard::KEY_A:
			output = 'a';
			break;
		case Input::Keyboard::KEY_B:
			output = 'b';
			break;
		case Input::Keyboard::KEY_C:
			output = 'c';
			break;
		case Input::Keyboard::KEY_D:
			output = 'd';
			break;
		case Input::Keyboard::KEY_E:
			output = 'e';
			break;
		case Input::Keyboard::KEY_F:
			output = 'f';
			break;
		case Input::Keyboard::KEY_G:
			output = 'g';
			break;
		case Input::Keyboard::KEY_H:
			output = 'h';
			break;
		case Input::Keyboard::KEY_I:
			output = 'i';
			break;
		case Input::Keyboard::KEY_J:
			output = 'j';
			break;
		case Input::Keyboard::KEY_K:
			output = 'k';
			break;
		case Input::Keyboard::KEY_L:
			output = 'l';
			break;
		case Input::Keyboard::KEY_M:
			output = 'm';
			break;
		case Input::Keyboard::KEY_N:
			output = 'n';
			break;
		case Input::Keyboard::KEY_O:
			output = 'o';
			break;
		case Input::Keyboard::KEY_P:
			output = 'p';
			break;
		case Input::Keyboard::KEY_Q:
			output = 'q';
			break;
		case Input::Keyboard::KEY_R:
			output = 'r';
			break;
		case Input::Keyboard::KEY_S:
			output = 's';
			break;
		case Input::Keyboard::KEY_T:
			output = 't';
			break;
		case Input::Keyboard::KEY_U:
			output = 'u';
			break;
		case Input::Keyboard::KEY_V:
			output = 'v';
			break;
		case Input::Keyboard::KEY_W:
			output = 'w';
			break;
		case Input::Keyboard::KEY_X:
			output = 'x';
			break;
		case Input::Keyboard::KEY_Y:
			output = 'y';
			break;
		case Input::Keyboard::KEY_Z:
			output = 'z';
			break;

	};

	if (receiver->keyPressed((char)Input::Keyboard::KEY_LSHIFT) || receiver->keyPressed((char)Input::Keyboard::KEY_RSHIFT)){
		output = toupper(output);
	}
	if(output != ' ' || receiver->keyPressed(Input::Keyboard::KEY_SPACE)){
		return output;
	}


	switch(key){
		//Shift not being pressed.
		case Input::Keyboard::KEY_0:
			output = '0';
			break;
		case Input::Keyboard::KEY_1:
			output = '1';
			break;
		case Input::Keyboard::KEY_2:
			output = '2';
			break;
		case Input::Keyboard::KEY_3:
			output = '3';
			break;
		case Input::Keyboard::KEY_4:
			output = '4';
			break;
		case Input::Keyboard::KEY_5:
			output = '5';
			break;
		case Input::Keyboard::KEY_6:
			output = '6';
			break;
		case Input::Keyboard::KEY_7:
			output = '7';
			break;
		case Input::Keyboard::KEY_8:
			output = '8';
			break;
		case Input::Keyboard::KEY_9:
			output = '9';
			break;
	};
	

	return output;
}

std::string& Input::Keyboard::appendtoString(std::string &string, char c, bool backspace){
	if (!backspace){
		return string.append(&c);
	}
	else{
		if (string.length() > 0)
			return string.erase(string.length() - 1, 1);
	}
	return string;
}

Input::Keyboard::Keyboard() : InputDevice(){

}

Input::Keyboard::~Keyboard(){
}





Input::Action::Action(const std::string& sName) : m_ToggleTime(0), m_Amount(0){
	m_Name = sName;
	reset();
}
Input::Action::Action(const std::string& sName, int code, Input::InputDevice &device): m_ToggleTime(0), m_Amount(0){
	m_Name = sName;
	this->addCode(code, device);
}

void Input::Action::addCode(int code, Input::InputDevice &device){
	if (deviceCodeMap.find(device.getName()) != deviceCodeMap.end()){
		std::set<std::set<int> > codeList;
		std::set<int> codes;
		codes.insert(code);
		codeList.insert(codes);
		deviceCodeMap.insert(make_pair(device.getName(), codeList));
	}
	else{
		std::set<int> codes;
		codes.insert(code);
		deviceCodeMap[device.getName()].insert(codes);
	}
	reset();
}

void Input::Action::addCode(std::set<int>& codes, Input::InputDevice &device){
	if (deviceCodeMap.find(device.getName()) != deviceCodeMap.end()){
		std::set<std::set<int > > codeList;
		codeList.insert(codes);
		deviceCodeMap.insert(make_pair(device.getName(), codeList));
	}
	else{
		deviceCodeMap[device.getName()].insert(codes);
	}
	reset();
}

Input::Action::~Action(){
	reset();
}


int Input::Action::getAmount(){
	int retVal = m_Amount;
    if (retVal != 0) {
		if (m_State == Action::STATE_RELEASED) {
            m_Amount = 0;
        }
		else if (m_Behavior == Action::BEHAVIOR_DETECT_PRESS) {
			m_State = Action::STATE_WAITING_FOR_RELEASE;
        }
    }
    return retVal;
}

void Input::Action::tap(){
	press();
	release();
}

bool Input::Action::isPressed(){
	return(getAmount() != 0);
}

const std::string& Input::Action::getName() const{
	return m_Name;
}

void Input::Action::release(){
	if (m_Behavior == Input::Action::BEHAVIOR_DETECT_RELEASE){
		m_Amount += 1;
	}
	m_State = Input::Action::STATE_RELEASED;
}

void Input::Action::press(){
	press(1);
}

void Input::Action::press(const int amount){
	if (m_State != Action::STATE_WAITING_FOR_RELEASE) {
		if (m_Behavior != Input::Action::BEHAVIOR_DETECT_RELEASE){
			m_Amount += amount;
		}
		m_State = Action::STATE_PRESSED;
    }
}

void Input::Action::checkButtonStatus(){
	//Iterates over devices that this action uses
	std::map<std::string, std::set<std::set<int > > >::iterator deviceIter;
	//This flag says whether to release the button or not.
	bool pushedFlag = false;
	for (deviceIter = deviceCodeMap.begin(); deviceIter != deviceCodeMap.end(); deviceIter++){
		//Get current device to reduce string lookups
		Input::InputDevice& currentDevice = (InputManager::getSingleton().getDevice((*deviceIter).first));
		//Iterates over codes registered with this device for this action.
		std::set<std::set<int> >::iterator codeListIter, codeListIterEnd;
		codeListIterEnd = (*deviceIter).second.end();
		for (codeListIter = (*deviceIter).second.begin(); codeListIter != codeListIterEnd; codeListIter++){
			//Checks if the code on this device has been pressed.
			bool simultaneousPress = true;
			std::set<int>::iterator codeIter;
			
			for (codeIter = (*codeListIter).begin(); codeIter != (*codeListIter).end(); codeIter++){

				if (currentDevice.buttonStatus(*codeIter)){
					continue;
				}
				else{
					simultaneousPress = false;
					break;
				}
			}
			if(simultaneousPress){
				pushedFlag = true;
				if (m_Behavior == Input::Action::BEHAVIOR_DETECT_PRESS || m_Behavior == Input::Action::BEHAVIOR_DETECT_RELEASE){
					this->press();
				}
				else if (m_Behavior == Input::Action::BEHAVIOR_DETECT_TAP){
					this->tap();
				}

			}
		}

	}
	//Releases button if none of the registered codes have been pressed.
	if (!pushedFlag && m_State != Input::Action::STATE_RELEASED){
		this->release();
	}
}

const int Input::Action::BEHAVIOR_DETECT_PRESS =  1 << 0;
const int Input::Action::BEHAVIOR_DETECT_TAP = 1 << 1;
const int Input::Action::BEHAVIOR_DETECT_RELEASE = 1 << 2;

const int Input::Action::STATE_PRESSED = 1 << 0;
const int Input::Action::STATE_RELEASED = 1 << 1;
const int Input::Action::STATE_WAITING_FOR_RELEASE = 1 << 2;


Input::Action* InputManager::createAction(const std::string name, Input::InputDevice& device, int code, int behaviorFlag){
	m_ActionItr = m_ActionMap.find(name);
	if(m_ActionItr != m_ActionMap.end()){
		std::set<int> codes;
		codes.insert(code);
		m_ActionItr->second->addCode(codes, device);
		return m_ActionItr->second;
	}
	Input::Action* action = new Input::Action(name, code, device);
	action->m_Behavior = behaviorFlag;
	m_ActionMap.insert(make_pair(name, action));
	return action;
}

Input::Action* InputManager::createAction(const std::string name, int behaviorFlag){
	m_ActionItr = m_ActionMap.find(name);
	if(m_ActionItr != m_ActionMap.end()){
		return m_ActionItr->second;
	}
	Input::Action* action = new Input::Action(name);
	action->m_Behavior = behaviorFlag;
	m_ActionMap.insert(make_pair(name, action));
	return action;
}
Input::Action* InputManager::getAction(const std::string& name){
	//Find the Action in the map according to the given string
	m_ActionItr = m_ActionMap.find(name);

	if(m_ActionItr == m_ActionMap.end()){
		Input::Action* action = new Input::Action(name);
		action->m_Behavior = Input::Action::BEHAVIOR_DETECT_PRESS;
		m_ActionMap.insert(make_pair(name, action));
		return action;
	}

	return m_ActionItr->second;

}

bool InputManager::deleteAction( const std::string & sActionName){
	//Find the Action in the map according to the given string
	m_ActionItr = m_ActionMap.find(sActionName);
	//If the Action wasn't found, return false
	if(m_ActionItr == m_ActionMap.end())
		return false;

	delete m_ActionItr->second;
	m_ActionMap.erase(m_ActionItr);
	return true;
}

MastEventReceiver* InputManager::getEventReceiver(){
	return receiver;
}
bool InputManager::init(){
	receiver = new MastEventReceiver();
	receiver->init();


	if (!m_Wiimote.init(m_WiimoteInit))
		;//throw Input::InputManagerInitException();
	if (!m_Keyboard.init(m_KeyboardInit))
		;//throw Input::InputManagerInitException();
	if (!m_Mouse.init(m_MouseInit))
		;//throw Input::InputManagerInitException();
	Input::InputDevice* pMouse = &m_Mouse;
	Input::InputDevice* pKeyboard = &m_Keyboard;
	Input::InputDevice* pWiimote = &m_Wiimote;

	m_DeviceMap.insert(make_pair(m_Mouse.getName(), &m_Mouse));
	m_DeviceMap.insert(make_pair(m_Wiimote.getName(), &m_Wiimote));
	m_DeviceMap.insert(make_pair(m_Keyboard.getName(), &m_Keyboard));

	m_Mouse.receiver = receiver;
	m_Keyboard.receiver = receiver;
	
	//return success
	return(true);
}

void InputManager::resumePolling(){
	if (receiver){
		receiver->startEventProcess();
	}

	
}

void InputManager::stopPolling(){
	if (receiver){
		receiver->endEventProcess();
	}
}

Input::InputDevice& InputManager::getDevice(const std::string& name){
	//Find the Device in the map according to the given string
	Input::StrDeviceMap::iterator iter = m_DeviceMap.find(name);
	//If the Device wasn't found, return false
	//if(iter == m_DeviceMap.end()) //Don't know how to return a NULL value
		//return NULL;

	return *((*iter).second);

}
void InputManager::shutdown(){
	//Reset all actions, just cause
	resetAllActions();
	//Delete and remove all actions from the map.
	for(m_ActionItr = m_ActionMap.begin(); m_ActionItr != m_ActionMap.end(); ++m_ActionItr)
	{
		delete m_ActionItr->second;
	}
	m_ActionMap.clear();
	//have the keyboard and mouse shut themselves down.
	m_Keyboard.release();
	m_Wiimote.release();
	m_Mouse.release();
	//delete irrilicht input receiver
	if (receiver){
		delete receiver;
	}

}


bool InputManager::getInput(){

	m_Wiimote.read();
	m_Keyboard.read();
	m_Mouse.read();
	for(m_ActionItr = m_ActionMap.begin(); m_ActionItr != m_ActionMap.end(); ++m_ActionItr)
	{
		m_ActionItr->second->checkButtonStatus();
	}

	return true;

}

void InputManager::resetAllActions(){
	for(m_ActionItr = m_ActionMap.begin(); m_ActionItr != m_ActionMap.end(); ++m_ActionItr)
	{
		m_ActionItr->second->reset();
	}
}

InputManager::InputManager() : receiver(NULL){

}

InputManager::~InputManager(){
	this->shutdown();
}


std::ostream& operator << (std::ostream& os, const Input::InputDevice& device){
	return os << device.getName();
}

std::ostream& operator << (std::ostream& os, const Input::Action& action){
	return os << action.getName();
}
