#include "Gameplay/Scenes/Level1.h"

#include "BoxCollider.h"
#include "Core/GameContext.h"
#include "Debug/Assertion.h"
#include "Gameplay/Cube.h"
#include "Gameplay/Door.h"
#include "LowRenderer/Camera.h"
#include "LowRenderer/Light.h"
#include "LowRenderer/Mesh.h"
#include "Resources/Model.h"
#include "Resources/Material.h"
#include "Resources/ResourcesManager.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Utility/ServiceLocator.h"

using namespace LibMath;
using namespace LibGL::Resources;
using namespace LibGL::Rendering;
using namespace LibGL::Physics;

namespace PFA::Gameplay
{
	Level1::Level1() :
		IGameScene(Vector3(0.f), Vector3(0.f)),
		//IGameScene(Vector3(25.5f, 0.f, 63.5f), Vector3(0.f)),
		m_lightsSSBO(EAccessSpecifier::STREAM_DRAW)
	{
	}

	Level1& Level1::load()
	{
		IGameScene::load();

		Camera::getCurrent().setClearColor(Color::skyBlue);

		placeFloor();
		placeWalls();
		placeBlocks();
		placeDoors();
		placeLights();

		return *this;
	}

	void Level1::placeFloor()
	{
		// Load the lit shader (used by essentially all rendered entities)
		const Shader* shader = getShader("shaders/Lit.glsl");
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

		// Load the floor model, textures and material
		const Model* floorModel = resourceManager.getOrCreate<Model>("meshes/primitives/plane.obj");
		const Texture* floorDiffuse = resourceManager.getOrCreate<Texture>("textures/floor/floor_diffuse.png");
		const Texture* floorSpecular = resourceManager.getOrCreate<Texture>("textures/floor/floor_specular.png");
		//const Texture* floorNormal = resourceManager.getOrCreate<Texture>("textures/floor/floor_normal.jpg");

		const Vector3 floorScale = Vector3(120, 1, 68);

		const Material floorMat
		(
			*shader,
			{ floorDiffuse, floorSpecular, nullptr },
			{ { }, { floorScale.m_x, floorScale.m_z } },
			{ },
			4
		);

		Mesh& floor = addNode<Mesh>(nullptr, *floorModel, floorMat);
		floor.setPosition(Vector3::down());
		floor.setScale(floorScale);

		const Vector3 floorColliderOffset( 0.f, -floorScale.m_y / 2.f, 0.f );
		floor.addComponent<BoxCollider>(floorColliderOffset, floorScale);
	}

	void Level1::addWall(const Transform& transform)
	{
		// Load the lit shader (used by essentially all rendered entities)
		const Shader* shader = getShader("shaders/Lit.glsl");
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

		// Load the wall model, textures and material
		const Model* wallModel = resourceManager.getOrCreate<Model>("meshes/primitives/cube.obj");
		const Texture* wallDiffuse = resourceManager.getOrCreate<Texture>("textures/wall/wall_diffuse.jpg");
		const Texture* wallSpecular = resourceManager.getOrCreate<Texture>("textures/wall/wall_specular.png");
		const Texture* wallNormal = resourceManager.getOrCreate<Texture>("textures/wall/wall_normal.jpg");

		const Material wallMat
		(
			*shader,
			{ wallDiffuse, wallSpecular, wallNormal },
			{ { }, { transform.getScale().m_x, transform.getScale().m_y } },
			{ },
			4
		);

		Mesh& sceneWall = addNode<Mesh>(nullptr, *wallModel, wallMat);
		sceneWall.setPosition(transform.getPosition());
		sceneWall.setRotation(transform.getRotation());
		sceneWall.setScale(transform.getScale());

		sceneWall.addComponent<BoxCollider>(Vector3::zero(), Vector3::one());
	}

	void Level1::placeWalls()
	{
		const Vector3 wallScale(15.f, 5.f, .25f);

		addWall({
			{ -2.f, wallScale.m_y / 2.f, 0.f },
			Vector3::zero(),
			wallScale
		});
	}

	void Level1::addBlock(const Transform& transform, const Color& color)
	{
		// Load the lit shader (used by essentially all rendered lit entities)
		const Shader* shader = getShader("shaders/Lit.glsl");
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

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
			{ color },
			16
		);

		Mesh& block = addNode<Mesh>(nullptr, *blockModel, blockMat);
		block.setPosition(transform.getPosition());
		block.setRotation(transform.getRotation());
		block.setScale(transform.getScale());

		block.addComponent<BoxCollider>(Vector3::zero(), Vector3::one());
		block.addComponent<Cube>();
	}

	void Level1::placeBlocks()
	{
		addBlock({ Vector3::one(), Vector3::zero(), Vector3::one() }, Color::blue);
	}

	void Level1::addDoor(const Transform& transform, const Color& color)
	{
		// Load the lit shader (used by essentially all rendered entities)
		const Shader* shader = getShader("shaders/Lit.glsl");
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

		// Load the door model, textures and materials
		const Model* doorModel = resourceManager.getOrCreate<Model>("meshes/primitives/cube.obj");
		const Texture* doorOpenDiffuse = resourceManager.getOrCreate<Texture>("textures/door/door_open_diffuse.png");
		const Texture* doorOpenSpecular = resourceManager.getOrCreate<Texture>("textures/door/door_open_specular.png");

		const Material doorOpenMat
		(
			*shader,
			{ doorOpenDiffuse, doorOpenSpecular, nullptr },
			{ },
			{ color },
			64
		);

		const Texture* doorClosedDiffuse = resourceManager.getOrCreate<Texture>("textures/door/door_closed_diffuse.png");
		const Texture* doorClosedSpecular = resourceManager.getOrCreate<Texture>("textures/door/door_closed_specular.png");

		const Material doorClosedMat
		(
			*shader,
			{ doorClosedDiffuse, doorClosedSpecular, nullptr },
			{ },
			{ color },
			64
		);

		Mesh& block = addNode<Mesh>(nullptr, *doorModel, doorClosedMat);
		block.setPosition(transform.getPosition());
		block.setRotation(transform.getRotation());
		block.setScale(transform.getScale());

		block.addComponent<BoxCollider>(Vector3::zero(), Vector3::one());
		block.addComponent<Cube>();

		Mesh& door = addNode<Mesh>(nullptr, *doorModel, doorClosedMat);
		door.setPosition(transform.getPosition());
		door.setRotation(transform.getRotation());
		door.setScale(transform.getScale());

		door.addComponent<BoxCollider>(Vector3::zero(), Vector3::one());
		door.addComponent<Door>(doorOpenMat, doorClosedMat);
	}

	void Level1::placeDoors()
	{
		const Vector3 doorScale(15.f, 5.f, .25f);

		addDoor({
			{ -2.f, doorScale.m_y / 2.f, 0.f },
			Vector3::zero(),
			doorScale
		}, Color::blue);
	}

	void Level1::placeLights() const
	{
		struct GLMat4
		{
			float data[16];

			GLMat4(const Matrix& mat) : data{}
			{
				memcpy(data, mat.transposed().getArray(), sizeof(float[16]));
			}
		};

		std::vector<GLMat4> lightMats;

		lightMats.emplace_back(Light(Color(.1f, .1f, .1f, 1.f)).getMatrix());

		lightMats.emplace_back(DirectionalLight(
			Color::white,
			Vector3::down()
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color::magenta,
			{ -1, 1, 1 },
			AttenuationData(16)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color::magenta,
			{ 1, 1, 1 },
			AttenuationData(16)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color::magenta,
			{ 0, 1, -1 },
			AttenuationData(16)
		).getMatrix());

		m_lightsSSBO.sendBlocks(lightMats.data(), lightMats.size() * sizeof(GLMat4));
	}

	void Level1::update()
	{
		m_lightsSSBO.bind(0);
		// TODO (NTH): Use a ubo to not have to manually give information to a specific shader
		updateShader("shaders/Lit.glsl");

		IGameScene::update();

		m_lightsSSBO.unbind();
	}
}
