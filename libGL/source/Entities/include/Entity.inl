#pragma once

#include "Component.h"
#include "Entity.h"

namespace LibGL
{
	template <typename T, typename ... Args>
	T& Entity::addComponent(Args&&... args)
	{
		static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

		m_components.push_back(new T(*this, args...));

		return static_cast<T&>(*m_components.back());
	}

	template <typename T>
	void Entity::removeComponent()
	{
		static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

		const T* firstComponent = getComponent<T>();

		if (nullptr != firstComponent)
			removeComponent(*firstComponent);
	}

	template <typename T>
	T* Entity::getComponent()
	{
		static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

		for (const auto& component : m_components)
		{
			if (typeid(component) == typeid(T*) || dynamic_cast<T*>(component) != nullptr)
				return dynamic_cast<T*>(component);
		}

		return nullptr;
	}

	template <typename T>
	T* Entity::getComponent(const Component::ComponentId id)
	{
		static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

		for (const auto& component : m_components)
		{
			if (id == component->getId() &&
				(typeid(component) == typeid(T*) ||
					dynamic_cast<T*>(component) != nullptr))
			{
				return dynamic_cast<T*>(component);
			}
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*> Entity::getComponents()
	{
		static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

		std::vector<T*> components;

		for (const auto& component : m_components)
		{
			if (typeid(component) == typeid(T*) || dynamic_cast<T*>(component) != nullptr)
				components.push_back(dynamic_cast<T*>(component));
		}

		return components;
	}
}
