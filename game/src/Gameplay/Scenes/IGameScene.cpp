#include "Gameplay/Scenes/IGameScene.h"

#include "Gameplay/CharacterController.h"
#include "BoxCollider.h"
#include "Window.h"
#include "Angle/Degree.h"
#include "Core/Renderer.h"
#include "Eventing/EventManager.h"
#include "Gameplay/EndButton.h"
#include "Gameplay/Telephone.h"
#include "LowRenderer/Mesh.h"
#include "Resources/ResourcesManager.h"
#include "Resources/Texture.h"
#include "Resources/Shader.h"
#include "Resources/Model.h"
#include "Utility/ServiceLocator.h"

#define MOVE_SPEED 2.f				// Player's move speed in game units per second
#define ROTATION_SPEED 30.f			// Player's/Camera's rotation speed in degrees per second
#define JUMP_FORCE 6.f				// Player's jump impulse force in Newton
#define CAM_HEIGHT 1.63f			// Player's "eyes" local height
#define PLAYER_SIZE 1.75f			// Player's capsule collider height

#define END_BUTTON_USE_RANGE 2.f	// The max distance the level end button is usable from

using namespace LibMath::Literal;
using namespace LibMath;
using namespace LibGL::Rendering;
using namespace LibGL::Physics;
using namespace LibGL::Resources;
using namespace LibGL::Application;
using namespace LibGL;
using namespace PFA::Events;

namespace PFA::Gameplay
{
	IGameScene::IGameScene(const Vector3& spawnPoint,
		const Vector3& spawnRotation, const Vector3& endPoint) :
		m_spawnPoint(spawnPoint), m_spawnRotation(spawnRotation),
		m_endPoint(endPoint)
	{
	}

	IGameScene::~IGameScene()
	{
		LGL_SERVICE(Window).m_resizeEvent.unsubscribe(m_resizeListenerId);
		LGL_SERVICE(EventManager).unsubscribe<LevelCompleteEvent>(m_levelCompleteListenerId);
	}

	IGameScene& IGameScene::load()
	{
		// Clear the scene in case it was already loaded
		clear();

		bindLevelCompleteListener();

		// Setup the player
		const Transform playerTransform
		{
			Vector3(0.f, PLAYER_SIZE / 2.f, 0.f),
			m_spawnRotation,
			Vector3(1.f, PLAYER_SIZE, 1.f)
		};

		Entity& player = addNode<Entity>(nullptr, playerTransform);

		player.addComponent<BoxCollider>(Vector3::zero(),
			Vector3(1.f));

		// A rigidbody is automatically added by the character controller
		player.addComponent<CharacterController>(
			Vector3(0.f, -playerTransform.getPosition().m_y / 2.f - .01f, 0.f), MOVE_SPEED,
			ROTATION_SPEED, JUMP_FORCE);

		// Setup the camera
		addCamera(player);

		player.translate(m_spawnPoint);

		addEndPoint();

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

	void IGameScene::addCamera(Entity& parent)
	{
		const auto projMat = Matrix4::perspectiveProjection(60_deg,
			LGL_SERVICE(LibGL::Application::Window).getAspect(), .3f, 1000.f);

		const Vector4 camPos = parent.getMatrix().inverse() * Vector4(0.f, CAM_HEIGHT, 0.f, 1.f);

		const Transform camTransform
		{
			camPos.xyz(),
			Vector3(0.f, 0.f, 0.f),
			Vector3::one() / parent.getScale()
		};

		Camera& cam = parent.addChild<Camera>(camTransform, projMat);

		// Set the scene cam as the current camera
		Camera::setCurrent(cam);

		// Bind the window resize function
		bindResizeListener();

		// Add the telephone and crosshair to the camera
		addPhone(cam);
		addCrosshair(cam);
	}

	void IGameScene::bindResizeListener()
	{
		if (m_resizeListenerId != 0)
			LGL_SERVICE(Window).m_resizeEvent.unsubscribe(m_resizeListenerId);

		const auto resizeFunc = [](const Window::dimensions_t size)
		{
			LGL_SERVICE(Renderer).setViewPort(0, 0, size.first, size.second);
			Camera::getCurrent().setProjectionMatrix(Matrix4::perspectiveProjection(60_deg,
				static_cast<float>(size.first) / static_cast<float>(size.second),
				.3f, 1000.f));
		};

		m_resizeListenerId = LGL_SERVICE(Window).m_resizeEvent.subscribe(resizeFunc);
	}

	void IGameScene::addPhone(Camera& camera) const
	{
		auto& resourceManager = LGL_SERVICE(ResourceManager);
		const Shader* unlitShader = getShader("shaders/Unlit.glsl");
		Model* phoneModel = resourceManager.getOrCreate<Model>("meshes/primitives/quad.obj");
		Texture* phoneTexture = resourceManager.getOrCreate<Texture>("textures/ui/telephone.png");
		phoneTexture->setMinFilter(ETextureFilter::NEAREST);
		phoneTexture->setMagFilter(ETextureFilter::NEAREST);

		Material phoneMat
		{
			*unlitShader,
			{ phoneTexture },
			{ },
			{ },
			0
		};

		Mesh& phoneEntity = camera.addChild<Mesh>(*phoneModel, phoneMat);
		phoneEntity.addComponent<Telephone>();
		phoneEntity.setPosition(Vector3(.27f, -.122f, -.31f));
		phoneEntity.setRotation(Vector3(0.f, 180.f, 0.f));
		phoneEntity.setScale(Vector3(.06f, .08f, 1.f));
	}

	void IGameScene::addCrosshair(Camera& camera) const
	{
		auto& resourceManager = LGL_SERVICE(ResourceManager);
		const Shader* unlitShader = getShader("shaders/Unlit.glsl");
		Model* crosshairModel = resourceManager.getOrCreate<Model>("meshes/primitives/quad.obj");
		Texture* crosshairTexture = resourceManager.getOrCreate<Texture>("textures/ui/crosshair.png");
		crosshairTexture->setMinFilter(ETextureFilter::NEAREST);
		crosshairTexture->setMagFilter(ETextureFilter::NEAREST);

		Material crosshairMat
		{
			*unlitShader,
			{ crosshairTexture },
			{ },
			{ },
			0
		};

		Mesh& crosshairEntity = camera.addChild<Mesh>(*crosshairModel, crosshairMat);
		crosshairEntity.setPosition(Vector3(0.f, 0.f, -.31f));
		crosshairEntity.setRotation(Vector3(0.f, 180.f, 0.f));
		crosshairEntity.setScale(Vector3(.002f, .002f, 1.f));
	}

	void IGameScene::addEndPoint()
	{
		auto& resourceManager = LGL_SERVICE(ResourceManager);
		const Shader* litShader = getShader("shaders/lit.glsl");
		Model* endPointModel = resourceManager.getOrCreate<Model>("meshes/end_button.obj");
		Texture* endPointDiffuse = resourceManager.getOrCreate<Texture>("textures/end_button/end_button_diffuse.png");
		Texture* endPointSpecular = resourceManager.getOrCreate<Texture>("textures/end_button/end_button_specular.png");

		Material endPointMat
		{
			*litShader,
			{ endPointDiffuse, endPointSpecular, nullptr },
			{ },
			{ },
			64
		};

		Mesh& endPoint = addNode<Mesh>(nullptr, *endPointModel, endPointMat);
		endPoint.addComponent<BoxCollider>(Vector3( 0.f, .1f, 0.f ), Vector3( 1.f, 1.2f, 1.f ));
		endPoint.addComponent<EndButton>(END_BUTTON_USE_RANGE);
		endPoint.setPosition(m_endPoint + Vector3(0.f, .5f, 0.f));
	}
}
