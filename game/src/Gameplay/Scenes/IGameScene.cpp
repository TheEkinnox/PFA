#include "Gameplay/Scenes/IGameScene.h"

#include "Gameplay/CharacterController.h"
#include "CapsuleCollider.h"

#define MOVE_SPEED 2.f		// Player's move speed in game units per second
#define ROTATION_SPEED 30.f	// Player's/Camera's rotation speed in degrees per second
#define JUMP_FORCE 6.f		// Player's jump impulse force in Newton
#define CAM_HEIGHT 1.63f	// Player's "eyes" local height
#define PLAYER_SIZE 1.75f	// Player's capsule collider height

using namespace LibMath;
using namespace LibGL::Rendering;
using namespace LibGL::Physics;

namespace PFA::Gameplay
{
	IGameScene::IGameScene(const Vector3& spawnPoint,
		const Vector3& spawnRotation) : m_spawnPoint(spawnPoint),
		m_spawnRotation(spawnRotation)
	{
	}

	IGameScene& IGameScene::load()
	{
		// Clear the scene in case it was already loaded
		clear();

		Camera& cam = addNode(Camera::getCurrent());
		cam.removeComponent<CapsuleCollider>();
		cam.removeComponent<CharacterController>();

		cam.setPosition(m_spawnPoint + Vector3( 0.f, CAM_HEIGHT, 0.f ));
		cam.setRotation(m_spawnRotation);
		cam.setScale(Vector3::one());

		// Setup the collider and character controller
		// A rigidbody is automatically added by the character controller
		cam.addComponent<CapsuleCollider>(Vector3(0.f, PLAYER_SIZE / 2.f, 0.f),
			Vector3::up(), PLAYER_SIZE, .5f);

		CharacterController player = cam.addComponent<CharacterController>(
			Vector3( 0.f, -CAM_HEIGHT - .1f, 0.f ), MOVE_SPEED, ROTATION_SPEED,
			JUMP_FORCE);

		Camera::setCurrent(cam);

		return *this;
	}
}
