#pragma once
#include "IContext.h"
#include "InputManager.h"
#include "LowRenderer/Camera.h"
#include "Resources/ResourcesManager.h"
#include "Core/EventManager.h"

namespace PFA::Core
{
	class GameContext final : public LibGL::Application::IContext
	{
	public:
		GameContext();
		~GameContext() override;

		void update() override;
		void bindExitFunc();

	private:
		IEvent::ListenerId									m_exitListenerId = 0;

		std::unique_ptr<LibGL::Rendering::Camera>			m_camera;
		std::unique_ptr<LibGL::Application::InputManager>	m_inputManager;
		std::unique_ptr<LibGL::Resources::ResourceManager>	m_resourcesManager;
		std::unique_ptr<EventManager>						m_eventManager;
	};
}
