#pragma once

#include "Core/Event.h"

namespace PFA::Core
{
	class EventManager
	{
	public:
		typedef std::unique_ptr<IEvent> EventPtr;
		typedef std::unordered_map<size_t, EventPtr> EventMap;

		template <typename EventType>
		IEvent::ListenerId subscribe(typename EventType::Action action);

		template <typename EventType>
		void unsubscribe(IEvent::ListenerId listener);

		template <typename EventType, typename ...Args>
		void broadcast(Args... args);

		void clear();

	private:
		EventMap m_events;
	};
}

#include "Core/EventManager.inl"
