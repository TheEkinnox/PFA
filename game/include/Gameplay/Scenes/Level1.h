#pragma once

#include "Core/Buffers/ShaderStorageBuffer.h"
#include "Gameplay/Scenes/IGameScene.h"

namespace LibGL::Rendering
{
	class Mesh;
	class Material;
	struct Color;
}

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
		LibGL::Rendering::ShaderStorageBuffer	m_lightsSSBO;

		/**
		 * \brief Adds a floor to the scene with the given transform
		 * \param transform The floor's transform
		 */
		void addFloor(const LibMath::Transform& transform);

		/**
		 * \brief Places the level's floor
		 */
		void placeFloor();

		/**
		 * \brief Adds a stair step to the scene with the given transform
		 * \param transform The stair step's transform
		 */
		void addStair(const LibMath::Transform& transform);

		/**
		 * \brief Places the level's stairs
		 */
		void placeStairs();

		/**
		 * \brief Adds a wall to the scene with the given transform
		 * \param transform The wall's transform
		 */
		void addWall(const LibMath::Transform& transform);

		/**
		 * \brief Places the level's walls
		 */
		void placeWalls();

		/**
		 * \brief Adds a block to the scene with the given transform
		 * \param transform The block's transform
		 * \param color The block's color
		 * \return A reference to the added block's mesh
		 */
		LibGL::Rendering::Mesh& addBlock(const LibMath::Transform& transform,
			const LibGL::Rendering::Color& color);

		/**
		 * \brief Places the level's color blocks
		 */
		void placeBlocks();

		/**
		 * \brief Adds a door to the scene with the given transform
		 * \param transform The door's transform
		 * \param color The door's color
		 */
		void addDoor(const LibMath::Transform& transform,
			const LibGL::Rendering::Color& color);

		/**
		 * \brief Places the level's doors
		 */
		void placeDoors();

		/**
		 * \brief Adds a window to the scene with the given transform
		 * \param transform The window's transform
		 */
		void addWindow(const LibMath::Transform& transform);

		/**
		 * \brief Places the level's windows
		 */
		void placeWindows();

		/**
		 * \brief Places the level's lights
		 */
		void placeLights() const;

		/**
		 * \brief Updates the level
		 */
		void update() override;

		void bindLevelCompleteListener() override;
	};
}
