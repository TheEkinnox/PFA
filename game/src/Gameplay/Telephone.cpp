#include "Gameplay/Telephone.h"

#include "InputManager.h"
#include "Utility/ServiceLocator.h"
#include "Debug/Assertion.h"
#include "Raycast.h"
#include "Gameplay/Cube.h"
#include "LowRenderer/Camera.h"
#include "ICollider.h"
#include "Core/AudioManager.h"
#include "Core/EventDefs.h"
#include "Eventing/EventManager.h"
#include "LowRenderer/Mesh.h"
#include "Vector/Vector4.h"

using namespace LibMath;
using namespace LibGL::Application;
using namespace LibGL::Rendering;
using namespace LibGL::Physics;
using namespace LibGL;
using namespace PFA::Core;
using namespace PFA::Events;

namespace PFA::Gameplay
{
	Telephone::Telephone(Entity& owner, const Color color) :
		Component(owner), m_color(color)
	{
		const auto onSceneLoaded = [this]
		{
			updateColor();
		};

		m_sceneLoadedListenerId = LGL_SERVICE(EventManager).subscribe<SceneLoadedEvent>(onSceneLoaded);
	}

	Telephone::~Telephone()
	{
		if (m_sceneLoadedListenerId != 0)
			LGL_SERVICE(EventManager).unsubscribe<SceneLoadedEvent>(m_sceneLoadedListenerId);
	}

	void Telephone::updateColor() const
	{
		Entity& owner = getOwner();
		ASSERT(typeid(owner) == typeid(Mesh) || dynamic_cast<Mesh*>(&owner) != nullptr);

		Mesh& ownerMesh = dynamic_cast<Mesh&>(owner);
		ownerMesh.getMaterial().setTint(m_color);

		auto& eventManager = LGL_SERVICE(EventManager);
		eventManager.broadcast<ColorChangedEvent>(m_color);
	}

	void Telephone::update()
	{
		Component::update();

		const auto& inputManager = LGL_SERVICE(InputManager);
		const auto& camera = Camera::getCurrent();

		const auto& camCollider = dynamic_cast<Entity*>(camera.getParent())->getComponent<ICollider>();

		if (inputManager.isMouseButtonPressed(EMouseButton::MOUSE_BUTTON_LEFT))
		{
			Vector3 castPos = camera.getGlobalTransform().getPosition();

			if (camCollider != nullptr)
			{
				const Vector3 castOffset = camera.getGlobalTransform().forward() * camCollider->getBounds().m_sphereRadius;
				const Vector3 closestOnSurface = camCollider->getClosestPointOnSurface(castPos + castOffset);

				castPos = closestOnSurface + castOffset.normalized() * .01f;
			}

			RaycastHit hitInfo;
			if (raycast(castPos,
				camera.getGlobalTransform().forward(), hitInfo))
			{
				const Cube* intersectedCube = hitInfo.m_collider->getOwner().getComponent<Cube>();

				if (intersectedCube != nullptr)
				{
					intersectedCube->swapColors(m_color);
					updateColor();

					const Vector3 pos = intersectedCube->getOwner().getGlobalTransform().getPosition();
					auto& soundEngine = LGL_SERVICE(AudioManager).getSoundEngine();
					soundEngine.play3D(SWAP_SOUND, { pos.m_x, pos.m_y, pos.m_z });
				}
			}
		}
	}
}