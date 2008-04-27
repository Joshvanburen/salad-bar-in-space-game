
#ifndef PAUSE_H
#define PAUSE_H

#include "Common.h"

class Pause{

public:

	Pause();
	Pause(bool p );
	bool pause();
	bool unPause();
	bool isPaused();

private:
	bool b_pause;

};
#endif