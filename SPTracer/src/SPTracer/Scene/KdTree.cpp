#include "../stdafx.h"
#include "../Util.h"
#include "../Primitive/Primitive.h"
#include "KdTree.h"
#include "KdTreeNode.h"
#include "SplittingPlane.h"

namespace SPTracer
{
	const float KdTree::TraverseStepCost = 0.3f;
	const float KdTree::IntersectionCost = 1.0f;

	KdTree::KdTree(std::vector<std::shared_ptr<Primitive>> primitives)
	{
		// get the bounding box for scene
		Vec3 min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		Vec3 max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

		for (const auto& p : primitives)
		{
			Box box = p->GetBoundingBox();

			min[0] = std::min(min[0], box.min()[0] - Util::Eps);
			max[0] = std::max(max[0], box.max()[0] + Util::Eps);

			min[1] = std::min(min[1], box.min()[1] - Util::Eps);
			max[1] = std::max(max[1], box.max()[1] + Util::Eps);

			min[2] = std::min(min[2], box.min()[2] - Util::Eps);
			max[2] = std::max(max[2], box.max()[2] + Util::Eps);
		}

		// build kd-Tree
		rootNode_ = Build(Box(std::move(min), std::move(max)), std::move(primitives));
	}

	KdTree::~KdTree()
	{
	}

	const KdTreeNode& KdTree::rootNode() const
	{
		return *rootNode_;
	}

	std::unique_ptr<KdTreeNode> KdTree::Build(Box box, std::vector<std::shared_ptr<Primitive>> primitives)
	{
		// find the best possible split
		
		// TODO: find the best split
		
		float minSplitCost;

		// check if it makes sense to split
		if (minSplitCost > (IntersectionCost * primitives.size()))
		{
			return std::make_unique<KdTreeNode>(std::move(box), std::move(primitives));
		}
		
		return std::unique_ptr<KdTreeNode>();
	}

	std::tuple<Box, Box> KdTree::SplitBox(const Box& box, const SplittingPlane& plane) const
	{
		// copy dimenstions from the original box
		Vec3 leftMin = box.min();
		Vec3 leftMax = box.max();
		Vec3 rightMin = box.min();
		Vec3 rightMax = box.max();

		// modify corresponding dimension
		leftMax[plane.dimension] = plane.position;
		rightMin[plane.dimension] = plane.position;

		// return tuple
		return std::make_tuple(
			Box(std::move(leftMin), std::move(leftMax)),
			Box(std::move(rightMin), std::move(rightMax)));
	}

	std::tuple<float, bool> KdTree::SelectCostSide(const Box& leftBox, const Box& rightBox, float surfaceArea, size_t leftCount, size_t rightCount, size_t middleCount) const
	{
		// get probabilities
		float leftProb = leftBox.GetSurfaceArea() / surfaceArea;
		float rightProb = rightBox.GetSurfaceArea() / surfaceArea;

		// get costs
		float costLeft = Cost(leftProb, rightProb, leftCount + middleCount, rightCount);
		float costRight = Cost(leftProb, rightProb, leftCount, rightCount + middleCount);
		
		// return the best cost
		if (costLeft < costRight)
		{
			return std::make_tuple(costLeft, true);
		}
		else
		{
			return std::make_tuple(costRight, false);
		}
	}

	float KdTree::Cost(float leftProb, float rightProb, size_t leftCount, size_t rightCount) const
	{
		float k = (leftCount == 0) || (rightCount == 0) ? 0.8f : 1.0f;
		return k * (TraverseStepCost + IntersectionCost * (leftProb * leftCount + rightProb * rightCount));
	}
}
