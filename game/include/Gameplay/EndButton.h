#pragma once
#include "Component.h"

namespace PFA::Gameplay
{
	class EndButton final : public LibGL::Component
	{
	public:
		EndButton(LibGL::Entity& owner, float useRange);

		void update() override;

	private:
		inline static const char* PRESSED_SOUND = "assets/sounds/finish.wav";

		float m_useRange;
	};
}
