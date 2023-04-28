#pragma once
#include "Eventing/Event.h"
#include <ranges>

namespace LibGL
{
	template <class ... ArgTypes>
	IEvent::ListenerId Event<ArgTypes...>::subscribe(Action action)
	{
		m_actions[m_currentId] = action;
		return m_currentId++;
	}

	template <class ... ArgTypes>
	void Event<ArgTypes...>::unsubscribe(ListenerId& listenerId)
	{
		if (m_actions.contains(listenerId))
			m_actions.erase(listenerId);

		listenerId = 0;
	}

	template <class ... ArgTypes>
	size_t Event<ArgTypes...>::subscribersCount() const
	{
		return m_actions.size();
	}

	template <class ... ArgTypes>
	void Event<ArgTypes...>::invoke(ArgTypes... args) const
	{
		for (auto& action : m_actions | std::views::values)
			action(args...);
	}

	template <class ... ArgTypes>
	void Event<ArgTypes...>::clear()
	{
		m_actions.clear();
	}
}
