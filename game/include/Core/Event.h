#pragma once

#include <functional>

namespace PFA::Core
{
	/**
	 * \brief Non-templated part of an Event
	 */
	class IEvent
	{
	public:
		typedef uint64_t ListenerId;

		virtual ~IEvent() = default;

		virtual void unsubscribe(ListenerId listener) = 0;
		virtual size_t subscribersCount() = 0;
		virtual void clear() = 0;

	protected:
		inline static ListenerId m_currentId = 1;
	};

	template <class ... ArgTypes>
	class Event : public IEvent
	{
	public:
		typedef std::function<void(ArgTypes...)> Action;
		typedef std::unordered_map<ListenerId, Action> ActionMap;

		/**
		 * \brief Subscribes an action to the event and returns it's ListenerId
		 * \param action The action to perform when the event is invoked
		 * \return The listener id of the subscribed action
		 */
		ListenerId subscribe(Action action);

		/**
		 * \brief Unsubscribes the listener with the given id from this event
		 * \param listener The id of the listener to unsubscribe
		 */
		void unsubscribe(ListenerId listener) override;

		/**
		 * \brief Gets the current number of subscribed actions
		 * \return The number of subscribed actions
		 */
		size_t subscribersCount() override;

		/**
		 * \brief Invokes all actions linked to this event
		 * \param args The arguments to pass when calling the actions
		 */
		void invoke(ArgTypes... args);

		/**
		 * \brief Unsubscribes all listeners from this event
		 */
		void clear() override;

	private:
		ActionMap	m_actions;
	};
}

#include "Core/Event.inl"
