#pragma once

#include <Scene.h>

namespace PFA::Gameplay
{
	class IGameScene : public LibGL::Resources::Scene
	{
	public:
		/**
		 * \brief Loads the scene
		 * \return A reference to the current scene
		 */
		virtual IGameScene& load();

	protected:
		IGameScene(const LibMath::Vector3& spawnPoint, const LibMath::Vector3& spawnRotation);

	private:
		LibMath::Vector3	m_spawnPoint;
		LibMath::Vector3	m_spawnRotation;
	};
}