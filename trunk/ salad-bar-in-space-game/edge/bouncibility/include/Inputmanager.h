

#pragma once
#include "singleton.h"
#include "wiiuse.h"
#include "irrlicht.h"

class MastEventReceiver;
class InputManager;
//!namespace containing all Input related structures used by the InputManager.
namespace Input{


	//! InputDeviceInit should be inherited from if a new device needs to be added, should contain initialiation information.
	struct InputDeviceInit{
		std::string m_name;	//!< Name of the device
	};
	//! Default Irrilicht keyboard provided.
	struct KeyboardInit : InputDeviceInit{
		KeyboardInit(){	
			m_name = "Irrlicht Keyboard";
		}
	};

	struct WiimoteInit : InputDeviceInit{

		WiimoteInit(){
			m_name = "Wii Mote";
		}
	};
	//!InputDevice is the base class for all devices added to the InputManager.
	class InputDevice
	{
		friend class Action;
		friend class ::InputManager;
	protected:

		std::string m_name; //!< The name of the device.

		MastEventReceiver* receiver; //!< A pointer to the Irrilicth input receiver object, initialized by InputManager.

		virtual void release() = 0; //!< release() should be overridden by derived class, it is called by InputManager.
		
		//! init() should be overridden by the derived class, it is called in registerDevice() in the InputManager.
		/*! InputManager passes init() the InputDeviceInit structure that the device was registered with, this structure should
		* contain all the needed initialization information.
		* @param deviceInit structure containing initialization information.
		*/
		virtual bool init(InputDeviceInit &deviceInit) = 0;

		virtual bool read() = 0; //!< read() is called for all devices when getInput() is called in the InputManager. Only returns false when fatal error occurs(hardly ever)

		//! buttonStatus() returns if the button represented by the sent code is in a pressed or releaseed state.
		/*! The function will return true if the button represented by the code was pressed at the time of the last input poll.
		* the code needs to be static_cast<> to the type that was passed in.  The writer of the derived class should have this information.
		* @param code key being polled.
		*/
		virtual bool buttonStatus(int code) const = 0;


	public:
		InputDevice();
		
		virtual ~InputDevice();

		friend std::ostream& operator << (std::ostream& os, const InputDevice& device);


		const std::string& getName() const{
			return m_name;
		}

	};

	class Wiimote : public InputDevice{
	public:
		static const int WII_MOVE_HANDS_OUTWARD;
		static const int WII_MOVE_HANDS_INWARD;
		static const int WII_RIGHT_HAND_UP;
		static const int WII_BOTH_HANDS_DOWN;
		static const int WII_RIGHT_HAND_RIGHT;
		static const int WII_RIGHT_HAND_LEFT;
		static const int WII_RIGHT_HAND_POINT_UP;
		static const int WII_DRUM;
		static const int WII_A_BUTTON;
		static const int WII_B_BUTTON;
		static const int WII_PLUS_BUTTON;
		static const int WII_MINUS_BUTTON;
		static const int WII_Z_BUTTON;
		static const int WII_C_BUTTON;
		static const int WII_1_BUTTON;
		static const int WII_2_BUTTON;
		static const int WII_HOME_BUTTON;



		friend class ::InputManager;
	private:
	
	protected:
		bool found;
		int LEDs[4];
		float battery_level;
		bool attachment;
		int moteID;
		wiimote** wiimotes;
		bool hands_out;
		bool hands_in;
		bool right_hand_up;
		bool both_hands_down;
		bool right_hand_right;
		bool right_hand_left;

		void handle_event(struct wiimote_t* wm);

		void handle_ctrl_status(struct wiimote_t* wm);

		//void handle_disconnect(wiimote* wm);

		bool init(InputDeviceInit &deviceInit);

		bool read();

		void release();

		bool buttonStatus(int code) const;

		bool operator()(int code) const;


	public:

		void toggleRumble();

		float getBatteryLevel();

		bool hasAttachement();

		int* whichLEDs();

		float joystickAngle();

		float joystickMagnitude();

		void toggleLED(int which);
		Wiimote();

		virtual ~Wiimote();
	};
	//! Keyboard is a specialization of the InputDevice provided by the InputManager by default.
	/*! This implementation of Keyboard is using the default DirectInput keyboard.  Keys are stored in an array of 256 BYTES.  The class
	* also provides the ability to covert input codes into a char if printing of typed keys was needed.  In addition to keyToChar(), appendToString() 
	* can be used to append each key to a previous string.
	*/
	class Keyboard : public InputDevice{
	public:
		static const int KEY_A;
		static const int KEY_B;
		static const int KEY_C;
		static const int KEY_D;
		static const int KEY_E;
		static const int KEY_F;
		static const int KEY_G;
		static const int KEY_H;
		static const int KEY_I;
		static const int KEY_J;
		static const int KEY_K;
		static const int KEY_L;
		static const int KEY_M;
		static const int KEY_N;
		static const int KEY_O;
		static const int KEY_P;
		static const int KEY_Q;
		static const int KEY_R;
		static const int KEY_S;
		static const int KEY_T;
		static const int KEY_U;
		static const int KEY_V;
		static const int KEY_W;
		static const int KEY_X;
		static const int KEY_Y;
		static const int KEY_Z;
		static const int KEY_LSHIFT;
		static const int KEY_RSHIFT;
		static const int KEY_SPACE;
		static const int KEY_1;
		static const int KEY_2;
		static const int KEY_3;
		static const int KEY_4;
		static const int KEY_5;
		static const int KEY_6;
		static const int KEY_7;
		static const int KEY_8;
		static const int KEY_9;
		static const int KEY_0;

		friend class ::InputManager;
	protected:
		bool init(InputDeviceInit &deviceInit);

		bool read();

		void release();
		//! Converts given Irrilicht keycode to a char.
		/*! Uses the directx keycode constants to find a corresponding char and returns it.
		* @param key The irrilicht key constant.
		* @return char value represented by the constant.
		*/
		char keyToChar(int key);
		//! Appends the given char to the given string and returns a new string with the result.
		/*! Adds a char to the given string and returns a new string object as the result.  If backspace is pressed then
		* it removes a char from the end of the string.  This is used for saving keypresses into a string that is to be displayed on something like a console.
		* @param string String to append char to.
		* @param c char to append to string.
		* @param backspace bool indicating whether backspace is held down or not.
		* @return A new string object with the result.
		*/
		std::string& appendtoString(std::string& string, char c, bool backspace);
		
		bool buttonStatus(int code) const;

		bool operator()(int code) const;
	public:
		Keyboard();

		virtual ~Keyboard();



	};


	//! Action maps a button to a behavior.
	/*! The Action class encapsulates a behavior.  An action is created with the inputmanager.  When creating an action an InputDevice must be specified,
	* as well as a code representing the key that triggers this Action.  An Action can have two types of behavior, pressing and tapping.  
	* The Action methods are thread safe.
	*/
	class Action{
		friend class InputManager;
	public:
		static const int BEHAVIOR_DETECT_PRESS; //!< When a key is pressed, it will stay pressed until the key has been released.
		static const int BEHAVIOR_DETECT_TAP; //!< When a key is pressed, it is released immediately.  Tapped buttons cannot be held down.
		static const int BEHAVIOR_DETECT_RELEASE; //!<When a key is released after being pressed, the action is triggered.
	protected:
		static const int STATE_RELEASED; //!< The button this Action represents is released.
		static const int STATE_PRESSED; //!< The button this Action represents is pressed.
		static const int STATE_WAITING_FOR_RELEASE; //!< The button this Action represents is waiting to be released.


		std::string m_Name; //!< The name of the Action.
		int m_Behavior; //!< A behavior flag.
		int m_Amount; //!< The amount this Action has been triggered.
		int m_State; //!< The current state of the Action.
		long m_ToggleTime; //!<Timeout before tapped button can be pressed again.
		std::map<std::string, std::set<std::set<int> > > deviceCodeMap;
		
	public:
		//! Resets the state of the Action to default.
		virtual  void reset() { 
			m_State = STATE_RELEASED;
			m_Amount = 0;
		}
		void addCode(int code, Input::InputDevice& device); //!< Adds an additional key or mouse code to trigger this action.

		void addCode(std::set<int>& codes, Input::InputDevice &device);

		int getAmount(); //!< Returns the amount the Action has been triggered.

		bool isPressed(); //!< returns true if the Action is currently pressed, false otherwise.

		void release(); //!< Releases the button.

		void press(); //!< Presses the button.

		void press(const int amount);  //!< Presses the button the specified amount of times.

		void setToggleTime(long timeMill); //!<Sets the time a BEHAVIOR_DETECT_TAP button should wait before it can be pressed again.

		long getToggleTime() const //!<Gets the time a BEHAVIOR_DETECT_TAP button should wait before it can be pressed again.
		{
			return m_ToggleTime;
		}
		//WaitForSingleObject( hInputMutex, INFINITE );
		void tap(); //!< Taps the button. (Presses and Releases the button)

		const std::string& getName() const; //!< returns the name given to this Action.

		friend std::ostream& operator << (std::ostream& os, const Action& action);

	protected:
		explicit Action(const std::string& sName);

		explicit Action(const std::string& sName, int code, InputDevice& device);

		Action(const Action& rhs);

		Action& operator=(const Action& rhs);

		~Action();
		//! Used by InputManager to check if this Action's button was pressed.
		void checkButtonStatus();
	};

	typedef std::map<std::string, Action*> StrActionMap; 

	typedef std::map<std::string, InputDevice*> StrDeviceMap;

	//end namespace Input
};
//! InputManager encapsulates all workings of the InputSystem.
/*! Currently the InputManager only uses DirectInput.  In the future the Input API used might be decoupled from the InputManager.  But at the present time,
* there are very few other options besides DirectInput.
*/
class InputManager :
	public CSingleton<InputManager>{
private:

	Input::Keyboard m_Keyboard; //!< provided Keyboard implementation.

	Input::Wiimote m_Wiimote; //!< provided Wiimote implementation
	//Create initialization structures
	Input::KeyboardInit m_KeyboardInit;
	Input::WiimoteInit m_WiimoteInit;

	Input::StrActionMap m_ActionMap; //!< Maps strings to Actions.

	Input::StrActionMap::iterator m_ActionItr; //!< An iterator for the string action map.

	Input::StrDeviceMap m_DeviceMap; //!< Maps strings to devices.

	MastEventReceiver* receiver;

	InputManager();
	~InputManager();
	InputManager(const InputManager& rhs);
	InputManager& operator=(const InputManager& rhs);
public:
	friend CSingleton<InputManager>;

	bool init(); //!< Initialize the input system.

	void shutdown();  //!< shutdown any resources used by the InputManager.
	
	void stopPolling(); //!< Stops the irrilicht keyboard input system from polling the keyboard.  Should be called at beginning of main loop.

	void resumePolling(); //!< Starts the irrilicth keyboard input system to poll the keyboard.  Should be called at end of main loop.

	MastEventReceiver* getEventReceiver(); //!< Returns the eventReceiver that needs to be passed to the irrilcht createDevice method
	//! Creates an Action with the given parameters and returns it to the caller.
	/*! 
	* Actions are managed by the InputManager, deleteAction is provided if needed, but when the InputManager is shutdown
	* the Actions will be taken care of.
	*/
	Input::Action* createAction(const std::string name, Input::InputDevice& device, int code, int behaviorFlag = Input::Action::BEHAVIOR_DETECT_PRESS);
	
	//!Creates an empty action with no key or mouse codes assigned to it, addCode must be called on the action in order to make this action useful.
	Input::Action* createAction(const std::string name, int behaviorFlag = Input::Action::BEHAVIOR_DETECT_PRESS);

	//! Removes the Action with the given name from the InputManager.
	bool deleteAction(const std::string& name);
	//! Gets input from all the devices and notifies all Actions.  Currently unsure if this should be internal or called by the user of InputManager.
	bool getInput();
	//!Returns a reference to an Action in the actionmap that has already been created.
	Input::Action* getAction(const std::string& name);
	//!Returns a reference to the device in the devicemap with the given name.
	Input::InputDevice& getDevice(const std::string& name);

	//! Resets all the registered actions, useful for beginning a level with nothing pressed.
	void resetAllActions();


	Input::Keyboard& getKeyboard(){
		return m_Keyboard;
	}

	Input::Wiimote& getWiimote(){
		return m_Wiimote;
	}
	//! Used to retrieve the keyboard initialization structure to modify settings before calling InputManager::init().
	Input::KeyboardInit& getKeyboardInit(){
		return m_KeyboardInit;
	}

	Input::WiimoteInit& getWiimoteInit(){
		return m_WiimoteInit;
	}
};

// different behavior when for holding down and tapping buttons.

