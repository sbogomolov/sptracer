#include "../stdafx.h"
#include "KdTreeNode.h"

namespace SPTracer
{
	KdTreeNode::KdTreeNode(unsigned int dimension, float position, std::unique_ptr<KdTreeNode> left, std::unique_ptr<KdTreeNode> right)
		: dimension_(dimension), position_(position), left_(std::move(left)), right_(std::move(right))
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

	const unsigned int KdTreeNode::dimension() const
	{
		return dimension_;
	}

	const float KdTreeNode::position() const
	{
		return position_;
	}

}
