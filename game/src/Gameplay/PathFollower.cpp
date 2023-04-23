#include"Gameplay/PathFollower.h"
#include <Utility/ServiceLocator.h>

using namespace PFA::Gameplay;
using Vec3 = LibMath::Vector3;



PFA::Gameplay::PathFollower::PathFollower(LibGL::Entity& p_owner, const std::vector<Vec3>& p_path, ECyclingMode p_cyclingMode, float p_moveSpeed):
	LibGL::Component( p_owner),
		m_path(p_path),
		m_currentIndex(0),
		m_nextIndex(1),
		m_cyclingMode(p_cyclingMode),
		m_moveSpeed(p_moveSpeed)
{
	
}

Vec3 PathFollower::currentPoint()const
{
	return m_path[m_currentIndex];
}

 Vec3 PathFollower::nextPoint() const
{
	 return m_path[m_nextIndex];
}

void PFA::Gameplay::PathFollower::computeNext()
{
	if (m_currentIndex == m_path.size() - 1)
	{
		switch (m_cyclingMode)
		{
		case ECyclingMode::LOOP:

			m_nextIndex = 0;
			break;

		case ECyclingMode::PING_PONG:

			std::reverse(m_path.begin(), m_path.end());
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
	float deltaTime = LGL_SERVICE(LibGL::Utility::Timer).getDeltaTime();

	if (!isActive())
		return;

	const Vec3& next = nextPoint();

	Vec3 toNext = next - getOwner().getPosition();
	float distanceToNext = toNext.magnitude();

	if (distanceToNext < m_moveSpeed)
	{
		m_currentIndex = m_nextIndex;
		computeNext();
	}
	else
	{
		getOwner().translate(toNext.normalized() * m_moveSpeed * deltaTime);
	}
}