#pragma once
#include "Vector/Vector3.h"
#include <vector>

using Vec3 = LibMath::Vector3;
class CyclingMode;
namespace PFA::Gameplay
{
	enum CyclingMode
	{
		LOOP,
		PING_PONG
	};

	class PathFollower
	{
	public:

		PathFollower(const std::vector<Vec3>& path, CyclingMode cyclingMode);
		Vec3&			currentPoint();
		Vec3&			nextPoint();
		void			update();
	private:

		std::vector<Vec3>		m_path;
		size_t					m_currentIndex;
		CyclingMode				m_cyclingMode;

	};

}