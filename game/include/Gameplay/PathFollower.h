#pragma once
#include "Vector/Vector3.h"
#include <vector>
#include "CyclingMode.h"

namespace PFA::Gameplay
{
	class PathFollower
	{
	public:
		using Vec3 = LibMath::Vector3;

		PathFollower(const std::vector<Vec3>& p_path, ECyclingMode p_cyclingMode);
		const			Vec3& currentPoint();
		const			Vec3& nextPoint();
		void			update();
	private:

		std::vector<Vec3>		m_path;
		size_t					m_currentIndex;
		ECyclingMode			m_cyclingMode;

	};

}