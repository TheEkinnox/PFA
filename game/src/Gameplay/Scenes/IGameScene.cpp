#include "Gameplay/Scenes/IGameScene.h"

#include "Gameplay/CharacterController.h"
#include "BoxCollider.h"
#include "LowRenderer/Camera.h"
#include "Window.h"
#include "Angle/Degree.h"
#include "Gameplay/Telephone.h"
#include "LowRenderer/Mesh.h"
#include "Resources/ResourcesManager.h"
#include "Resources/Texture.h"
#include "Resources/Shader.h"
#include "Resources/Model.h"
#include "Utility/ServiceLocator.h"

#define MOVE_SPEED 2.f		// Player's move speed in game units per second
#define ROTATION_SPEED 30.f	// Player's/Camera's rotation speed in degrees per second
#define JUMP_FORCE 6.f		// Player's jump impulse force in Newton
#define CAM_HEIGHT 1.63f	// Player's "eyes" local height
#define PLAYER_SIZE 1.75f	// Player's capsule collider height

using namespace LibMath;
using namespace LibMath::Literal;
using namespace LibGL;
using namespace LibGL::Rendering;
using namespace LibGL::Physics;
using namespace LibGL::Resources;

namespace PFA::Gameplay
{
	IGameScene::IGameScene(const Vector3& spawnPoint,
		const Vector3& spawnRotation) : m_spawnPoint(spawnPoint),
		m_spawnRotation(spawnRotation)
	{
	}

	IGameScene& IGameScene::load()
	{
		// Clear the scene in case it was already loaded
		clear();

		// Setup the player
		const Transform playerTransform
		{
			m_spawnPoint + Vector3(0.f, PLAYER_SIZE / 2.f, 0.f),
			m_spawnRotation,
			Vector3(1.f, PLAYER_SIZE, 1.f)
		};

		Entity& player = addNode<Entity>(nullptr, playerTransform);

		player.addComponent<BoxCollider>(Vector3::zero(),
			Vector3(1.f));

		// A rigidbody is automatically added by the character controller
		player.addComponent<CharacterController>(
			Vector3(0.f, -PLAYER_SIZE / 2.f - .01f, 0.f), MOVE_SPEED,
			ROTATION_SPEED, JUMP_FORCE);

		// Setup the camera
		const auto projMat = Matrix4::perspectiveProjection(90_deg,
			LGL_SERVICE(LibGL::Application::Window).getAspect(), .1f, 150.f);

		const Transform camTransform
		{
			Vector3(0.f, CAM_HEIGHT - PLAYER_SIZE / 2.f, 0.f),
			Vector3::zero(),
			Vector3::one() / player.getScale()
		};

		Camera& cam = player.addChild<Camera>(camTransform, projMat);

		// Set the scene cam as the current camera
		Camera::setCurrent(cam);

		// Add the telephone to the camera
		auto& resourceManager = LGL_SERVICE(ResourceManager);
		const Shader* unlitShader = getShader("shaders/Unlit.glsl");
		Model* phoneModel = resourceManager.getOrCreate<Model>("meshes/primitives/cube.obj");

		Material phoneMat
		{
			*unlitShader,
			{ },
			{ },
			{ },
			0
		};

		Mesh& phoneEntity = cam.addChild<Mesh>(*phoneModel, phoneMat);
		phoneEntity.addComponent<Telephone>();
		phoneEntity.setScale(Vector3(.1f, .1f, .1f));
		//phoneEntity.setPosition(Vector3(.5f, -.5f, .1f));

		return *this;
	}

	const Shader* IGameScene::getShader(const std::string& fileName)
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

	void IGameScene::updateShader(const std::string& fileName)
	{
		const Shader* shader = getShader(fileName);

		if (shader == nullptr)
			return;

		shader->use();

		// Setup camera
		shader->setUniformVec3("u_viewPos", Camera::getCurrent().getGlobalTransform().getPosition());

		Shader::unbind();
	}
}
