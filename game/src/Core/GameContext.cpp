#include "Core/GameContext.h"
#include "Core/EventDefs.h"

#include "Debug/Log.h"
#include "Gameplay/CharacterController.h"
#include "Gameplay/Scenes/Level1.h"
#include "LowRenderer/Mesh.h"
#include "Resources/Texture.h"
#include "Utility/ServiceLocator.h"

// TODO (NTH) : Load config from a file
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define APP_TITLE "PFA - LAN (Esc. to close)"

using namespace LibGL;
using namespace LibMath;
using namespace LibGL::Utility;
using namespace LibGL::Application;
using namespace LibGL::Resources;
using namespace LibGL::Rendering;

using namespace PFA::Gameplay;
using namespace PFA::Events;

namespace PFA::Core
{
	GameContext::GameContext() :
		IContext(WINDOW_WIDTH, WINDOW_HEIGHT, APP_TITLE),
		m_renderer(std::make_unique<Renderer>()),
		m_inputManager(std::make_unique<InputManager>(*m_window)),
		m_resourcesManager(std::make_unique<ResourceManager>()),
		m_eventManager(std::make_unique<EventManager>()),
		m_audioManager(std::make_unique<AudioManager>()),
		m_scene(std::make_unique<Level1>())
	{
		Debug::Log::openFile("console.log");

		m_renderer->setCapability(ERenderingCapability::BLEND, true);
		m_renderer->setBlendFunc(EBlendFactor::SRC_ALPHA, EBlendFactor::ONE_MINUS_SRC_ALPHA);

		m_renderer->setCapability(ERenderingCapability::CULL_FACE, true);
		m_renderer->setCullFace(ECullFace::BACK);

		m_renderer->setCapability(ERenderingCapability::DEPTH_TEST, true);

		ServiceLocator::provide<Renderer>(*m_renderer);
		ServiceLocator::provide<InputManager>(*m_inputManager);
		ServiceLocator::provide<ResourceManager>(*m_resourcesManager);
		ServiceLocator::provide<AudioManager>(*m_audioManager);
		ServiceLocator::provide<EventManager>(*m_eventManager);

		bindExitFunc();
		bindRestartFunc();

		// Lock the mouse to the center of the window
		m_window->disableCursor();

		m_scene->load();
	}

	GameContext::~GameContext()
	{
		LGL_SERVICE(EventManager).unsubscribe<Events::ExitEvent>(m_exitListenerId);
	}

	void GameContext::update()
	{
		IContext::update();

		const auto& inputManager = LGL_SERVICE(InputManager);

		if (inputManager.isKeyPressed(EKey::KEY_ESCAPE))
		{
			LGL_SERVICE(LibGL::EventManager).broadcast<ExitEvent>();
			return;
		}

#ifdef _DEBUG
		if (inputManager.isKeyDown(EKey::KEY_LEFT_SHIFT) || inputManager.isKeyDown(EKey::KEY_RIGHT_SHIFT))
		{
			if (inputManager.isKeyPressed(EKey::KEY_R))
			{
				LGL_SERVICE(LibGL::EventManager).broadcast<RestartEvent>();
				return;
			}
		}
#endif

		m_inputManager->updateMouse();
		m_renderer->clear(Camera::getCurrent());

		if (m_inputManager->isKeyPressed(EKey::KEY_P))
		{
			m_isPaused = !m_isPaused;
			m_isPaused ? m_window->showCursor() : m_window->disableCursor();
			m_timer->setTimeScale(m_isPaused ? 0.f : 1.f);
		}

		m_scene->update();

		m_audioManager->getSoundEngine().update();
		LGL_SERVICE(Window).swapBuffers();
	}

	void GameContext::bindExitFunc()
	{
		const auto exitFunc = [this]
		{
			m_inputManager->clearStates();
			m_window->setShouldClose(true);
		};

		m_exitListenerId = LGL_SERVICE(EventManager).subscribe<ExitEvent>(exitFunc);
	}

	void GameContext::bindRestartFunc()
	{
		const auto restartFunc = [this]
		{
			loadScene<Level1>();
		};

		m_restartListenerId = LGL_SERVICE(EventManager).subscribe<RestartEvent>(restartFunc);
	}
}
