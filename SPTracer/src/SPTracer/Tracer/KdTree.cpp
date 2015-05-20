#include "../stdafx.h"
#include "KdTree.h"
#include "KdTreeNode.h"

namespace SPTracer
{
	const float KdTree::TraverseStepCost = 0.3f;
	const float KdTree::IntersectionCost = 1.0f;

	KdTree::KdTree(const Scene& model)
	{
	}

	KdTree::~KdTree()
	{
	}

	const KdTreeNode& KdTree::rootNode() const
	{
		return *rootNode_;
	}

	std::unique_ptr<KdTreeNode> KdTree::Build(std::vector<Triangle> primitives, const Box& box)
	{
		if (false/* check terminate condition */)
		{
			// return std::make_unique<KdTreeNode>(std::move(primitives));
		}


		return std::unique_ptr<KdTreeNode>();
	}

}
