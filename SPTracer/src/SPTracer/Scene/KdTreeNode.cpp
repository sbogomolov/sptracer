#include "../stdafx.h"
#include "../Primitive/Primitive.h"
#include "KdTreeNode.h"

namespace SPTracer
{
	KdTreeNode::KdTreeNode(Box box, std::vector<std::shared_ptr<Primitive>> primitives)
		: box_(std::move(box)), primitives_(std::move(primitives))
	{
	}

	KdTreeNode::KdTreeNode(Box box, std::unique_ptr<KdTreeNode> left, std::unique_ptr<KdTreeNode> right)
		: box_(std::move(box)), left_(std::move(left)), right_(std::move(right))
	{
	}

	KdTreeNode::~KdTreeNode()
	{
	}

	const KdTreeNode& KdTreeNode::left() const
	{
		return *left_;
	}

	const KdTreeNode& KdTreeNode::right() const
	{
		return *right_;
	}

	const Box& KdTreeNode::box() const
	{
		return box_;
	}

}
