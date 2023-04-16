#include "Gameplay/CharacterController.h"

#include "Arithmetic.h"
#include "Angle/Degree.h"
#include "InputManager.h"
#include "Window.h"
#include "LowRenderer/Camera.h"
#include "Utility/ServiceLocator.h"
#include "Utility/Timer.h"
#include "CapsuleCollider.h"

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
		m_camera(this, Transform({0, CAM_HEIGHT, 0}, Vector3::zero(), Vector3::one() / transform.getScale()),
			Matrix4::perspectiveProjection(90_deg, LGL_SERVICE(Window).getAspect(), .1f, 150.f)),
		m_moveSpeed(moveSpeed), m_rotationSpeed(rotationSpeed),
		m_jumpForce(jumpForce)
	{
		Camera::setCurrent(m_camera);

		addComponent<CapsuleCollider>(*this, Vector3( 0, getScale().m_y / 2, 0 ), Vector3::up(), 1, .5f);

		// TODO: Uncomment when RigidBody is implemented
		//addComponent<RigidBody>(*this);
	}

	void CharacterController::update()
	{
		Entity::update();

		handleKeyboard();
		handleMouse();

		// TODO: Remove when RigidBody is implemented
		if (getPosition().m_y > 0)
		{
			const float gravity = -9.8f * LGL_SERVICE(Timer).getDeltaTime();
			translate({ 0, clamp(gravity, 0.f, -getPosition().m_y), 0 });
		}
	}

	void CharacterController::handleKeyboard()
	{
		const auto& inputManager = LGL_SERVICE(InputManager);
		Vector3 moveDir = Vector3::zero();

		if (inputManager.isKeyDown(EKey::KEY_SPACE))
			jump();

		if (inputManager.isKeyDown(EKey::KEY_W))
			moveDir = forward();

		if (inputManager.isKeyDown(EKey::KEY_S))
			moveDir = back();

		if (inputManager.isKeyDown(EKey::KEY_A))
			moveDir = left();

		if (inputManager.isKeyDown(EKey::KEY_D))
			moveDir = right();

		if (moveDir != Vector3::zero())
		{
			moveDir.m_y = 0;
			moveDir.normalize();
		}

		translate(moveDir * m_moveSpeed);
	}

	void CharacterController::handleMouse()
	{
		const auto& inputManager = LGL_SERVICE(InputManager);
		const float deltaTime = LGL_SERVICE(Timer).getDeltaTime();
		auto& camera = m_camera;

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
		// TODO: Uncomment when RigidBody is implemented
		//const Vector3 gravityDir = g_gravity.normalized();
		//if (!raycast(m_position, gravityDir, MAX_GROUND_DISTANCE))
		//	return;

		//RigidBody* rb = getComponent<RigidBody>();
		//ASSERT(rb != nullptr);

		//rb->addForce(m_jumpForce * -gravityDir);

		// TODO: Remove when RigidBody is implemented
		if (getPosition().m_y > MAX_GROUND_DISTANCE)
			return;

		const float jumpDistance = m_jumpForce * LGL_SERVICE(Timer).getDeltaTime();
		translate({ 0, max(jumpDistance, MAX_GROUND_DISTANCE), 0 });
	}
}
