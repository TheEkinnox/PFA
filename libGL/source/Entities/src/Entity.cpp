#include "Entity.h"

#include "Debug/Assertion.h"
#include "Vector/Vector4.h"

using namespace LibMath;

namespace LibGL
{
	Entity::Entity(Node* parent, const Transform& transform) :
		Node(parent), Transform(transform)
	{
		ASSERT(parent == nullptr || typeid(parent) == typeid(Entity*) ||
			dynamic_cast<Entity*>(parent) != nullptr);

		updateGlobalTransform();
	}

	Entity::Entity(const Entity& other) :
		Node(other), Transform(other),
		m_components(other.m_components)
	{
		if (!m_components.empty())
			for (const auto& component : m_components)
				component->m_owner = *this;

		updateGlobalTransform();
	}

	Entity::Entity(Entity&& other) noexcept :
		Node(std::move(other)), Transform(std::move(other)),
		m_components(std::move(other.m_components))
	{
		if (!m_components.empty())
			for (const auto& component : m_components)
				component->m_owner = *this;

		updateGlobalTransform();
	}

	Entity::~Entity()
	{
		if (!m_components.empty())
		{
			// Components remove themselves from the list on destruction.
			// Can't for / foreach (would overflow)

			while (!m_components.empty())
				delete m_components[0];
		}
	}

	Entity& Entity::operator=(const Entity& other)
	{
		if (&other == this)
			return *this;

		Node::operator=(other);
		Transform::operator=(other);

		m_components.clear();

		m_components = other.m_components;

		if (!m_components.empty())
			for (const auto& component : m_components)
				component->m_owner = *this;

		updateGlobalTransform();

		return *this;
	}

	Entity& Entity::operator=(Entity&& other) noexcept
	{
		if (&other == this)
			return *this;

		Node::operator=(other);
		Transform::operator=(other);

		m_components.clear();

		m_components = std::move(other.m_components);
		m_globalTransform = std::move(other.m_globalTransform);

		if (!m_components.empty())
			for (const auto& component : m_components)
				component->m_owner = *this;

		updateGlobalTransform();

		other.m_components.clear();

		return *this;
	}

	LibMath::Transform Entity::getGlobalTransform() const
	{
		return m_globalTransform;
	}


	void Entity::removeComponent(const Component& component)
	{
		if (m_components.empty())
			return;

		const auto findFunc = [component](const ComponentPtr& ptr)
		{
			return *ptr == component;
		};

		const auto iter = std::ranges::find_if(m_components, findFunc);

		if (iter != m_components.end())
		{
			delete* iter;
			m_components.erase(iter);
		}
	}

	void Entity::removeComponent(const Component::ComponentId id)
	{
		if (m_components.empty())
			return;

		const auto findFunc = [id](const ComponentPtr& ptr)
		{
			return ptr->getId() == id;
		};

		std::erase_if(m_components, findFunc);
	}

	void Entity::update()
	{
		for (const auto& component : m_components)
			component->update();

		for (const NodePtr& child : getChildren())
			reinterpret_cast<Entity*>(child)->update();
	}

	void Entity::onChange()
	{
		Transform::onChange();

		updateGlobalTransform();

		for (const NodePtr& child : getChildren())
			if (child != nullptr)
				reinterpret_cast<Entity*>(child)->onChange();
	}

	void Entity::updateGlobalTransform()
	{
		m_globalTransform = static_cast<Transform>(*this);

		const Entity* castParent = dynamic_cast<Entity*>(getParent());

		if (castParent != nullptr)
		{
			const Transform parentTransform = castParent->getGlobalTransform();
			const Matrix4 rotationMat = Matrix4::rotation(parentTransform.getRotation(), false);
			const Matrix4 translationMat = Matrix4::translation(parentTransform.getPosition());
			const Matrix4 scalingMat = Matrix4::scaling(parentTransform.getScale());
			const Vector4 posVec4 = Vector4(getPosition(), 1.f);
			m_globalTransform.setPosition((translationMat * scalingMat * rotationMat * posVec4).xyz());
			m_globalTransform.rotate(parentTransform.getRotation());
			m_globalTransform.scale(parentTransform.getScale());
		}
	}

	void Entity::removeChild(Node& child)
	{
		Node::removeChild(child);
		reinterpret_cast<Entity&>(child).updateGlobalTransform();
	}
}
