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
		inline static const char* JUMP_SOUND = "assets/sounds/jump.wav";

		LibMath::Vector3	m_groundCheckPos;
		float				m_moveSpeed;
		float				m_rotationSpeed;
		float				m_jumpForce;

		/**
		 * \brief Handles keyboard inputs
		 */
		void handleKeyboard() const;

		/**
		 * \brief Handles mouse movement and inputs
		 */
		void handleMouse() const;

		/**
		 * \brief Updates the audio listener's data
		 */
		void updateAudioListener() const;

		/**
		 * \brief Makes the player jump
		 */
		void jump() const;
	};
}
