#pragma once
#include "Entity.h"
#include "Core/EventDefs.h"
#include "Resources/Material.h"

namespace PFA::Gameplay
{
	class Door final : public LibGL::Component
	{
	public:
		LibGL::Rendering::Material m_openMat;
		LibGL::Rendering::Material m_closedMat;

		Door(LibGL::Entity& owner, const LibGL::Rendering::Material& openMat,
			const LibGL::Rendering::Material& closedMat);

		Door(const Door& other);
		Door(Door&& other) noexcept;

		~Door() override;

		Door& operator=(const Door& other);
		Door& operator=(Door&& other) noexcept;

		void update() override;

	private:
		Events::ColorChangedEvent::ListenerId	m_colorChangeListenerId = 0;
		LibMath::Vector2						m_uvOffset;

		void bindOnColorChange();
		void open() const;
		void close() const;
	};
}
