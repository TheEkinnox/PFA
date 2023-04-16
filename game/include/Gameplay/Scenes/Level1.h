#pragma once

#include "Gameplay/Scenes/IGameScene.h"

namespace PFA::Gameplay
{
	class Level1 final : public IGameScene
	{
	public:
		Level1();

		/**
		 * \brief Loads the game's first level
		 * \return A reference to the scene
		 */
		Level1& load() override;

	private:
		/**
		 * \brief Places the level's floor
		 */
		void placeFloor();

		/**
		 * \brief Places the level's walls
		 */
		void placeWalls();

		/**
		 * \brief Places the level's doors
		 */
		void placeDoors();

		/**
		 * \brief Places the level's color blocks
		 */
		void placeBlocks();
	};
}
