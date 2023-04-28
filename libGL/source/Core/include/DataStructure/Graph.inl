#pragma once
#include "Graph.h"

namespace LibGL::DataStructure
{
	template <class NodeT>
	Graph<NodeT>::~Graph()
	{
		clear();
	}

	template <class NodeT>
	template <typename DataT>
	DataT& Graph<NodeT>::addNode(DataT& node)
	{
		static_assert(std::is_same_v<NodeT, DataT> || std::is_base_of_v<NodeT, DataT>);
		const auto addedNode = new DataT(node);
		m_nodes.push_back(addedNode);
		return *addedNode;
	}

	template <class NodeT>
	template <typename DataT, typename ... Args>
	DataT& Graph<NodeT>::addNode(Args&&... args)
	{
		static_assert(std::is_same_v<NodeT, DataT> || std::is_base_of_v<NodeT, DataT>);
		const auto addedNode = new DataT(args...);
		m_nodes.push_back(addedNode);
		return *addedNode;
	}

	template <class NodeT>
	void Graph<NodeT>::removeNode(const NodeT& node)
	{
		const auto nodeIter = std::find(m_nodes.begin(), m_nodes.end(), new NodeT(&node));

		if (nodeIter != m_nodes.end())
		{
			delete *nodeIter;
			m_nodes.erase(nodeIter);
		}
	}

	template <class NodeT>
	std::vector<NodeT*>& Graph<NodeT>::getNodes()
	{
		return m_nodes;
	}

	template <class NodeT>
	std::vector<const NodeT*> Graph<NodeT>::getNodes() const
	{
		return std::vector<const NodeT*>(m_nodes.begin(), m_nodes.end());
	}

	template <class NodeT>
	void Graph<NodeT>::clear()
	{
		if (!m_nodes.empty())
		{
			for (const auto& node : m_nodes)
				delete node;

			m_nodes.clear();
		}
	}
}
