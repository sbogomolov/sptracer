#include "../stdafx.h"
#include "../Primitive/Primitive.h"
#include "KdTreeNode.h"

namespace SPTracer
{
	KdTreeNode::KdTreeNode(Box box, std::vector<std::shared_ptr<Primitive>> primitives)
		: box_(std::move(box)), primitives_(std::move(primitives)), isLeaf_(true)
	{
	}

	KdTreeNode::KdTreeNode(Box box, SplitPlane plane, std::shared_ptr<KdTreeNode> left, std::shared_ptr<KdTreeNode> right)
		: box_(std::move(box)), plane_(std::move(plane)), left_(std::move(left)), right_(std::move(right)), isLeaf_(false)
	{
	}

	KdTreeNode::~KdTreeNode()
	{
	}

	const Box& KdTreeNode::box() const
	{
		return box_;
	}

	const std::vector<std::shared_ptr<Primitive>>& KdTreeNode::primitives() const
	{
		return primitives_;
	}

	const SplitPlane & KdTreeNode::plane() const
	{
		return plane_;
	}

	const KdTreeNode& KdTreeNode::left() const
	{
		return *left_;
	}

	const KdTreeNode& KdTreeNode::right() const
	{
		return *right_;
	}

	const bool KdTreeNode::isLeaf() const
	{
		return isLeaf_;
	}

	const std::array<std::vector<std::shared_ptr<KdTreeNode>>, 6>& KdTreeNode::neighbours() const
	{
		return neighbours_;
	}

}
