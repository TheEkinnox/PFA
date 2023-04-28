#include"Gameplay/PathFollower.h"

#include <Utility/ServiceLocator.h>
#include <Utility/Timer.h>
#include <Entity.h>

using namespace PFA::Gameplay;
using Vec3 = LibMath::Vector3;

PathFollower::PathFollower(LibGL::Entity& p_owner, const std::vector<Vec3>& p_path,
	const ECyclingMode p_cyclingMode, const float p_moveSpeed):
	Component( p_owner),
		m_path(p_path),
		m_currentIndex(0),
		m_nextIndex(1),
		m_cyclingMode(p_cyclingMode),
		m_moveSpeed(p_moveSpeed)
{
	getOwner().setPosition(p_path[0]);
}

Vec3 PathFollower::currentPoint()const
{
	return m_path[m_currentIndex];
}

 Vec3 PathFollower::nextPoint() const
{
	 return m_path[m_nextIndex];
}

void PathFollower::computeNext()
{
	if (m_currentIndex == m_path.size() - 1)
	{
		switch (m_cyclingMode)
		{
		case ECyclingMode::LOOP:

			m_nextIndex = 0;
			break;

		case ECyclingMode::PING_PONG:

			std::ranges::reverse(m_path);
			m_currentIndex = 0;
			m_nextIndex = 1;

			break;

		default:
			break;
		}
	}
	else
	{
		m_nextIndex = m_currentIndex + 1;
	}
}

void PathFollower::update()
{
	if (!isActive())
		return;

	const float deltaTime = LGL_SERVICE(LibGL::Utility::Timer).getDeltaTime();

	const Vec3 toNext = nextPoint() - getOwner().getPosition();
	const float distanceToNext = toNext.magnitudeSquared();
	const float moveSpeed = m_moveSpeed * deltaTime;

	if (distanceToNext < moveSpeed * moveSpeed)
	{
		m_currentIndex = m_nextIndex;
		computeNext();
		getOwner().setPosition(currentPoint());
	}
	else
	{
		getOwner().translate(toNext.normalized() * moveSpeed);
	}
}