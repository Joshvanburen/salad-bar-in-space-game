#include "Common.h"
#include "MastEventReceiver.cpp"
#include "GameIncludes.h"


#define WIN32_LEAN_AND_MEAN
#include "windows.h"




bool running = true;


InputManager* m_InputMgr = InputManager::getSingletonPtr();
void pollWiimote()
{
	while(running){
		m_InputMgr->getWiimote().poll();
		::Sleep(10);
	}
}


#ifdef DEBUG
int main()
{
#else
extern "C" {

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT ){
#endif


	GameSystem* m_GameSystem = GameSystem::getSingletonPtr();

	m_GameSystem->init();

	boost::thread inputThread(&pollWiimote);

	m_GameSystem->run();

	running = false;
	inputThread.join();

	m_GameSystem->shutdown();
	return 0;
#ifdef DEBUG
}
#else
}
}
#endif

