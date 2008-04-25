#include "console.h"



class EXECUTESCRIPT : public IC_Command
{
public:
	EXECUTESCRIPT();
	virtual ~EXECUTESCRIPT();
	bool invoke(const array<WideString>& args, IC_Dispatcher *pDispatcher, IC_MessageSink* pOutput);
};


