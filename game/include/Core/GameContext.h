#pragma once
#include "IContext.h"
#include "InputManager.h"
#include "LowRenderer/Camera.h"
#include "Resources/ResourcesManager.h"
#include "Eventing/EventManager.h"
#include "Core/Renderer.h"
#include "Gameplay/Scenes/IGameScene.h"
#include "LowRenderer/Light.h"

// TODO (NTH): use an ssbo for the lights
#define NB_POINT_LIGHTS 8

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
		std::unique_ptr<Gameplay::IGameScene>				m_scene;

		LibGL::Rendering::DirectionalLight					m_dirLight;
		LibGL::Rendering::PointLight						m_pointLights[NB_POINT_LIGHTS];

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
		void loadScene() const;

		/**
		 * \brief Loads the shader with the given fileName
		 * \param fileName The path of the lit shader to get
		 * \return A pointer to the loaded lit shader on success, nullptr otherwise
		 */
		const LibGL::Resources::Shader* getLitShader(const std::string& fileName) const;

		/**
		 * \brief Update the lighting data for the shader with the given file name
		 * \param fileName The path of the lit shader to update
		 */
		void updateLitShader(const std::string& fileName) const;
	};
}
