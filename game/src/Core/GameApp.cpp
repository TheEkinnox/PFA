#include "Core/GameApp.h"
#include "Core/GameContext.h"
#include "Debug/Log.h"

using namespace PFA::Events;
using namespace LibGL::Application;

namespace PFA::Core
{
	GameApp::GameApp() :
		IApplication(std::make_unique<GameContext>())
	{
	}

	void GameApp::onStart()
	{
		DEBUG_LOG("Game started\n");
	}

	void GameApp::onUpdate()
	{
	}

	void GameApp::onStop()
	{
		DEBUG_LOG("Game stopped\n");
	}
}
