#include "Arithmetic.h"
#include "Gameplay/CharacterController.h"
#include "Angle/Degree.h"
#include "InputManager.h"
#include "Window.h"
#include "LowRenderer/Camera.h"
#include "Utility/ServiceLocator.h"
#include "Utility/Timer.h"
#include "CapsuleCollider.h"
#include "Rigidbody.h"
#include "Raycast.h"
#include "Core/AudioManager.h"

#define MAX_GROUND_DISTANCE .1f // The maximum distance from the ground at which the player is considered to be touching it
#define SPRINT_MULTIPLIER 1.5f

using namespace LibMath;
using namespace LibMath::Literal;
using namespace LibGL;
using namespace LibGL::Utility;
using namespace LibGL::Physics;
using namespace LibGL::Application;
using namespace LibGL::Rendering;
using namespace PFA::Core;

namespace PFA::Gameplay
{
	CharacterController::CharacterController(Entity& owner, const Vector3 groundCheckPos,
		const float moveSpeed, const float rotationSpeed, const float jumpForce) :
		Component(owner), m_groundCheckPos(groundCheckPos),
		m_moveSpeed(moveSpeed), m_rotationSpeed(rotationSpeed),
		m_jumpForce(jumpForce)
	{
		if (getOwner().getComponent<Rigidbody>() == nullptr)
			getOwner().addComponent<Rigidbody>();
	}

	void CharacterController::update()
	{
		Component::update();

		handleKeyboard();
		handleMouse();
		updateAudioListener();
	}

	void CharacterController::handleKeyboard() const
	{
		Rigidbody* rb = getOwner().getComponent<Rigidbody>();
		ASSERT(rb != nullptr);

		const Transform transform = getOwner().getGlobalTransform();

		const auto& inputManager = LGL_SERVICE(InputManager);
		Vector3 moveDir = Vector3::zero();

		if (inputManager.isKeyPressed(EKey::KEY_SPACE))
			jump();

		if (inputManager.isKeyDown(EKey::KEY_W))
			moveDir += transform.forward();

		if (inputManager.isKeyDown(EKey::KEY_S))
			moveDir += transform.back();

		if (inputManager.isKeyDown(EKey::KEY_A))
			moveDir += transform.left();

		if (inputManager.isKeyDown(EKey::KEY_D))
			moveDir += transform.right();

		Vector3 targetVelocity;

		if (!floatEquals(m_moveSpeed, 0.f) && moveDir != Vector3::zero())
		{
			float speedScale = 1.f;

			if (inputManager.isKeyDown(EKey::KEY_LEFT_SHIFT) ||
				inputManager.isKeyDown(EKey::KEY_RIGHT_SHIFT))
				speedScale = SPRINT_MULTIPLIER;

			targetVelocity = moveDir * (m_moveSpeed * speedScale / moveDir.magnitude());
		}

		targetVelocity.m_y += rb->m_velocity.m_y;
		rb->m_velocity = targetVelocity;
	}

	void CharacterController::handleMouse() const
	{
		const auto& inputManager = LGL_SERVICE(InputManager);
		const float deltaTime = LGL_SERVICE(Timer).getDeltaTime();
		auto& camera = Camera::getCurrent();

		//update camera rotation
		const float rotationSpeed = m_rotationSpeed * deltaTime;

		const Vector2 mouseDelta = inputManager.getMouseDelta();

		if (mouseDelta == Vector2::zero())
			return;

		// Rotate the player from left to right
		Vector3 rotation = getOwner().getRotation();

		rotation.m_y += mouseDelta.m_x * rotationSpeed;
		rotation.m_y = wrap(rotation.m_y, 0.f, 360.f);

		getOwner().setRotation(rotation);

		// Make the camera look up or down
		Vector3 camRotation = camera.getRotation();

		camRotation.m_x += -mouseDelta.m_y * rotationSpeed;
		camRotation.m_x = clamp(camRotation.m_x, -80.f, 80.f);

		camera.setRotation(camRotation);
	}

	void CharacterController::updateAudioListener() const
	{
		auto& soundEngine = LGL_SERVICE(AudioManager).getSoundEngine();
		const auto transform = getOwner().getGlobalTransform();
		const auto pos = transform.getPosition();
		const auto fwd = transform.forward();

		soundEngine.setListenerPosition(
			{ pos.m_x, pos.m_y, pos.m_z },
			{ fwd.m_x, fwd.m_y, fwd.m_z }
		);
	}

	void CharacterController::jump() const
	{
		const Vector3 gravityDir = g_gravity.normalized();
		const Vector3 pos = getOwner().getGlobalTransform().getPosition();
		if (!raycast(pos + m_groundCheckPos,
			gravityDir, MAX_GROUND_DISTANCE))
			return;

		Rigidbody* rb = getOwner().getComponent<Rigidbody>();
		ASSERT(rb != nullptr);

		rb->addForce(m_jumpForce * -gravityDir, EForceMode::IMPULSE);

		auto& soundEngine = LGL_SERVICE(AudioManager).getSoundEngine();
		soundEngine.play3D(JUMP_SOUND, { pos.m_x, pos.m_y, pos.m_z });
	}
}
