#pragma once
#include <vector>

namespace LibGL::DataStructure
{
	class Node
	{
	public:
		typedef uint64_t NodeId;
		typedef Node* NodePtr;
		typedef std::vector<NodePtr> NodeList;

		Node(const Node& other);
		Node(Node&& other) noexcept;
		virtual ~Node();

		Node& operator=(const Node& other);
		Node& operator=(Node&& other) noexcept;

		bool operator==(const Node& other) const;
		bool operator!=(const Node& other) const;

		/**
		 * \brief Gets the node's parent
		 * \return A pointer to the node's parent
		 */
		Node* getParent();

		/**
		 * \brief Gets the node's parent
		 * \return A pointer to the node's parent
		 */
		Node* getParent() const;

		/**
		 * \brief Gets the node's children
		 * \return The node's children
		 */
		std::vector<NodePtr> getChildren();

		/**
		 * \brief Adds the given node as a child of the current node
		 * \param child A pointer to the child to add to the current node
		 * \return A reference to the added node
		 */
		template <typename DataT>
		DataT& addChild(DataT& child);

		/**
		 * \brief Adds the given node as a child of the current node
		 * \param args The arguments to pass to the created node's constructor
		 * \return A reference to the added node
		 */
		template <typename DataT, typename ... Args>
		DataT& addChild(Args&&... args);

		/**
		 * \brief Removes the given node from this node's children
		 * \param child A pointer to the child to remove from the node's children
		 */
		virtual void removeChild(Node& child);

	protected:
		Node();
		explicit Node(Node* parent);

	private:
		inline static NodeId	s_currentId = 1;

		Node*					m_parent = nullptr;
		NodeList				m_children;
		NodeId					m_id = 0;
	};
}

#include "Node.inl"
