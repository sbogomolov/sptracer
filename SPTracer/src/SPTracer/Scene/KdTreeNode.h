#ifndef SPT_KD_TREE_NODE_H
#define SPT_KD_TREE_NODE_H

#include "../stdafx.h"
#include "../Primitive/Box.h"
#include "SplitPlane.h"

namespace SPTracer
{
	class Primitive;

	class KdTreeNode
	{
		friend class KdTree;

	public:
		KdTreeNode(Box box, std::vector<std::shared_ptr<Primitive>> primitives);
		KdTreeNode(Box box, SplitPlane plane, std::shared_ptr<KdTreeNode> left, std::shared_ptr<KdTreeNode> right);
		virtual ~KdTreeNode();

		const Box& box() const;
		const std::vector<std::shared_ptr<Primitive>>& primitives() const;
		const SplitPlane& plane() const;
		const KdTreeNode& left() const;
		const KdTreeNode& right() const;
		const bool isLeaf() const;
		const std::array<std::vector<std::shared_ptr<KdTreeNode>>, 6>& neighbours() const;

	private:
		Box box_;
		std::vector<std::shared_ptr<Primitive>> primitives_;
		SplitPlane plane_;
		std::shared_ptr<KdTreeNode> left_;
		std::shared_ptr<KdTreeNode> right_;
		bool isLeaf_;
		std::array<std::vector<std::shared_ptr<KdTreeNode>>, 6> neighbours_;
	};

}

#endif
