#pragma once

#include "Core/GameContext.h"

namespace PFA::Core
{
	template <typename SceneT>
	void GameContext::loadScene()
	{
		using namespace Gameplay;

		static_assert(std::is_same_v<IGameScene, SceneT> || std::is_base_of_v<IGameScene, SceneT>);

		m_scene = std::make_unique<SceneT>();
		m_scene->load();
	}
}
