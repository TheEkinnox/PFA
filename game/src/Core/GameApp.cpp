#include "Core/GameApp.h"
#include "Core/EventDefs.h"
#include "Core/GameContext.h"
#include "Debug/Log.h"

#include "Eventing/EventManager.h"
#include "Utility/ServiceLocator.h"

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
		const auto& inputManager = LGL_SERVICE(InputManager);

		if (inputManager.isKeyDown(EKey::KEY_ESCAPE))
			LGL_SERVICE(LibGL::EventManager).broadcast<ExitEvent>();

#ifdef _DEBUG
		if (inputManager.isKeyDown(EKey::KEY_R))
			LGL_SERVICE(LibGL::EventManager).broadcast<RestartEvent>();
#endif
	}

	void GameApp::onStop()
	{
		DEBUG_LOG("Game stopped\n");
	}
}
