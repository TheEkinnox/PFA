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

#define CAM_HEIGHT 1.63f		// Player's "eyes" local height
#define MAX_GROUND_DISTANCE .1f // The maximum distance from the ground at which the player is considered to be touching it

using namespace LibMath;
using namespace LibMath::Literal;
using namespace LibGL::Utility;
using namespace LibGL::Physics;
using namespace LibGL::Application;
using namespace LibGL::Rendering;

namespace PFA::Gameplay
{
	CharacterController::CharacterController(Entity* parent, const Transform& transform,
		const float moveSpeed, const float rotationSpeed, const float jumpForce) :
		Entity(parent, transform),
		m_moveSpeed(moveSpeed), m_rotationSpeed(rotationSpeed),
		m_jumpForce(jumpForce)
	{
		Camera& cam = Camera::getCurrent();

		cam.setPosition({ 0.f, CAM_HEIGHT, 0.f });
		cam.setRotation(Vector3::zero());
		cam.setScale(Vector3::one() / transform.getScale());

		Entity::addChild(cam);

		addComponent<CapsuleCollider>(Vector3( 0.f, getScale().m_y / 2.f, 0.f ), Vector3::up(), 1.f, .5f);
		addComponent<Rigidbody>();
	}

	void CharacterController::update()
	{
		Entity::update();

		handleKeyboard();
		handleMouse();
	}

	void CharacterController::handleKeyboard()
	{
		Rigidbody* rb = getComponent<Rigidbody>();
		ASSERT(rb != nullptr);

		const auto& inputManager = LGL_SERVICE(InputManager);
		Vector3 moveDir = Vector3::zero();

		if (inputManager.isKeyDown(EKey::KEY_SPACE))
			jump();

		if (inputManager.isKeyDown(EKey::KEY_W))
			moveDir += forward();

		if (inputManager.isKeyDown(EKey::KEY_S))
			moveDir += back();

		if (inputManager.isKeyDown(EKey::KEY_A))
			moveDir += left();

		if (inputManager.isKeyDown(EKey::KEY_D))
			moveDir += right();

		Vector3 targetVelocity;

		if (!floatEquals(m_moveSpeed, 0.f) && moveDir != Vector3::zero())
		{
			moveDir.m_y = 0;
			targetVelocity = moveDir * (m_moveSpeed / moveDir.magnitude());
		}

		targetVelocity.m_y += rb->m_velocity.m_y;
		rb->m_velocity = targetVelocity;
	}

	void CharacterController::handleMouse()
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
		Vector3 rotation = getRotation();

		rotation.m_y += mouseDelta.m_x * rotationSpeed;
		rotation.m_y = wrap(rotation.m_y, 0.f, 360.f);

		setRotation(rotation);

		// Make the camera look up or down
		Vector3 camRotation = camera.getRotation();

		camRotation.m_x += -mouseDelta.m_y * rotationSpeed;
		camRotation.m_x = clamp(camRotation.m_x, -80.f, 80.f);

		camera.setRotation(camRotation);
	}

	void CharacterController::jump()
	{
		const Vector3 gravityDir = g_gravity.normalized();
		if (!raycast(getPosition() + gravityDir * .1f, gravityDir, MAX_GROUND_DISTANCE))
			return;

		Rigidbody* rb = getComponent<Rigidbody>();
		ASSERT(rb != nullptr);

		rb->addForce(m_jumpForce * -gravityDir, EForceMode::IMPULSE);
	}
}
