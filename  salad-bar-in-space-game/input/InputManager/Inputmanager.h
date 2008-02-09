
//need to fix mouse actions.
//Might be useful to implement some callbacks in the actions.  Might also cause nasty resulting code that uses this class.
//Maybe later should add multiple keypresses of the same key in a certain time period to trigger action.

#pragma once
#include "singleton.h"

class InputManager;
//!namespace containing all Input related structures used by the InputManager.
namespace Input{

	//! InputDeviceInit should be inherited from if a new device needs to be added, should contain initialiation information.
	struct InputDeviceInit{
		std::string m_name;	//!< Name of the device
	};
	//! Default DirectX keyboard provided.
	struct KeyboardInit : InputDeviceInit{
		bool bDisableWinKey;
		DWORD dwCoopFlags;
		//!Initialized in InputManager::init() currently, but it would hurt for the user to do this.
		HWND hwnd;
		KeyboardInit(){
			bDisableWinKey = true;
			m_name = "DirectX Keyboard";
			hwnd = NULL;
			dwCoopFlags = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY;
		}
	};
	//! Default DirectX mouse provided.
	struct MouseInit : InputDeviceInit{
		DWORD dwCoopFlags;
		//! Initialized in InputManager::init() currently, but it would hurt for the user to do this.
		HWND  hwnd;

		MouseInit(){
			m_name = "DirectX Mouse";
			hwnd = NULL;
			dwCoopFlags = DISCL_EXCLUSIVE | DISCL_FOREGROUND;
		}
	private:

	};
	//!InputDevice is the base class for all devices added to the InputManager.
	class InputDevice
	{
		friend class Action;
		friend class ::InputManager;
	protected:

		std::string m_name; //!< The name of the device.

		LPDIRECTINPUT8 m_lpdi; //!< A pointer to the DirectInput8 object, initialized by InputManager.

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
	//! Keyboard is a specialization of the InputDevice provided by the InputManager by default.
	/*! This implementation of Keyboard is using the default DirectInput keyboard.  Keys are stored in an array of 256 BYTES.  The class
	* also provides the ability to covert input codes into a char if printing of typed keys was needed.  In addition to keyToChar(), appendToString() 
	* can be used to append each key to a previous string.
	*/
	class Keyboard : public InputDevice{
		friend class ::InputManager;
	protected:
		LPDIRECTINPUTDEVICE8 m_lpdikey; //!< a pointer to the keyboard device used by DirectInput.

		BYTE m_Key[256]; //!< array of BYTE which gets filled with the current state of the keyboard each time the keyboard is polled.

		bool m_Active;
		bool m_Acquired;

		bool init(InputDeviceInit &deviceInit);

		bool read();

		void release();
		//! Converts given directX keycode to a char.
		/*! Uses the directx keycode constants to find a corresponding char and returns it.
		* @param key The directx key constant.
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

		~Keyboard();



	};
	//! Mouse is a specialization of the InputDevice provided by the InputManager by default.
	/*! This implementation of Mouse is using the default DirectInput Mouse.  The buttons are stored as an array of BYTE, buttons can be referenced
	* by using the Mouse constants which are in the form of DIM_N, where N is the number of the button.  Mouse also offers the ability to change the mouse
	* options such as sensitivity, limits, and setting exclusive mode.
	*/
	class Mouse : public InputDevice{
		friend class ::InputManager;
	public:
		//! getRelativePosition() takes four LONG pointers and modifies the value they point to to be the current relative position of the mouse.
		void getRelativePosition(LONG* x, LONG* y, LONG* z);
		
		void setLimits(long x1, long y1, long x2, long y2); //!< Sets limits for the mouse.  Useful to limit the mouse to a smaller window.

		void setSensitivity(int factor); //!< sets the factor that mouse movements will be multiplied by, I currently don't have a range and need to add it in here when i find out.

		int getSensitivity() const; //!< returns a float containing the current sensitivity setting for the mouse.
		//! getAbsolutePosition() takes two ints and modifies their values so they point to the current absolute position of the mouse pointer.
		void getAbsolutePosition(int* x, int* y);

		long getRelativeX() const{
			return m_dx;
		}

		long getRelativeY() const{
			return m_dy;
		}

		long getRelativeZ() const{
			return m_dz;
		}
	protected:
		LPDIRECTINPUTDEVICE8 m_lpdimouse; //!< A pointer to the Mouse device used by DirectInput.

		BYTE m_Button[8]; //!< array of BYTE which gets filled with the current state of the Mouse buttons each time the Mouse is polled.

		LONG m_dx,	//!< LONG value representing the relative change in X the last time the Mouse was Polled.
			 m_dy,  //!< LONG value representing the relative change in Y the last time the Mouse was Polled.
			 m_dz;  //!< LONG value representing the relative change in Z(mouse wheel, usually) the last time the Mouse was Polled.

		bool m_Active;
		bool m_Acquired;

		LONG m_MouseSpeed;
		LONG m_MouseAbsX, m_MouseAbsY;

		bool init(InputDeviceInit &deviceInit);

		bool read();

		void release();

		bool buttonStatus(int code) const;
	public:
		Mouse();

		~Mouse();
	public:
		//! Returns absolute x position of mouse
		LONG getAbsX() const{
			return m_MouseAbsX;
		}
		//! Returns absolute y position of mouse
		LONG getAbsY() const{
			return m_MouseAbsY;
		}
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


		const std::string& m_Name; //!< The name of the Action.
		int m_Behavior; //!< A behavior flag.
		int m_Amount; //!< The amount this Action has been triggered.
		int m_State; //!< The current state of the Action.
		long m_ToggleTime; //!<Timeout before tapped button can be pressed again.
		std::map<std::string, std::set<std::set<int> > > deviceCodeMap;
		
		::HANDLE  hInputMutex;        //!< Input Mutex used to synchronize access to the Action functions.
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

	Input::Mouse m_Mouse; //!< provided Mouse implementation.

	//Create initialization structures
	Input::MouseInit m_MouseInit;
	Input::KeyboardInit m_KeyboardInit;

	Input::StrActionMap m_ActionMap; //!< Maps strings to Actions.

	Input::StrActionMap::iterator m_ActionItr; //!< An iterator for the string action map.

	Input::StrDeviceMap m_DeviceMap; //!< Maps strings to devices.

	LPDIRECTINPUT8 m_lpdi;

	InputManager();
	~InputManager();
	InputManager(const InputManager& rhs);
	InputManager& operator=(const InputManager& rhs);
public:
	friend CSingleton<InputManager>;

	bool init(HINSTANCE main_instance, HWND hwnd); //!< Initialize DirectInput and the rest of the input system.

	void shutdown();  //!< shutdown DirectInput and any resources used by the InputManager.
	
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

	Input::Mouse& getDIMouse(){
		return m_Mouse;
	}

	Input::Keyboard& getDIKeyboard(){
		return m_Keyboard;
	}
	//! Used to retrieve the Mouse initialization structure to modify settings before calling InputManager::init().
	Input::MouseInit& getMouseInit(){
		return m_MouseInit;
	}
	//! Used to retrieve the keyboard initialization structure to modify settings before calling InputManager::init().
	Input::KeyboardInit& getKeyboardInit(){
		return m_KeyboardInit;
	}
};

// different behavior when for holding down and tapping buttons.

