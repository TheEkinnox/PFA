#pragma once
#include "Core/Color.h"
#include "Eventing/Event.h"

namespace PFA::Events
{
	/**
	 * \brief An event indicating the application should close
	 */
	class ExitEvent final : public LibGL::Event<> {};

	/**
	 * \brief An event indicating the application should restart
	 */
	class RestartEvent final : public LibGL::Event<> {};

	/**
	 * \brief An event indicating the phone's color changed
	 */
	class ColorChangedEvent final : public LibGL::Event<LibGL::Rendering::Color> {};

	/**
	 * \brief An event indicating the current level was completed
	 */
	class LevelCompleteEvent final : public LibGL::Event<> {};
}
