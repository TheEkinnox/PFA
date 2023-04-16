#include "Core/GameContext.h"
#include "Core/EventDefs.h"

#include "Debug/Log.h"
#include "Gameplay/CharacterController.h"
#include "Gameplay/Scenes/Level1.h"
#include "LowRenderer/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Utility/ServiceLocator.h"

// TODO (NTH) : Load config from a file
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define APP_TITLE "PFA - LAN (Esc. to close)"

using namespace LibGL;
using namespace LibMath;
using namespace LibGL::Utility;
using namespace LibGL::Application;
using namespace LibGL::Resources;
using namespace LibGL::Rendering;

using namespace PFA::Gameplay;

namespace PFA::Core
{
	GameContext::GameContext() :
		IContext(WINDOW_WIDTH, WINDOW_HEIGHT, APP_TITLE),
		m_renderer(std::make_unique<Renderer>()),
		m_inputManager(std::make_unique<InputManager>(*m_window)),
		m_resourcesManager(std::make_unique<ResourceManager>()),
		m_eventManager(std::make_unique<EventManager>()),
		m_scene(std::make_unique<Level1>())
	{
		Debug::Log::openFile("console.log");

		ServiceLocator::provide<Renderer>(*m_renderer);
		ServiceLocator::provide<InputManager>(*m_inputManager);
		ServiceLocator::provide<ResourceManager>(*m_resourcesManager);
		ServiceLocator::provide<EventManager>(*m_eventManager);

		bindExitFunc();
	}

	GameContext::~GameContext()
	{
		LGL_SERVICE(EventManager).unsubscribe<Events::ExitEvent>(m_exitListenerId);
	}

	void GameContext::update()
	{
		IContext::update();
		m_renderer->clear(Camera::getCurrent());

		// TODO (NTH): Use a ubo to not have to manually give information to a specific shader
		updateLitShader("shaders/Lit.glsl");

		m_scene->update();
	}

	void GameContext::bindExitFunc()
	{
		const auto exitFunc = [&]
		{
			m_inputManager->clearStates();
			m_window->setShouldClose(true);
		};

		m_exitListenerId = LGL_SERVICE(EventManager).subscribe<Events::ExitEvent>(exitFunc);
	}

	void GameContext::bindRestartFunc()
	{
		const auto restartFunc = [&]
		{
			loadScene<Level1>();
		};

		m_restartListenerId = LGL_SERVICE(EventManager).subscribe<Events::RestartEvent>(restartFunc);
	}

	const Shader* GameContext::getLitShader(const std::string& fileName) const
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

	void GameContext::updateLitShader(const std::string& fileName) const
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
			const std::string prefix = formatString("pointLights[%i]", i);
			m_pointLights[i].setupUniform(prefix, *shader);
		}

		Shader::unbind();
	}
}
