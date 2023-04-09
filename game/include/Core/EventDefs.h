#pragma once
#include "Core/Event.h"

namespace PFA::Events
{
	/**
	 * \brief An event indicating the application should close
	 */
	class ExitEvent final : public Core::Event<> {};
}
