#include "Core/GameApp.h"

#include <crtdbg.h>

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	PFA::Core::GameApp app;

	app.run();

	return 0;
}
