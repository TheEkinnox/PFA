#include "Core/GameContext.h"
#include "Core/EventDefs.h"

#include "Debug/Log.h"
#include "Utility/ServiceLocator.h"

// TODO (NTH) : Load config from a file
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define APP_TITLE "PFA - LAN (Esc. to close)"

using namespace LibGL;
using namespace LibGL::Utility;
using namespace LibGL::Application;
using namespace LibGL::Resources;
using namespace LibGL::Rendering;

namespace PFA::Core
{
	GameContext::GameContext() :
		IContext(WINDOW_WIDTH, WINDOW_HEIGHT, APP_TITLE),
		m_camera(std::make_unique<Camera>()),
		m_inputManager(std::make_unique<InputManager>(*m_window)),
		m_resourcesManager(std::make_unique<ResourceManager>()),
		m_eventManager(std::make_unique<EventManager>())
	{
		Debug::Log::openFile("console.log");

		ServiceLocator::provide<Camera>(*m_camera);
		ServiceLocator::provide<InputManager>(*m_inputManager);
		ServiceLocator::provide<ResourceManager>(*m_resourcesManager);
		ServiceLocator::provide<EventManager>(*m_eventManager);

		bindExitFunc();
	}

	GameContext::~GameContext()
	{
		LGL_SERVICE(EventManager).unsubscribe<Events::ExitEvent>(m_exitListenerId);
	}

	void GameContext::update()
	{
		IContext::update();
	}

	void GameContext::bindExitFunc()
	{
		const auto exitFunc = [&]
		{
			m_inputManager->clearStates();
			m_window->setShouldClose(true);
		};

		m_exitListenerId = LGL_SERVICE(EventManager).subscribe<Events::ExitEvent>(exitFunc);
	}
}
