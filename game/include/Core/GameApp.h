#pragma once
#include "IApplication.h"

namespace PFA::Core
{
	class GameApp final : public LibGL::Application::IApplication
	{
	public:
		GameApp();

	private:
		void onStart() override;

		void onUpdate() override;

		void onStop() override;
	};
}
