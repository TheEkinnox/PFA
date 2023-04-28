#pragma once

#include <Scene.h>

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

		IGameScene(const IGameScene& other) = default;
		IGameScene(IGameScene&& other) = default;
		~IGameScene() override = default;

		IGameScene& operator=(const IGameScene& other) = default;
		IGameScene& operator=(IGameScene&& other) = default;

	protected:
		IGameScene(const LibMath::Vector3& spawnPoint, const LibMath::Vector3& spawnRotation);

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

	private:
		LibMath::Vector3			m_spawnPoint;
		LibMath::Vector3			m_spawnRotation;
		LibGL::Event<>::ListenerId	m_resizeListenerId = 0;
	};
}
