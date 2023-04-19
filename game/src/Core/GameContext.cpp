#include "Core/GameContext.h"
#include "Core/EventDefs.h"

#include "Debug/Log.h"
#include "Gameplay/CharacterController.h"
#include "Gameplay/Scenes/Level1.h"
#include "LowRenderer/Mesh.h"
#include "Resources/Texture.h"
#include "Utility/ServiceLocator.h"
#include "Angle/Degree.h"

// TODO (NTH) : Load config from a file
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define APP_TITLE "PFA - LAN (Esc. to close)"

using namespace LibGL;
using namespace LibMath;
using namespace LibMath::Literal;
using namespace LibGL::Utility;
using namespace LibGL::Application;
using namespace LibGL::Resources;
using namespace LibGL::Rendering;

using namespace PFA::Gameplay;

namespace PFA::Core
{
	GameContext::GameContext() :
		IContext(WINDOW_WIDTH, WINDOW_HEIGHT, APP_TITLE),
		m_camera(std::make_unique<Camera>()),
		m_renderer(std::make_unique<Renderer>()),
		m_inputManager(std::make_unique<InputManager>(*m_window)),
		m_resourcesManager(std::make_unique<ResourceManager>()),
		m_eventManager(std::make_unique<EventManager>()),
		m_scene(std::make_unique<Level1>())
	{
		Debug::Log::openFile("console.log");

		ServiceLocator::provide<Renderer>(*m_renderer);
		ServiceLocator::provide<InputManager>(*m_inputManager);
		ServiceLocator::provide<ResourceManager>(*m_resourcesManager);
		ServiceLocator::provide<EventManager>(*m_eventManager);

		m_camera->setProjectionMatrix(Matrix4::perspectiveProjection(90_deg,
			LGL_SERVICE(Window).getAspect(), .1f, 150.f));

		Camera::setCurrent(*m_camera);

		bindExitFunc();
		bindRestartFunc();

		m_scene->load();
	}

	GameContext::~GameContext()
	{
		LGL_SERVICE(EventManager).unsubscribe<Events::ExitEvent>(m_exitListenerId);
	}

	void GameContext::update()
	{
		IContext::update();
		m_renderer->clear(Camera::getCurrent());

		m_scene->update();

		LGL_SERVICE(Window).swapBuffers();
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

	void GameContext::bindRestartFunc()
	{
		const auto restartFunc = [&]
		{
			loadScene<Level1>();
		};

		m_restartListenerId = LGL_SERVICE(EventManager).subscribe<Events::RestartEvent>(restartFunc);
	}
}
