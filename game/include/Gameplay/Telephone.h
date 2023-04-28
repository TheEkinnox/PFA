#ifndef TELEPHONE_H
#define TELEPHONE_H
#include "Component.h"
#include "Core/Color.h"
#include "Core/EventDefs.h"

namespace PFA::Gameplay
{
	class Telephone final : public LibGL::Component
	{
	public:
		explicit Telephone(LibGL::Entity& owner,
			LibGL::Rendering::Color color = LibGL::Rendering::Color::white);

		~Telephone() override;

	private:
		inline static const char* SWAP_SOUND = "assets/sounds/swap.wav";

		LibGL::Rendering::Color					m_color;
		Events::SceneLoadedEvent::ListenerId	m_sceneLoadedListenerId = 0;

		void updateColor() const;
		void update() override;
	};
}
#endif // TELEPHONE_H
