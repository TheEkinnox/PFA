#include "Gameplay/EndButton.h"

#include "ColliderOverlaps.h"
#include "Entity.h"
#include "ICollider.h"
#include "InputManager.h"
#include "Core/AudioManager.h"
#include "Core/EventDefs.h"
#include "Eventing/EventManager.h"
#include "Gameplay/CharacterController.h"
#include "Utility/ServiceLocator.h"

using namespace LibMath;
using namespace LibGL::Physics;
using namespace LibGL::Application;
using namespace LibGL;
using namespace PFA::Core;
using namespace PFA::Events;

namespace PFA::Gameplay
{
	EndButton::EndButton(Entity& owner, const float useRange) :
		Component(owner), m_useRange(useRange)
	{
	}

	void EndButton::update()
	{
		Component::update();

		const auto& inputManager = LGL_SERVICE(InputManager);

		if (inputManager.isKeyPressed(EKey::KEY_E))
		{
			const Vector3 pos = getOwner().getGlobalTransform().getPosition();

			const auto colliders = overlapSphere(pos, m_useRange);

			for (const auto& collider : colliders)
			{
				if (collider != nullptr &&
					collider->getOwner().getComponent<CharacterController>())
				{
					auto& soundEngine = LGL_SERVICE(AudioManager).getSoundEngine();
					soundEngine.play3D(PRESSED_SOUND, { pos.m_x, pos.m_y, pos.m_z });
					LGL_SERVICE(EventManager).broadcast<LevelCompleteEvent>();
					break;
				}
			}
		}
	}
}
