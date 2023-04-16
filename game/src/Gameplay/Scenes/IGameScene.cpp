#include "Gameplay/Scenes/IGameScene.h"

#include "Gameplay/CharacterController.h"

#define MOVE_SPEED 2.f		// Player move speed in game units per second
#define ROTATION_SPEED 30.f	// Player/Camera rotation speed in degrees per second
#define JUMP_FORCE 20.f		// Player jump force in Newton

using namespace LibMath;
using namespace LibGL::Rendering;

namespace PFA::Gameplay
{
	IGameScene::IGameScene(const LibMath::Vector3& spawnPoint,
		const LibMath::Vector3& spawnRotation) : m_spawnPoint(spawnPoint),
		m_spawnRotation(spawnRotation)
	{
	}

	IGameScene& IGameScene::load()
	{
		// Clear the scene in case it was already loaded
		clear();

		// Setup the character
		static const Transform startTransform(m_spawnPoint, m_spawnRotation, Vector3(1, 1.75, 1));
		CharacterController player(nullptr, startTransform, MOVE_SPEED, ROTATION_SPEED, JUMP_FORCE);

		// Add the player to the scene
		addNode(player);

		return *this;
	}
}
