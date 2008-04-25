#include "console.h"

class LOADSCRIPT : public IC_Command
{
public:
	LOADSCRIPT();
	virtual ~LOADSCRIPT();
	bool invoke(const array<WideString>& args, IC_Dispatcher *pDispatcher, IC_MessageSink* pOutput);
};
