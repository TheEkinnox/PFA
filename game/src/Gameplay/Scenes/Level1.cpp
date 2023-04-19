#include "Gameplay/Scenes/Level1.h"

#include "Core/GameContext.h"
#include "Debug/Assertion.h"
#include "LowRenderer/Camera.h"
#include "LowRenderer/Mesh.h"
#include "Resources/Model.h"
#include "Resources/Material.h"
#include "Resources/ResourcesManager.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Utility/ServiceLocator.h"
#include "Utility/utility.inl"

using namespace LibMath;
using namespace LibGL::Resources;
using namespace LibGL::Rendering;

namespace PFA::Gameplay
{
	Level1::Level1() :
		IGameScene(Vector3(25.5f, 0.f, 63.5f), Vector3(0.f, 90.f, 0.f))
	{
	}

	Level1& Level1::load()
	{
		IGameScene::load();

		placeFloor();
		placeWalls();
		placeBlocks();
		placeDoors();

		return *this;
	}

	const Shader* Level1::getLitShader(const std::string& fileName) const
	{
		auto& resourceManager = LGL_SERVICE(ResourceManager);
		Shader* shader = resourceManager.get<Shader>(fileName);

		if (shader == nullptr)
		{
			shader = resourceManager.create<Shader>(fileName);

			ASSERT(shader != nullptr);
			ASSERT(shader->setVertexShader());
			ASSERT(shader->setFragmentShader());
			ASSERT(shader->link());
		}

		return shader;
	}

	void Level1::updateLitShader(const std::string& fileName) const
	{
		const Shader* shader = getLitShader(fileName);

		if (shader == nullptr)
			return;

		shader->use();

		// Setup camera
		shader->setUniformVec3("u_viewPos", Camera::getCurrent().getGlobalTransform().getPosition());

		// Setup directional light
		m_dirLight.setupUniform("dirLight", *shader);

		// Setup point lights
		for (size_t i = 0; i < NB_POINT_LIGHTS; i++)
		{
			const std::string prefix = LibGL::Utility::formatString("pointLights[%i]", i);
			m_pointLights[i].setupUniform(prefix, *shader);
		}

		Shader::unbind();
	}

	void Level1::placeFloor()
	{
		// Load the lit shader (used by essentially all rendered entities)
		const Shader* shader = getLitShader("shaders/Lit.glsl");
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

		// Load the floor model, textures and material
		const Model* floorModel = resourceManager.getOrCreate<Model>("meshes/primitives/plane.obj");
		const Texture* floorDiffuse = resourceManager.getOrCreate<Texture>("textures/floor/floor_diffuse.png");
		const Texture* floorSpecular = resourceManager.getOrCreate<Texture>("textures/floor/floor_specular.png");
		const Texture* floorNormal = resourceManager.getOrCreate<Texture>("textures/floor/floor_normal.jpg");

		const Material floorMat
		(
			*shader,
			{ floorDiffuse, floorSpecular, floorNormal },
			{ },
			{ },
			4
		);

		Mesh floor(nullptr, *floorModel, floorMat);
		floor.setScale(Vector3(50, 1, 50));
		addNode(floor);
	}

	void Level1::placeWalls()
	{
		// Load the lit shader (used by essentially all rendered entities)
		const Shader* shader = getLitShader("shaders/Lit.glsl");
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

		// Load the wall model, textures and material
		//const Model* wallModel = resourceManager.getOrCreate<Model>("meshes/primitives/quad.obj");
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
	}

	void Level1::placeBlocks()
	{
		// Load the lit shader (used by essentially all rendered entities)
		const Shader* shader = getLitShader("shaders/Lit.glsl");
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

		// Load the block model, textures and material
		//const Model* blockModel = resourceManager.getOrCreate<Model>("meshes/primitives/cube.obj");
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
	}

	void Level1::placeDoors()
	{
		// Load the lit shader (used by essentially all rendered entities)
		const Shader* shader = getLitShader("shaders/Lit.glsl");
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

		// Load the door model, textures and materials
		//const Model* doorModel = resourceManager.getOrCreate<Model>("meshes/primitives/plane.obj");
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
	}

	void Level1::update()
	{
		// TODO (NTH): Use a ubo to not have to manually give information to a specific shader
		updateLitShader("shaders/Lit.glsl");

		IGameScene::update();
	}
}
