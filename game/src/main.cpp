#include "Core/GameApp.h"

#include <crtdbg.h>
#include <irrKlang.h>
using namespace irrklang;

int main()
{
//#ifdef _DEBUG
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif
//
//	PFA::Core::GameApp app;
//
//	app.run();
	ISoundEngine* engine = createIrrKlangDevice();
	if (!engine)
	{
		return 0;
	}

	engine->play2D("media/getout.ogg", true);

	while (true)
	{

	}
	engine->drop();
	return 0;
}
