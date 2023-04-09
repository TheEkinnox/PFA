#include "Core/GameApp.h"
#include "Core/EventDefs.h"
#include "Core/GameContext.h"

#include "Debug/Log.h"
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
	}

	void GameApp::onUpdate()
	{
		const auto& inputManager = LGL_SERVICE(InputManager);

		if (inputManager.isKeyDown(EKey::KEY_ESCAPE))
			LGL_SERVICE(EventManager).broadcast<ExitEvent>();
	}

	void GameApp::onStop()
	{
	}
}
