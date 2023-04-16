#pragma once
#include "IContext.h"
#include "InputManager.h"
#include "Resources/ResourcesManager.h"
#include "Core/Renderer.h"

namespace LibGL::Demo
{
	class DemoContext final : public Application::IContext
	{
	public:
		DemoContext(int windowWidth, int windowHeight, const char* title);

		void update() override;

		std::unique_ptr<Application::InputManager>	m_inputManager;
		std::unique_ptr<Resources::ResourceManager>	m_resourceManager;
		std::unique_ptr<Rendering::Renderer>		m_renderer;
		std::unique_ptr<Rendering::Camera>			m_camera;
	};
}
