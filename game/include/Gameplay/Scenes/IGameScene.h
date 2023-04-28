#pragma once

#include <Scene.h>

#include "Core/EventDefs.h"

namespace LibGL::Resources
{
	class Shader;
}

namespace PFA::Gameplay
{
	class IGameScene : public LibGL::Resources::Scene
	{
	public:
		IGameScene(const IGameScene& other) = default;
		IGameScene(IGameScene&& other) = default;
		~IGameScene() override;

		IGameScene& operator=(const IGameScene& other) = default;
		IGameScene& operator=(IGameScene&& other) = default;

		/**
		 * \brief Loads the scene
		 * \return A reference to the current scene
		 */
		virtual IGameScene& load();

	protected:
		Events::LevelCompleteEvent::ListenerId	m_levelCompleteListenerId = 0;

		IGameScene(const LibMath::Vector3& spawnPoint, const LibMath::Vector3& spawnRotation,
			const LibMath::Vector3& endPoint);

		/**
		 * \brief Gets or loads the shader with the given file name
		 * \param fileName The path of the shader to load
		 * \return A pointer to the shader on success. Nullptr otherwise.
		 */
		static const LibGL::Resources::Shader* getShader(const std::string& fileName);

		/**
		 * \brief Update the uniforms for the shader with the given file name
		 * \param fileName The path of the shader to update
		 */
		static void updateShader(const std::string& fileName);

		/**
		 * \brief Binds the function to call when the current level is complete
		 */
		virtual void bindLevelCompleteListener() = 0;

	private:
		LibMath::Vector3			m_spawnPoint;
		LibMath::Vector3			m_spawnRotation;
		LibMath::Vector3			m_endPoint;
		LibGL::Event<>::ListenerId	m_resizeListenerId = 0;

		/**
		 * \brief Places the level's end point
		 */
		void addEndPoint();
	};
}
