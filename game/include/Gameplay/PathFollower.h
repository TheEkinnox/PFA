#pragma once
#include "Vector/Vector3.h"
#include <vector>
#include "CyclingMode.h"
#include "Component.h"

namespace PFA::Gameplay
{
	class PathFollower: public LibGL::Component
	{
	public:
		using Vec3 = LibMath::Vector3;

		PathFollower(LibGL::Entity& p_owner, const std::vector<Vec3>& p_path, ECyclingMode p_cyclingMode, float p_moveSpeed);
		Vec3				currentPoint()const;
		Vec3				nextPoint()const;
		
		void				update() override;
	private:

		std::vector<Vec3>	m_path;
		size_t				m_currentIndex;
		size_t				m_nextIndex;
		ECyclingMode		m_cyclingMode;
		float				m_moveSpeed;
		void				computeNext();
	};

}