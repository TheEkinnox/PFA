#include "Gameplay/Door.h"

#include "ICollider.h"
#include "Core/EventDefs.h"
#include "Debug/Assertion.h"
#include "Eventing/EventManager.h"
#include "LowRenderer/Mesh.h"
#include "Utility/ServiceLocator.h"
#include "Utility/Timer.h"

using namespace LibGL::Rendering;
using namespace LibGL;

namespace PFA::Gameplay
{
	Door::Door(Entity& owner, const Material& openMat, const Material& closedMat) :
		Component(owner), m_openMat(openMat), m_closedMat(closedMat), m_uvOffset(LibMath::Vector2::zero())
	{
		ASSERT(typeid(owner) == typeid(Mesh) || dynamic_cast<Mesh*>(&owner) != nullptr);
		bindOnColorChange();
	}

	Door::Door(const Door& other) :
		Component(other), m_openMat(other.m_openMat),
		m_closedMat(other.m_closedMat), m_uvOffset(other.m_uvOffset)
	{
	}

	Door::Door(Door&& other) noexcept :
		Component(std::move(other)), m_openMat(other.m_openMat),
		m_closedMat(other.m_closedMat), m_uvOffset(other.m_uvOffset)
	{
	}

	Door::~Door()
	{
		auto& eventManager = LGL_SERVICE(EventManager);
		eventManager.unsubscribe<Events::ColorChangedEvent>(m_colorChangeListenerId);
	}

	Door& Door::operator=(const Door& other)
	{
		if (&other == this)
			return *this;

		Component::operator=(other);
		m_openMat = other.m_openMat;
		m_closedMat = other.m_closedMat;
		m_uvOffset = other.m_uvOffset;

		return *this;
	}

	Door& Door::operator=(Door&& other) noexcept
	{
		if (&other == this)
			return *this;

		Component::operator=(other);
		m_openMat = other.m_openMat;
		m_closedMat = other.m_closedMat;
		m_uvOffset = other.m_uvOffset;

		return *this;
	}

	void Door::update()
	{
		Component::update();

		Entity& owner = getOwner();
		ASSERT(typeid(owner) == typeid(Mesh) || dynamic_cast<Mesh*>(&owner) != nullptr);

		const float scrollSpeed = LGL_SERVICE(Utility::Timer).getUnscaledDeltaTime() * .5f;
		m_uvOffset += LibMath::Vector2(scrollSpeed);

		Mesh& ownerMesh = dynamic_cast<Mesh&>(owner);
		ownerMesh.getMaterial().setUVOffset(m_uvOffset);
	}

	void Door::bindOnColorChange()
	{
		const auto onColorChangeFunc = [this](const Color& newColor)
		{
			if (newColor.rgb() == m_closedMat.getTint().rgb())
				open();
			else
				close();
		};

		auto& eventManager = LGL_SERVICE(EventManager);
		m_colorChangeListenerId = eventManager.subscribe<Events::ColorChangedEvent>(
			onColorChangeFunc);
	}

	void Door::open() const
	{
		Entity& owner = getOwner();
		ASSERT(typeid(owner) == typeid(Mesh) || dynamic_cast<Mesh*>(&owner) != nullptr);

		const auto& colliders = owner.getComponents<Physics::ICollider>();

		for (const auto& collider : colliders)
			collider->setActive(false);

		Mesh& ownerMesh = dynamic_cast<Mesh&>(owner);
		ownerMesh.getMaterial() = m_openMat;
	}

	void Door::close() const
	{
		Entity& owner = getOwner();
		ASSERT(typeid(owner) == typeid(Mesh) || dynamic_cast<Mesh*>(&owner) != nullptr);

		const auto& colliders = owner.getComponents<Physics::ICollider>();

		for (const auto& collider : colliders)
			collider->setActive(true);

		Mesh& ownerMesh = dynamic_cast<Mesh&>(owner);
		ownerMesh.getMaterial() = m_closedMat;
	}
}
