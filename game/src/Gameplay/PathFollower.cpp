#include"Gameplay/PathFollower.h"

using namespace PFA::Gameplay;
using Vec3 = LibMath::Vector3;
PathFollower::PathFollower(const std::vector<Vec3>& p_path, ECyclingMode p_cyclingMode = LOOP)
{
	m_path = p_path;
	m_currentIndex = 0;
	m_cyclingMode = p_cyclingMode;
}

const Vec3& PathFollower::currentPoint()
{
	return m_path[m_currentIndex];
}

const Vec3& PathFollower::nextPoint()
{
	size_t nextIndex; 
	if (m_currentIndex == m_path.size() - 1)
	{
		switch (m_cyclingMode)
		{
		case PFA::Gameplay::LOOP:

			nextIndex = 0;
			break;

		case PFA::Gameplay::PING_PONG:

			std::reverse(m_path.begin(), m_path.end());
			m_currentIndex = 0;
			nextIndex = 1;

			break;

		default:
			break;
		}
	}
	
	else
	{
		nextIndex = m_currentIndex + 1;
	}

	m_currentIndex = nextIndex;

	return m_path[nextIndex];
}

void PathFollower::update()
{
	nextPoint();
}