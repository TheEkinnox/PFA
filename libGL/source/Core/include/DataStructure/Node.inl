#pragma once
#include "Node.h"

namespace LibGL::DataStructure
{
	template <typename DataT>
	DataT& Node::addChild(DataT& child)
	{
		static_assert(std::is_same_v<Node, DataT> || std::is_base_of_v<Node, DataT>);
		const auto addedNode = std::make_shared<DataT>(child);
		m_children.push_back(addedNode);
		return *addedNode;
	}

	template <typename DataT, typename ... Args>
	DataT& Node::addChild(Args&&... args)
	{
		static_assert(std::is_same_v<Node, DataT> || std::is_base_of_v<Node, DataT>);
		const auto addedNode = std::make_shared<DataT>(this, args...);
		m_children.push_back(addedNode);
		return *addedNode;
	}
}
