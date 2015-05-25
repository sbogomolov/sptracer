#ifndef KD_TREE_H
#define KD_TREE_H

#include "../stdafx.h"

namespace SPTracer
{
	struct SplittingPlane;
	class Box;
	class KdTreeNode;
	class Primitive;
	class Scene;

	class KdTree
	{
	public:
		KdTree(std::vector<std::shared_ptr<Primitive>> primitives);
		virtual ~KdTree();

		const KdTreeNode& rootNode() const;

	private:
		static const float TraverseStepCost;
		static const float IntersectionCost;

		std::unique_ptr<KdTreeNode> rootNode_;

		// recurent tree building procedure
		std::unique_ptr<KdTreeNode> Build(Box box, std::vector<std::shared_ptr<Primitive>> primitives);
		
		// splits the box with a plane
		std::tuple<Box, Box> SplitBox(const Box& box, const SplittingPlane& plane) const;
		
		// selects the side where primitives lying exactly on plane should belong
		// true - to the left side, false - to the right side
		std::tuple<float, bool> SelectCostSide(const Box& leftBox, const Box& rightBox, float surfaceArea, size_t leftCount, size_t rightCount, size_t middleCount) const;

		// gets the split cost
		float Cost(float leftProb, float rightProb, size_t leftCount, size_t rightCount) const;
	};

}

#endif
