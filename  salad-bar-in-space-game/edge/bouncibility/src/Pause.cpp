
#include "Pause.h"

Pause::Pause(){
	b_pause = false;
}

Pause::Pause( bool p ){
	b_pause = p;
}

bool Pause::pause(){
	b_pause = true;
	return b_pause;
}

bool Pause::unPause(){
	b_pause = false;
	return b_pause;
}

bool Pause::isPaused(){
	return b_pause;
}




