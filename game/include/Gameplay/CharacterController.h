#pragma once
#include "Entity.h"
#include "LowRenderer/Camera.h"

namespace LibGL::Physics
{
	class CapsuleCollider;
}

namespace PFA::Gameplay
{
	class CharacterController final : public LibGL::Entity
	{
	public:
		CharacterController(Entity* parent, const Transform& transform,
			float moveSpeed, float rotationSpeed, float jumpForce);

		/**
		 * \brief Update the character controller
		 */
		void update() override;

	private:
		float	m_moveSpeed;
		float	m_rotationSpeed;
		float	m_jumpForce;

		/**
		 * \brief Handle keyboard inputs
		 */
		void handleKeyboard();

		/**
		 * \brief Handle mouse movement and inputs
		 */
		void handleMouse();

		/**
		 * \brief Makes the player jump
		 */
		void jump();
	};
}
