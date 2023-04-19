#pragma once

#include "Gameplay/Scenes/IGameScene.h"
#include "LowRenderer/Light.h"
#include "Resources/Shader.h"

// TODO (NTH): use an ssbo for the lights
#define NB_POINT_LIGHTS 8

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
		LibGL::Rendering::DirectionalLight	m_dirLight;
		LibGL::Rendering::PointLight		m_pointLights[NB_POINT_LIGHTS];

		/**
		 * \brief Loads the shader with the given fileName
		 * \param fileName The path of the lit shader to get
		 * \return A pointer to the loaded lit shader on success, nullptr otherwise
		 */
		const LibGL::Resources::Shader* getLitShader(const std::string& fileName) const;

		/**
		 * \brief Update the lighting data for the shader with the given file name
		 * \param fileName The path of the lit shader to update
		 */
		void updateLitShader(const std::string& fileName) const;

		/**
		 * \brief Places the level's floor
		 */
		void placeFloor();

		/**
		 * \brief Places the level's walls
		 */
		void placeWalls();

		/**
		 * \brief Places the level's color blocks
		 */
		void placeBlocks();

		/**
		 * \brief Places the level's doors
		 */
		void placeDoors();

		/**
		 * \brief Updates the level
		 */
		void update() override;
	};
}
