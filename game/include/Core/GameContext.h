#pragma once
#include "AudioManager.h"
#include "IContext.h"
#include "InputManager.h"
#include "Resources/ResourcesManager.h"
#include "Eventing/EventManager.h"
#include "Core/Renderer.h"
#include "Gameplay/Scenes/IGameScene.h"

namespace PFA::Core
{
	class GameContext final : public LibGL::Application::IContext
	{
	public:
		GameContext();
		~GameContext() override;

		/**
		 * \brief Updates the game context
		 */
		void update() override;

	private:
		LibGL::IEvent::ListenerId							m_exitListenerId = 0;
		LibGL::IEvent::ListenerId							m_restartListenerId = 0;

		std::unique_ptr<LibGL::Rendering::Renderer>			m_renderer;
		std::unique_ptr<LibGL::Application::InputManager>	m_inputManager;
		std::unique_ptr<LibGL::Resources::ResourceManager>	m_resourcesManager;
		std::unique_ptr<LibGL::EventManager>				m_eventManager;
		std::unique_ptr<Core::AudioManager>					m_audioManager;
		std::unique_ptr<Gameplay::IGameScene>				m_scene;
		bool												m_isPaused = false;

		/**
		 * \brief Binds the game's exit function
		 */
		void bindExitFunc();

		/**
		 * \brief Binds the game's restart function
		 */
		void bindRestartFunc();

		/**
		 * \brief Loads (or reloads) the game scene
		 */
		template <typename SceneT>
		void loadScene();
	};
}

#include "Core/GameContext.inl"
