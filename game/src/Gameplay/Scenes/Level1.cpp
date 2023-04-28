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
#include "Eventing/EventManager.h"
#include "Core/EventDefs.h"
#include "Gameplay/PathFollower.h"
#include "Angle.h"
#include "Trigonometry.h"

using namespace LibMath::Literal;
using namespace LibMath;
using namespace LibGL::Resources;
using namespace LibGL::Rendering;
using namespace LibGL::Physics;
using namespace LibGL;
using namespace PFA::Events;

#define SHADER_LIT "shaders/Lit.glsl"

namespace PFA::Gameplay
{
	Level1::Level1() :
		IGameScene(Vector3(18.f, .05f, 20.f), Vector3(0.f, 90.f, 0.f),
			Vector3(8.f, 0.f, 7.f)),
		m_lightsSSBO(EAccessSpecifier::STREAM_DRAW)
	{
	}

	Level1& Level1::load()
	{
		IGameScene::load();

		placeFloor();
		placeStairs();
		placeWalls();
		placeBlocks();
		placeDoors();
		placeWindows();
		placeLights();

		Camera::getCurrent().setClearColor(Color::skyBlue);

		LGL_SERVICE(EventManager).broadcast<SceneLoadedEvent>();

		return *this;
	}

	void Level1::addFloor(const Transform& transform)
	{
		// Load the lit shader (used by essentially all rendered entities)
		const Shader* shader = getShader(SHADER_LIT);
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

		// Load the floor model and material
		const Model* floorModel = resourceManager.getOrCreate<Model>("meshes/primitives/cube.obj");

		const Material floorMat
		(
			*shader,
			{ },
			{ },
			{ { .4f, .4f, .5f } },
			16
		);

		Mesh& floor = addNode<Mesh>(nullptr, *floorModel, floorMat);
		floor.setPosition(transform.getPosition());
		floor.setRotation(transform.getRotation());
		floor.setScale(transform.getScale());

		floor.addComponent<BoxCollider>(Vector3::zero(), Vector3::one());
	}

	void Level1::placeFloor()
	{
		// Floor 0's floor
		addFloor({
			{ 18.f, -.1f, 14.f },
			{ 0.f, 0.f, 0.f },
			{ 36.f, .1f, 26.f }
		});

#pragma region FLOOR_FLOOR_1
		addFloor({
			{ 30.f, 2.95f, 12.f },
			{ 0.f, 0.f, 0.f },
			{ 4.f, .1f, 20.f }
		});

		addFloor({
			{ 21.f, 2.95f, 8.5f },
			{ 0.f, 0.f, 0.f },
			{ 14.f, .1f, 7.f }
		});

		addFloor({
			{ 4.f, 2.95f, 7.f },
			{ 0.f, 0.f, 0.f },
			{ 4.f, .1f, 10.f }
		});

		addFloor({
			{ 20.f, 2.95f, 20.f },
			{ 0.f, 0.f, 0.f },
			{ 16.f, .1f, 12.f }
		});

		addFloor({
			{ 17.f, 5.95f, 7.f },
			{ 0.f, 0.f, 0.f },
			{ 30.f, .1f, 10.f }
		});

		addFloor({
			{ 12.f, 2.95f, 7.f },
			{ 0.f, 0.f, 0.f },
			{ 4.f, .1f, 10.f }
		});

		addFloor({
			{ 8.f, 2.95f, 10.f },
			{ 0.f, 0.f, 0.f },
			{ 4.f, .1f, 4.f }
		});

		addFloor({
			{ 8.f, 2.95f, 4.f },
			{ 0.f, 0.f, 0.f },
			{ 4.f, .1f, 4.f }
		});
#pragma endregion
	}

	void Level1::addStair(const LibMath::Transform& transform)
	{
		// Load the lit shader (used by essentially all rendered entities)
		const Shader* shader = getShader(SHADER_LIT);
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

		// Load the floor model and material
		const Model* stairModel = resourceManager.getOrCreate<Model>("meshes/primitives/cube.obj");

		const Material stairMat
		(
			*shader,
			{ },
			{ },
			{ },
			32
		);

		Mesh& stair = addNode<Mesh>(nullptr, *stairModel, stairMat);
		stair.setPosition(transform.getPosition());
		stair.setRotation(transform.getRotation());
		stair.setScale(transform.getScale());

		stair.addComponent<BoxCollider>(Vector3::zero(), Vector3::one());
	}

	void Level1::placeStairs()
	{
		addStair({
			{ 24.5f, 1/3.f - .05f, 3.5f },
			{ 0.f, 0.f, 0.f },
			{ 1.f, .1f, 3.f }
		});

		addStair({
			{ 25.5f, .95f, 3.5f },
			{ 0.f, 0.f, 0.f },
			{ 1.f, .1f, 3.f }
		});

		addStair({
			{ 26.5f, 5/3.f - .05f, 3.5f },
			{ 0.f, 0.f, 0.f },
			{ 1.f, .1f, 3.f }
		});

		addStair({
			{ 27.5f, 7/3.f - .05f, 3.5f },
			{ 0.f, 0.f, 0.f },
			{ 1.f, .1f, 3.f }
		});
	}

	void Level1::addWall(const Transform& transform)
	{
		// Load the lit shader (used by essentially all rendered entities)
		const Shader* shader = getShader(SHADER_LIT);
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

		// Load the wall model and material
		const Model* wallModel = resourceManager.getOrCreate<Model>("meshes/primitives/cube.obj");

		const Material wallMat
		(
			*shader,
			{ },
			{ },
			{ { .4f, .4f, .5f } },
			32
		);

		Mesh& sceneWall = addNode<Mesh>(nullptr, *wallModel, wallMat);
		sceneWall.setPosition(transform.getPosition());
		sceneWall.setRotation(transform.getRotation());
		sceneWall.setScale(transform.getScale());

		sceneWall.addComponent<BoxCollider>(Vector3::zero(), Vector3::one());
	}

	void Level1::addWindow(const Transform& transform)
	{
		// Load the lit shader (used by essentially all rendered entities)
		const Shader* shader = getShader(SHADER_LIT);
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

		// Load the window model and material
		const Model* windowModel = resourceManager.getOrCreate<Model>("meshes/primitives/cube.obj");

		const Material windowMat
		(
			*shader,
			{ },
			{ },
			{ { .8f, 1.f, 1.f, .5f } },
			64
		);

		Mesh& sceneWindow = addNode<Mesh>(nullptr, *windowModel, windowMat);
		sceneWindow.setPosition(transform.getPosition());
		sceneWindow.setRotation(transform.getRotation());
		sceneWindow.setScale(transform.getScale());

		sceneWindow.addComponent<BoxCollider>(Vector3::zero(), Vector3::one());
	}

	void Level1::placeWalls()
	{
#pragma region WALLS_FLOOR_0
		addWall({
			{ 1.95f, 1.5f, 7.f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 3.f, 10.2f }
		});

		addWall({
			{ 17.f, 1.5f, 1.95f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 3.f, 30.f }
		});

		addWall({
			{ 15.f, 1.5f, 12.05f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 3.f, 26.f }
		});

		addWall({
			{ 32.05f, 1.5f, 10.7f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 3.f, 17.6f }
		});

		addWall({
			{ 27.95f, 1.5f, 15.f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 3.f, 5.8f }
		});

		addWall({
			{ 26.f, 1.5f, 17.95f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 3.f, 4.f }
		});

		addWall({
			{ 24.05f, 1.5f, 16.f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 3.f, 3.8f }
		});

		addWall({
			{ 18.f, 1.5f, 14.05f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 3.f, 12.2f }
		});

		addWall({
			{ 11.95f, 1.5f, 20.05f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 3.f, 11.9f }
		});

		addWall({
			{ 18.f, 1.5f, 26.05f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 3.f, 12.2f }
		});

		addWall({
			{ 24.05f, 1.5f, 24.05f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 3.f, 3.9f }
		});

		addWall({
			{ 28.f, 1.5f, 22.05f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 3.f, 8.f }
		});

		addWall({
			{ 32.05f, 1.5f, 21.3f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 3.f, 1.6f }
		});

		addWall({
			{ 32.05f, .5f, 20.f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 1.f, 1.f }
		});

		addWall({
			{ 32.05f, 2.5f, 20.f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 1.f, 1.f }
		});
#pragma endregion

#pragma region WALLS_FLOOR_1
		addWall({
			{ 32.05f, 4.5f, 6.f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 3.f, 8.2f }
		});

		addWall({
			{ 24.05f, 4.5f, 10.05f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 3.f, 16.1f }
		});

		addWall({
			{ 1.95f, 4.5f, 7.f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 3.f, 10.2f }
		});

		addWall({
			{ 8.f, 4.5f, 12.05f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 3.f, 12.f }
		});

		addWall({
			{ 15.f, 4.5f, 10.5f },
			{ 0.f, 0.f, 0.f },
			{ 2.f, 3.f, 3.2f }
		});

		addWall({
			{ 14.5f, 4.45f, 4.5f },
			{ 0.f, 0.f, 0.f },
			{ 1.f, 3.1f, 1.f }
		});

		addWall({
			{ 14.05f, 4.45f, 2.45f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 3.1f, 1.1f }
		});

		addWall({
			{ 17.f, 4.5f, 1.95f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 3.f, 30.f }
		});

		addWall({
			{ 15.05f, 4.f, 2.5f },
			{ 0.f, 90.f, 0.f },
			{ 1.f, 2.2f, 1.9f }
		});

		addWall({
			{ 26.f, 4.45f, 4.95f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 3.1f, 4.f }
		});

		addWall({
			{ 8.f, 4.5f, 12.05f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 3.f, 12.f }
		});

		addWall({
			{ 20.f, 4.45f, 3.5f },
			{ 0.f, 0.f, 0.f },
			{ 8.f, 3.1f, 3.f }
		});

		addWall({
			{ 15.5f, 3.45f, 4.5f },
			{ 0.f, 0.f, 0.f },
			{ 1.f, 1.1f, 1.f }
		});

		addWall({
			{ 14.5f, 3.45f, 3.5f },
			{ 0.f, 0.f, 0.f },
			{ 1.f, 1.1f, 1.f }
		});

		addWall({
			{ 14.5f, 5.5f, 3.5f },
			{ 0.f, 0.f, 0.f },
			{ 1.f, 1.f, 1.f }
		});

		addWall({
			{ 15.5f, 5.5f, 4.5f },
			{ 0.f, 0.f, 0.f },
			{ 1.f, 1.f, 1.f }
		});

		addWall({
			{ 20.05f, 3.5f, 9.45f },
			{ 0.f, 0.f, 0.f },
			{ 8.1f, 1.f, 1.1f }
		});

		addWall({
			{ 20.05f, 5.5f, 9.45f },
			{ 0.f, 0.f, 0.f },
			{ 8.1f, 1.f, 1.1f }
		});
#pragma endregion
	}

	Mesh& Level1::addBlock(const Transform& transform, const Color& color)
	{
		// Load the lit shader (used by essentially all rendered lit entities)
		const Shader* shader = getShader(SHADER_LIT);
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
			256
		);

		Mesh& block = addNode<Mesh>(nullptr, *blockModel, blockMat);
		block.setPosition(transform.getPosition());
		block.setRotation(transform.getRotation());
		block.setScale(transform.getScale());

		block.addComponent<BoxCollider>(Vector3::zero(), Vector3::one());
		block.addComponent<Cube>();

		return block;
	}

	void Level1::placeBlocks()
	{
		addBlock({
			{  32.5f, 1.5f, 20.f },
			Vector3::zero(),
			Vector3::one()
		}, Color::red);

		addBlock({
			{  15.5f, 4.5f, 3.5f },
			Vector3::zero(),
			Vector3::one()
		}, Color::green);

		Mesh& movingBlock = addBlock({
			{  16.5f, 4.5f, -9.5f },
			Vector3::zero(),
			Vector3::one()
		}, Color::blue);

		std::vector<Vector3> path;
		path.emplace_back(23.5f, 4.5f, 9.5f);
		path.emplace_back(16.5f, 4.5f, 9.5f);

		movingBlock.addComponent<PathFollower>(path, ECyclingMode::PING_PONG, 1.f);
	}

	void Level1::addDoor(const Transform& transform, const Color& color)
	{
		// Load the lit shader (used by essentially all rendered entities)
		const Shader* shader = getShader(SHADER_LIT);
		ASSERT(shader != nullptr);

		// Get the resource manager
		auto& resourceManager = LGL_SERVICE(ResourceManager);

		// Load the door model, textures and materials
		const Model* doorModel = resourceManager.getOrCreate<Model>("meshes/primitives/cube.obj");
		const Texture* doorOpenDiffuse = resourceManager.getOrCreate<Texture>("textures/door/door_open_diffuse.png");

		const auto doorScale = transform.getScale();

		const Material doorOpenMat
		(
			*shader,
			{ doorOpenDiffuse, nullptr, nullptr },
			{ { }, { doorScale.m_z, doorScale.m_y }},
			{ color },
			64
		);

		const Texture* doorClosedDiffuse = resourceManager.getOrCreate<Texture>("textures/door/door_closed_diffuse.png");

		const Material doorClosedMat
		(
			*shader,
			{ doorClosedDiffuse, nullptr, nullptr },
			{ { }, { doorScale.m_z, doorScale.m_y } },
			{ color },
			64
		);

		Mesh& door = addNode<Mesh>(nullptr, *doorModel, doorClosedMat);
		door.setPosition(transform.getPosition());
		door.setRotation(transform.getRotation());
		door.setScale(transform.getScale());

		door.addComponent<BoxCollider>(Vector3::zero(), Vector3::one());
		door.addComponent<Door>(doorOpenMat, doorClosedMat);
	}

	void Level1::placeDoors()
	{
		addDoor({
			{ 24.05f, 1.45f, 20.f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 2.9f, 4.f }
		}, Color::white);

		addDoor({
			{ 30.f, 1.45f, 16.f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 2.9f, 4.f }
		}, Color::red);

		addDoor({
			{ 14.05f, 4.45f, 6.95f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 2.9f, 3.9f }
		}, Color::blue);

		addDoor({
			{ 18.05f, 4.45f, 6.95f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 2.9f, 3.9f }
		}, Color::red);

		addDoor({
			{ 22.05f, 4.45f, 6.95f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 2.9f, 3.9f }
		}, Color::green);

		addDoor({
			{ 8.f, 2.95f, 7.f },
			{ 0.f, 0.f, 90.f },
			{ .1f, 4.f, 2.f }
		}, Color::red);
	}

	void Level1::placeWindows()
	{
		// Floor 0's window
		addWindow({
			{ 14.05f, 1.45f, 7.f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 2.9f, 10.f }
			});

#pragma region WINDOWS_FLOOR_1
		addWindow({
			{ 24.05f, 4.5f, 9.45f },
			{ 0.f, 0.f, 0.f },
			{ .1f, 1.f, 1.1f }
			});

		addWindow({
			{ 21.7f, 4.5f, 8.95f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 1.f, 2.5f }
			});

		addWindow({
			{ 18.25f, 4.5f, 8.95f },
			{ 0.f, 90.f, 0.f },
			{ .1f, 1.f, 2.5f }
			});
#pragma endregion
	}

	void Level1::placeLights() const
	{
		struct GLMat4
		{
			float m_data[16];

			GLMat4(const Matrix& mat) : m_data{}
			{
				memcpy(m_data, mat.transposed().getArray(), sizeof(float[16]));
			}
		};

		std::vector<GLMat4> lightMats;

		// Ambient light
		lightMats.emplace_back(Light(Color(.2f, .2f, .2f, 1.f)).getMatrix());
		
#pragma region SPOT_LIGHTS
		lightMats.emplace_back(SpotLight(
			Color( .698f, .945f, .698f, 1.75f ),
			{ -2, 3, 12 },
			{ .7077f, -.3827f, .5938f },
			AttenuationData(10),
			{ cos(0_deg), cos(30_deg)}
		).getMatrix());

		lightMats.emplace_back(SpotLight(
			Color( .698f, .945f, .698f, 1.75f ),
			{ 2, 3, 2 },
			{ .7077f, -.3827f, -.5939f },
			AttenuationData(10),
			{ cos(0_deg), cos(30_deg)}
		).getMatrix());

		lightMats.emplace_back(SpotLight(
			Color( .698f, .945f, .698f, 1.75f ),
			{ 14, 3, 2 },
			{ -.7077f, -.3827f, .5939f },
			AttenuationData(10),
			{ cos(0_deg), cos(30_deg)}
		).getMatrix());

		lightMats.emplace_back(SpotLight(
			Color( .698f, .945f, .698f, 1.75f ),
			{ 14, 3, 12 },
			{ -.7077f, -.3827f, -.5938f },
			AttenuationData(10),
			{ cos(0_deg), cos(30_deg)}
		).getMatrix());

		lightMats.emplace_back(SpotLight(
			Color( .698f, .945f, .698f, 1.75f ),
			{ -2, 6, 12 },
			{ .7077f, -.3827f, .5938f },
			AttenuationData(10),
			{ cos(0_deg), cos(30_deg)}
		).getMatrix());

		lightMats.emplace_back(SpotLight(
			Color( .698f, .945f, .698f, 1.75f ),
			{ 2, 6, 2 },
			{ .7077f, -.3827f, -.5939f },
			AttenuationData(10),
			{ cos(0_deg), cos(30_deg)}
		).getMatrix());

		lightMats.emplace_back(SpotLight(
			Color( .698f, .945f, .698f, 1.75f ),
			{ 14, 6, 2 },
			{ -.7077f, -.3827f, .5939f },
			AttenuationData(10),
			{ cos(0_deg), cos(30_deg)}
		).getMatrix());

		lightMats.emplace_back(SpotLight(
			Color( .698f, .945f, .698f, 1.75f ),
			{ 14, 6, 12 },
			{ -.7077f, -.3827f, -.5938f },
			AttenuationData(10),
			{ cos(0_deg), cos(30_deg)}
		).getMatrix());
#pragma endregion

#pragma region POINT_LIGHTS_FLOOR_0
		lightMats.emplace_back(PointLight(
			Color( .945f, .945f, .945f ),
			{ 18.f, 2.25f, 20.f },
			AttenuationData(9)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .945f, .698f, .698f ),
			{ 30.f, 2.25f, 20.f },
			AttenuationData(7)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f ),
			{ 30.5f, 2.25f, 7.f },
			AttenuationData(7)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f ),
			{ 30.5f, 2.25f, 3.5f },
			AttenuationData(7)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f ),
			{ 15.5f, 2.25f, 3.5f },
			AttenuationData(7)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f ),
			{ 15.5f, 2.25f, 7.f },
			AttenuationData(7)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f ),
			{ 30.5f, 2.25f, 10.5f },
			AttenuationData(7)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f ),
			{ 15.5f, 2.25f, 10.5f },
			AttenuationData(7)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f ),
			{ 23.f, 2.25f, 3.5f },
			AttenuationData(7)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f ),
			{ 23.f, 2.25f, 7.f },
			AttenuationData(7)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f ),
			{ 23.f, 2.25f, 10.5f },
			AttenuationData(7)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f, 1.f ),
			{ 8.f, 5.25f, 7.f },
			AttenuationData(9)
		).getMatrix());
#pragma endregion

#pragma region POINT_LIGHTS_FLOOR_1
		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f, 1.75f ),
			{ 30.5f, 5.25f, 7.f },
			AttenuationData(4)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f, 1.75f ),
			{ 30.5f, 5.25f, 3.5f },
			AttenuationData(4)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f, 1.75f ),
			{ 15.5f, 5.25f, 7.f },
			AttenuationData(4)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f, 1.75f ),
			{ 20.f, 5.25f, 7.f },
			AttenuationData(4)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f, 1.4f ),
			{ 25.f, 5.25f, 7.f },
			AttenuationData(5)
		).getMatrix());

		lightMats.emplace_back(PointLight(
			Color( .698f, .945f, .698f, 1.f ),
			{ 8.f, 5.25f, 7.f },
			AttenuationData(9)
		).getMatrix());
#pragma endregion

		m_lightsSSBO.sendBlocks(lightMats.data(), lightMats.size() * sizeof(GLMat4));
	}

	void Level1::update()
	{
		m_lightsSSBO.bind(0);
		// TODO (NTH): Use a ubo to not have to manually give information to a specific shader
		updateShader(SHADER_LIT);

		IGameScene::update();
	}

	void Level1::bindLevelCompleteListener()
	{
		if (m_levelCompleteListenerId != 0)
			return;

		const auto levelCompleteFunc = []
		{
			LGL_SERVICE(EventManager).broadcast<RestartEvent>();
		};

		m_levelCompleteListenerId = LGL_SERVICE(EventManager).subscribe<LevelCompleteEvent>(levelCompleteFunc);
	}
}
