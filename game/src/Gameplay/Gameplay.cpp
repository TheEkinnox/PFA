#include"Gameplay/Gameplay.h"

using namespace PFA::Gameplay;

PFA::Gameplay::PathFollower::PathFollower(const std::vector<Vec3>& path, CyclingMode cyclingMode = LOOP)
{
	m_path = path;
	m_currentIndex = 0;
	m_cyclingMode = cyclingMode;
}

Vec3& PathFollower::currentPoint()
{
	return m_path[m_currentIndex];
}

Vec3& PathFollower::nextPoint()
{
	size_t nextIndex;

	if (m_currentIndex == m_path.size() - 1)
	{
		if (m_cyclingMode == CyclingMode::LOOP)
		{
			nextIndex = 0;
		}
		else
		{
			nextIndex = m_path.size() - 2;
			m_cyclingMode = CyclingMode::PING_PONG;
		}
	}
	else if (m_currentIndex == 0 && m_cyclingMode == CyclingMode::PING_PONG)
	{
		nextIndex = 1;
		m_cyclingMode = CyclingMode::LOOP;
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