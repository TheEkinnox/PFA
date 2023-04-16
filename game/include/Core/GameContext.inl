#pragma once

#include "Core/GameContext.h"
#include "Debug/Assertion.h"
#include "Utility/ServiceLocator.h"

namespace PFA::Core
{
	template <typename SceneT>
	void GameContext::loadScene() const
	{
		using namespace LibMath;
		using namespace Gameplay;

		static_assert(std::is_same_v<IGameScene, SceneT> || std::is_base_of_v<IGameScene, SceneT>);

		m_scene = std::make_unique<SceneT>().load();

		// Load the lit shader (used by essentially all rendered entities)
		const Shader* shader = getLitShader("shaders/Lit.glsl");
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

		// Load the floor model, textures and material
		const Model* floorModel = resourceManager.getOrCreate<Model>("meshes/primitives/plane.obj");
		const Texture* floorDiffuse = resourceManager.getOrCreate<Texture>("textures/floor/floor_diffuse.png");
		const Texture* floorSpecular = resourceManager.getOrCreate<Texture>("textures/floor/floor_specular.png");
		const Texture* floorNormal = resourceManager.getOrCreate<Texture>("textures/floor/floor_normal.png");

		const Material floorMat
		(
			*shader,
			{ floorDiffuse, floorSpecular, floorNormal },
			{ },
			{ },
			4
		);

		// Load the wall model, textures and material
		const Model* wallModel = resourceManager.getOrCreate<Model>("meshes/primitives/quad.obj");
		const Texture* wallDiffuse = resourceManager.getOrCreate<Texture>("textures/wall/wall_diffuse.jpg");
		const Texture* wallSpecular = resourceManager.getOrCreate<Texture>("textures/wall/wall_specular.png");
		const Texture* wallNormal = resourceManager.getOrCreate<Texture>("textures/wall/wall_normal.jpg");

		const Material wallMat
		(
			*shader,
			{ wallDiffuse, wallSpecular, wallNormal },
			{ },
			{ },
			4
		);

		// Load the block model, textures and material
		const Model* blockModel = resourceManager.getOrCreate<Model>("meshes/primitives/cube.obj");
		const Texture* blockDiffuse = resourceManager.getOrCreate<Texture>("textures/block/block_diffuse.png");
		const Texture* blockSpecular = resourceManager.getOrCreate<Texture>("textures/block/block_specular.png");
		const Texture* blockNormal = resourceManager.getOrCreate<Texture>("textures/block/block_normal.png");

		const Material blockMat
		(
			*shader,
			{ blockDiffuse, blockSpecular, blockNormal },
			{ },
			{ },
			16
		);

		// Load the door model, textures and materials
		const Model* doorModel = resourceManager.getOrCreate<Model>("meshes/primitives/plane.obj");
		const Texture* doorOpenDiffuse = resourceManager.getOrCreate<Texture>("textures/door/door_open_diffuse.png");
		const Texture* doorOpenSpecular = resourceManager.getOrCreate<Texture>("textures/door/door_open_specular.png");

		const Material doorOpenMat
		(
			*shader,
			{ doorOpenDiffuse, doorOpenSpecular, nullptr },
			{ },
			{ },
			64
		);

		const Texture* doorClosedDiffuse = resourceManager.getOrCreate<Texture>("textures/door/door_closed_diffuse.png");
		const Texture* doorClosedSpecular = resourceManager.getOrCreate<Texture>("textures/door/door_closed_specular.png");

		const Material doorClosedMat
		(
			*shader,
			{ doorClosedDiffuse, doorClosedSpecular, nullptr },
			{ },
			{ },
			64
		);

		// Place floor
		Mesh floor(nullptr, *floorModel, floorMat);
		floor.setScale(Vector3(50, 1, 50));
		m_scene->addNode(floor);

		// TODO: Place walls

		// TODO: Place blocks

	}
}
