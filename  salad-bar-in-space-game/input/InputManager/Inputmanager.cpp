#include "Common.h"
#include ".\inputmanager.h"
//Interface methods don't need an implementation, yet.
bool Input::InputDevice::init(Input::InputDeviceInit &deviceInit){
	this->m_name = deviceInit.m_name;
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

Input::InputDevice::InputDevice() : m_lpdi(NULL){
}

Input::InputDevice::~InputDevice(){
}
//End interface methods

bool Input::Keyboard::init(Input::InputDeviceInit &deviceInit){
	//Call the base class version first.
	InputDevice::init(deviceInit);

	Input::KeyboardInit *keyboardInit = (KeyboardInit*)&deviceInit;
	this->release();

	HRESULT hr;

	//Default coopflags are foreground nonexclusive, this way we can also use DISCL_NOWINKEY to disable the windows key.
	if (keyboardInit->bDisableWinKey){
		keyboardInit->dwCoopFlags |= DISCL_NOWINKEY;
	}
	if (FAILED(hr = this->m_lpdi->CreateDevice(GUID_SysKeyboard, &m_lpdikey, NULL))){
		return false;
	}
	//Set the data format to 256 array of BYTE.
	if (FAILED(hr = m_lpdikey->SetDataFormat(&c_dfDIKeyboard))){
		return false;
	}

	//Set the cooperativity level.
	hr = m_lpdikey->SetCooperativeLevel(keyboardInit->hwnd, keyboardInit->dwCoopFlags);
	//If failed set to the safest coop level.
	if (hr == DIERR_UNSUPPORTED){
		hr = m_lpdikey->SetCooperativeLevel(keyboardInit->hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	}
	//If still failed return false.
	if (FAILED(hr)){
		return false;
	}

	//Acquire the keyboard
	m_lpdikey->Acquire();
	m_Acquired = true;
	m_Active = true;

	return true;

}

void Input::Keyboard::release(){
	InputDevice::release();
	if (m_lpdikey){
		m_lpdikey->Unacquire();
	}
	m_Active = false;
	m_Acquired = false;

	//Release any DirectInput objects.
	if (m_lpdikey){
		m_lpdikey->Release();
	}

	m_lpdikey = NULL;
}

bool Input::Keyboard::read(){
	InputDevice::read();
	HRESULT hr;

	if (m_lpdikey == NULL){
		return true;
	}

	//Get keyboard state
	ZeroMemory(m_Key, sizeof(m_Key));
	hr = m_lpdikey->GetDeviceState(sizeof(m_Key), m_Key);

	if (FAILED(hr)){
		//The stream might have been interrupted.  Re-acquire.
		hr = m_lpdikey->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = m_lpdikey->Acquire();

		if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED)
			m_Acquired = false;

		//problems, return true and try again later.
		return true;
	}

	return true;
}

bool Input::Keyboard::buttonStatus(int code) const{
	InputDevice::buttonStatus(code);
	if (m_Key[code] & 0x80)
		return true;
	else
		return false;
}

char Input::Keyboard::keyToChar(int key){
	char output = ' ';

	switch(key){
		case DIK_A:
			output = 'a';
			break;
		case DIK_B:
			output = 'b';
			break;
		case DIK_C:
			output = 'c';
			break;
		case DIK_D:
			output = 'd';
			break;
		case DIK_E:
			output = 'e';
			break;
		case DIK_F:
			output = 'f';
			break;
		case DIK_G:
			output = 'g';
			break;
		case DIK_H:
			output = 'h';
			break;
		case DIK_I:
			output = 'i';
			break;
		case DIK_J:
			output = 'j';
			break;
		case DIK_K:
			output = 'k';
			break;
		case DIK_L:
			output = 'l';
			break;
		case DIK_M:
			output = 'm';
			break;
		case DIK_N:
			output = 'n';
			break;
		case DIK_O:
			output = 'o';
			break;
		case DIK_P:
			output = 'p';
			break;
		case DIK_Q:
			output = 'q';
			break;
		case DIK_R:
			output = 'r';
			break;
		case DIK_S:
			output = 's';
			break;
		case DIK_T:
			output = 't';
			break;
		case DIK_U:
			output = 'u';
			break;
		case DIK_V:
			output = 'v';
			break;
		case DIK_W:
			output = 'w';
			break;
		case DIK_X:
			output = 'x';
			break;
		case DIK_Y:
			output = 'y';
			break;
		case DIK_Z:
			output = 'z';
			break;
	};

	if (m_Key[DIK_LSHIFT] & 0x80 || m_Key[DIK_RSHIFT] & 0x80){
		output = toupper(output);
	}
	if(output != ' ' || key == DIK_SPACE){
		return output;
	}

	if (m_Key[DIK_LSHIFT] & 0x80 || m_Key[DIK_RSHIFT] & 0x80){
		//Shift is being pressed.
		switch(key){
			case DIK_GRAVE:
				return '~';
				break;
			case DIK_1:
				return '!';
				break;
			case DIK_2:
				return '@';
				break;
			case DIK_3:
				return '#';
				break;
			case DIK_4:
				return '$';
				break;
			case DIK_5:
				return '%';
				break;
			case DIK_6:
				return '^';
				break;
			case DIK_7:
				return '&';
				break;
			case DIK_8:
				return '*';
				break;
			case DIK_9:
				return '(';
				break;
			case DIK_0:
				return ')';
				break;
			case DIK_MINUS:
				return '_';
				break;
			case DIK_EQUALS:
				return '+';
				break;
			case DIK_BACKSLASH:
				return '|';
				break;
			case DIK_LBRACKET:
				return '{';
				break;
			case DIK_RBRACKET:
				return '}';
				break;
			case DIK_SEMICOLON:
				return ':';
				break;
			case DIK_APOSTROPHE:
				return '\"';
				break;
			case DIK_COMMA:
				return '<';
				break;
			case DIK_PERIOD:
				return '>';
				break;
			case DIK_SLASH:
				return'?';
				break;
		};
	}
	else{
		switch(key){
			//Shift not being pressed.
			case DIK_GRAVE:
				return '`';
				break;
			case DIK_1:
				return '1';
				break;
			case DIK_2:
				return '2';
				break;
			case DIK_3:
				return '3';
				break;
			case DIK_4:
				return '4';
				break;
			case DIK_5:
				return '5';
				break;
			case DIK_6:
				return '6';
				break;
			case DIK_7:
				return '7';
				break;
			case DIK_8:
				return '8';
				break;
			case DIK_9:
				return '9';
				break;
			case DIK_0:
				return '0';
				break;
			case DIK_MINUS:
				return '-';
				break;
			case DIK_EQUALS:
				return '=';
				break;
			case DIK_BACKSLASH:
				return '\\';
				break;
			case DIK_LBRACKET:
				return '[';
				break;
			case DIK_RBRACKET:
				return ']';
				break;
			case DIK_SEMICOLON:
				return ';';
				break;
			case DIK_APOSTROPHE:
				return '\'';
				break;
			case DIK_COMMA:
				return ',';
				break;
			case DIK_PERIOD:
				return '.';
				break;
			case DIK_SLASH:
				return '/';
				break;
		};
	}

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

Input::Keyboard::Keyboard() : InputDevice(), m_Acquired(false), m_Active(false), m_lpdikey(false){
	memset(&m_Key,0,sizeof(BYTE) * 256);
}

Input::Keyboard::~Keyboard(){
}

bool Input::Mouse::buttonStatus(int code) const{
	InputDevice::buttonStatus(code);
	if (m_Button[code] & 0x80)
		return true;
	else
		return false;
}
bool Input::Mouse::init(Input::InputDeviceInit &deviceInit){
	//Call the base class version first.
	InputDevice::init(deviceInit);

	//Release the mouse
	this->release();
	// Obtain an interface to the system mouse device.
	Input::MouseInit* mouseInit = (MouseInit*)&deviceInit;
	if (FAILED(m_lpdi->CreateDevice(GUID_SysMouse, &m_lpdimouse, NULL)))
		return false;
	//Set data format to mouse format - predefined by directx DIMOUSESTATE2
	if (FAILED(m_lpdimouse->SetDataFormat(&c_dfDIMouse2)))
		return false;
	HRESULT hr;
	// Set cooperative level
	hr = m_lpdimouse->SetCooperativeLevel(mouseInit->hwnd, mouseInit->dwCoopFlags);
	// if failed set to different cooperative level
	if (hr == DIERR_UNSUPPORTED){
		hr = m_lpdimouse->SetCooperativeLevel(mouseInit->hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}
	// if still failed return false
	if (FAILED(hr)){
		return false;
	}
	// aquire mouse
	if (m_lpdimouse->Acquire() != DI_OK)
		return false;

	m_Acquired = true;
	m_Active = true;

	return true;
}

void Input::Mouse::release(){
	InputDevice::release();
	if (m_lpdimouse)
		m_lpdimouse->Unacquire();

	m_Acquired = false;
	m_Active = false;
	if (m_lpdimouse){
		m_lpdimouse->Release();
	}

	m_lpdimouse = NULL;
}

int Input::Mouse::getSensitivity() const{
	return m_MouseSpeed;
}

void Input::Mouse::setSensitivity(int factor){
	m_MouseSpeed = factor;
}

void Input::Mouse::getRelativePosition(LONG *x, LONG *y, LONG *z){
	*x = m_dx;
	*y = m_dy;
	*z = m_dz;
}


void Input::Mouse::setLimits(long x1, long y1, long x2, long y2){
}

bool Input::Mouse::read(){
	InputDevice::read();
	HRESULT hr;
	DIMOUSESTATE2 dims2; //DirectInput mouse state structure

	if (m_lpdimouse == NULL)
		return true;

	ZeroMemory( &dims2, sizeof(dims2) );
	//Get the mouse's state
	hr = m_lpdimouse->GetDeviceState(sizeof(DIMOUSESTATE2), &dims2);
	if (FAILED(hr)){
		//input stream may have been interrupted. need to re-acquire.
		hr = m_lpdimouse->Acquire();
		while(hr == DIERR_INPUTLOST)
			hr = m_lpdimouse->Acquire();

		if(hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED)
			m_Acquired = false;

		//app may be minimized or in the process of switching, try again later.
		return true;
	}

	m_Acquired = true;

	//retrieve mouse information and store in Input::Mouse variables
	m_dx = dims2.lX * m_MouseSpeed;
	m_dy = dims2.lY * m_MouseSpeed;
	m_dz = dims2.lZ * m_MouseSpeed;
	m_Button[0] = dims2.rgbButtons[0];
	m_Button[1] = dims2.rgbButtons[1];
	m_Button[2] = dims2.rgbButtons[2];
	m_Button[3] = dims2.rgbButtons[3];
	m_Button[4] = dims2.rgbButtons[4];
	m_Button[5] = dims2.rgbButtons[5];
	m_Button[6] = dims2.rgbButtons[6];
	m_Button[7] = dims2.rgbButtons[7];

	m_MouseAbsX += m_dx;
    m_MouseAbsY += m_dy;

	return true;
}

Input::Mouse::Mouse() : InputDevice(), m_lpdimouse(NULL), m_Active(false), m_Acquired(false){

	m_dx = 0;
	m_dy = 0;
	m_dz = 0;
	m_MouseSpeed = 2;
	m_MouseAbsX = 0;
	m_MouseAbsY = 0;

	memset(&m_Button,0,sizeof(BYTE) * 8);
}

Input::Mouse::~Mouse(){
}


Input::Action::Action(const std::string& sName) : m_Name(sName), m_ToggleTime(0), m_Amount(0){
	reset();
}
Input::Action::Action(const std::string& sName, int code, Input::InputDevice &device): m_Name(sName), m_ToggleTime(0), m_Amount(0){
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
            m_Amount = 0;
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

bool InputManager::init(HINSTANCE main_instance, HWND hwnd){
	//Create main DirectInput object
	if (FAILED(DirectInput8Create(main_instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_lpdi,NULL)))
		return(false);
	//Give the devices the pointer the DirectInput object just created, InputManager is a friend class of InputDevice.
	m_Mouse.m_lpdi = m_lpdi;
	m_Keyboard.m_lpdi = m_lpdi;
	
	this->m_MouseInit.hwnd = hwnd;
	this->m_KeyboardInit.hwnd = hwnd;

	//Init keyboard and mouse
	if (!m_Mouse.init(m_MouseInit))
		return(false);
	if (!m_Keyboard.init(m_KeyboardInit))
		return(false);
	Input::InputDevice* pMouse = &m_Mouse;
	Input::InputDevice* pKeyboard = &m_Keyboard;
	m_DeviceMap.insert(make_pair(m_Mouse.getName(), pMouse));
	m_DeviceMap.insert(make_pair(m_Keyboard.getName(), pKeyboard));
	//return success
	return(true);
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
	//have the keyboard and mouse shut themselves down.
	m_Keyboard.release();
	m_Mouse.release();
	//Release directinput object
	if (m_lpdi)
		m_lpdi->Release();

	m_lpdi = NULL;
}


bool InputManager::getInput(){

	m_Mouse.read();
	m_Keyboard.read();

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

InputManager::InputManager() : m_lpdi(NULL){

}

InputManager::~InputManager(){
	this->shutdown();
}

int main(){
	return 0;
}

std::ostream& operator << (std::ostream& os, const Input::InputDevice& device){
	return os << device.getName();
}

std::ostream& operator << (std::ostream& os, const Input::Action& action){
	return os << action.getName();
}
