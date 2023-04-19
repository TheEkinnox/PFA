#pragma once
#include "Entity.h"
#include "LowRenderer/Camera.h"

namespace LibGL::Physics
{
	class CapsuleCollider;
}

namespace PFA::Gameplay
{
	class CharacterController final : public LibGL::Component
	{
	public:
		CharacterController(LibGL::Entity& owner, LibMath::Vector3 groundCheckPos,
			float moveSpeed, float rotationSpeed, float jumpForce);

		/**
		 * \brief Update the character controller
		 */
		void update() override;

	private:
		LibMath::Vector3	m_groundCheckPos;
		float				m_moveSpeed;
		float				m_rotationSpeed;
		float				m_jumpForce;

		/**
		 * \brief Handle keyboard inputs
		 */
		void handleKeyboard() const;

		/**
		 * \brief Handle mouse movement and inputs
		 */
		void handleMouse() const;

		/**
		 * \brief Makes the player jump
		 */
		void jump() const;
	};
}
