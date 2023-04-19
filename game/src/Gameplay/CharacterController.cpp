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

#define MAX_GROUND_DISTANCE .1f // The maximum distance from the ground at which the player is considered to be touching it

using namespace LibMath;
using namespace LibMath::Literal;
using namespace LibGL;
using namespace LibGL::Utility;
using namespace LibGL::Physics;
using namespace LibGL::Application;
using namespace LibGL::Rendering;

namespace PFA::Gameplay
{
	CharacterController::CharacterController(Entity& owner, Vector3 groundCheckPos,
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
			const Vector3 gravityMask
			{
				LibMath::abs(g_gravity.m_x) > 0.f ? 1.f : 0.f,
				LibMath::abs(g_gravity.m_y) > 0.f ? 1.f : 0.f,
				LibMath::abs(g_gravity.m_z) > 0.f ? 1.f : 0.f
			};

			moveDir -= moveDir * gravityMask;
			targetVelocity = moveDir * (m_moveSpeed / moveDir.magnitude());
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

	void CharacterController::jump() const
	{
		const Vector3 gravityDir = g_gravity.normalized();
		if (!raycast(getOwner().getGlobalTransform().getPosition() + m_groundCheckPos,
			gravityDir, MAX_GROUND_DISTANCE))
			return;

		Rigidbody* rb = getOwner().getComponent<Rigidbody>();
		ASSERT(rb != nullptr);

		rb->addForce(m_jumpForce * -gravityDir, EForceMode::IMPULSE);
	}
}
