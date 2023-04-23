#include "DataStructure/Graph.h"

namespace LibGL::DataStructure
{
	Node::Node() : m_id(s_currentId++)
	{
	}

	Node::Node(Node* parent)
		: m_parent(parent), m_id(s_currentId++)
	{
	}

	Node::Node(const Node& other) : m_parent(other.m_parent),
		m_children(other.m_children), m_id(s_currentId++)
	{
	}

	Node::Node(Node&& other) noexcept : m_parent(other.m_parent),
		m_children(std::move(other.m_children)), m_id(other.m_id)
	{
	}

	Node::~Node()
	{
		for (const NodePtr& child : m_children)
			child->m_parent = nullptr;

		if (m_parent != nullptr)
			m_parent->removeChild(*this);

		m_children.clear();
	}

	Node& Node::operator=(const Node& other)
	{
		if (this == &other)
			return *this;

		m_parent = other.m_parent;
		m_children = other.m_children;
		m_id = s_currentId++;

		return *this;
	}

	Node& Node::operator=(Node&& other) noexcept
	{
		if (this == &other)
			return *this;

		m_parent = other.m_parent;
		m_children = std::move(other.m_children);
		m_id = other.m_id;

		other.m_id = 0;
		other.m_children.clear();

		return *this;
	}

	bool Node::operator==(const Node& other) const
	{
		return other.m_id == m_id;
	}

	bool Node::operator!=(const Node& other) const
	{
		return !(*this == other);
	}

	Node* Node::getParent()
	{
		return m_parent;
	}

	Node* Node::getParent() const
	{
		return m_parent;
	}

	Node::NodeList Node::getChildren()
	{
		return m_children;
	}

	void Node::removeChild(Node& child)
	{
		const auto findFunc = [child](const NodePtr& node)
		{
			return *node == child;
		};

		const auto childIter = std::ranges::find_if(m_children, findFunc);

		if (childIter != m_children.end())
		{
			child.m_parent = nullptr;
			m_children.erase(childIter);
		}
	}
}
