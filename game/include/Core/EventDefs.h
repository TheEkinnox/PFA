#pragma once
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
}
